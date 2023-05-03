#include "AirQualityMonitoringSystem.h"
#include "Comms.h"
#include "arduino_secrets.h"
#include <ArduinoJson.h>
#include <MKRNB.h>
#include <ArduinoHttpClient.h>
#include <coap-simple.h>
#include <MQUnifiedsensor.h>
#include <DHT.h>
#include <DHT_U.h>

void setupDHT11(DHT &dht)
{
  Serial.print("DHT11: Setting up...");
  dht.begin();
  Serial.println("done.");
}

void setupMQ135(MQUnifiedsensor &MQ135)
{
  float calcR0 = 0;
  MQ135.setRegressionMethod(1);
  MQ135.init();
  MQ135.setRL(10);
  Serial.print("MQ135: Calibrating...");
  for(int i = 1; i <= 10; i++)
  {
    MQ135.update();
    calcR0 += MQ135.calibrate(3.6); // RS/R0 = 3.6 ppm
    Serial.print(".");
  }
  MQ135.setR0(calcR0/10);
  Serial.println("done.");

  if(isinf(calcR0))
  {
    Serial.println("MQ135: WARNING: Conection issue, R0 is infinite - Check your wiring and supply and press RESET on your board");
    while(1);
  }

  if(calcR0 == 0)
  {
    Serial.println("MQ135: WARNING: Conection issue, R0 is zero - Check your wiring and supply and press RESET on your board");
    while(1);
  }
}

float getHumidity(DHT &dht)
{
  float hum_percent = dht.readHumidity();
  if (isnan(hum_percent))
  {
      Serial.println("DHT - getHumidity: Failed to read from DHT sensor! - Returning 0");
      return 0;
  }
  return hum_percent;
}

float getTemperature(DHT &dht)
{
  float temp_c = dht.readTemperature();
  if (isnan(temp_c))
  {
      Serial.println("DHT - getTemperature: Failed to read from DHT sensor! - Returning 0");
      return 0;
  }
  return temp_c;
}

float getCO(MQUnifiedsensor &MQ135)
{
  MQ135.setA(605.18);
  MQ135.setB(-3.937);
  float co_ppm = MQ135.readSensor();
  return co_ppm;
}

float getCO2(MQUnifiedsensor &MQ135)
{
  MQ135.setA(110.47);
  MQ135.setB(-2.862);
  float co2_ppm = MQ135.readSensor();
  return co2_ppm;
}

float averageArray(float *array, int elems)
{
  long sum = 0L;
  for (int i = 0; i < elems - 1; i++)
      sum += array[i];
  return ((float)sum) / elems;
}

void sendAQMSData(float &temp_c, float &hum_percent, float &co_ppm, float &co2_ppm, NB &nbAccess, GPRS &gprsAccess, IPAddress &ipAddress, HttpClient &httpClient, Coap &coap)
{
  // Create a string for storing the serialized JSON document
  char jsonDocBuf[JSON_BUF_SIZE];

  serialiseJson(temp_c, hum_percent, co_ppm, co2_ppm, jsonDocBuf);

  // Turn on radio and transmit change in parking state
  Serial.println(jsonDocBuf);

  setupModem();
  // Check if connected and if not, reconnect
  if (nbAccess.status() != NB_READY || gprsAccess.status() != GPRS_READY)
  {
      connectNB(nbAccess, gprsAccess);
  }
  getIPAddress(ipAddress, httpClient);

  Serial.print("Setting Up CoAP Functionality...");
  coap.start();
  Serial.println("done.");

  sendPacket(ipAddress, coap, jsonDocBuf);

  Serial.print("Disconnecting from ISP and turning off Modem...");
  nbAccess.shutdown();
  Serial.println("done.");
}

void serialiseJson(float &temp_c, float &hum_percent, float &co_ppm, float &co2_ppm, char *buffer)
{
  // Size calculated on https://arduinojson.org/v6/assistant/
  StaticJsonDocument<192> jsonDoc;

  // Set the values of the JSON packet
  jsonDoc["nodetype"] = "AQMS";
  jsonDoc["id"] = "aqms0001";

  // Creating and setting the value for the data nested object
  JsonObject data = jsonDoc.createNestedObject("data");
  data["temperature_c"] = temp_c;
  data["humidity_percent"] = hum_percent;
  data["co_level_ppm"] = co_ppm;
  data["co2_level_ppm"] = co2_ppm;

  // Serialize the JSON document
  serializeJson(jsonDoc, buffer, JSON_BUF_SIZE);

  // Free the memory occupied by the JSON document
  jsonDoc.clear();
}