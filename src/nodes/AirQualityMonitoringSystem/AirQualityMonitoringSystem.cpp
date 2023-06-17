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

void setupMQ135(MQUnifiedsensor &mq135)
{
  Serial.print("MQ135: Setting up...");
  float calcR0 = MQ135_R0_CALIBRATION;
  delay(20000); // Preheat Time
  mq135.setRegressionMethod(1);
  mq135.init();
  mq135.setRL(10);
  mq135.setR0(calcR0 / 10);
  Serial.println("done.");

  if (isinf(calcR0))
  {
    Serial.println("MQ135: WARNING: Conection issue, R0 is infinite - Check your wiring and supply and press RESET on your board");
    while (1)
      ;
  }

  if (calcR0 == 0)
  {
    Serial.println("MQ135: WARNING: Conection issue, R0 is zero - Check your wiring and supply and press RESET on your board");
    while (1)
      ;
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

float getCO(MQUnifiedsensor &mq135)
{
  mq135.setA(-0.220061597);
  mq135.setB(0.653581876);
  float co_ppm = mq135.readSensor();
  return co_ppm;
}

float getCO2(MQUnifiedsensor &mq135)
{
  mq135.setA(-0.366725791);
  mq135.setB(0.763607977);
  float co2_ppm = mq135.readSensor();
  co2_ppm += 400;
  return co2_ppm;
}

float averageArray(float *array, int elems)
{
  float sum = 0;
  for (int i = 0; i < elems; i++)
    sum += array[i];
  return sum / elems;
}

void sendAQMSData(float &temp_c, float &hum_percent, float &co_ppm, float &co2_ppm, NB &nbAccess, GPRS &gprsAccess, IPAddress &ipAddress, HttpClient &httpClient, Coap &coap)
{
  // Create a string for storing the serialized JSON document
  char jsonDocBuf[JSON_BUF_SIZE];

  // Serialise and show JSON Document on Serial Monitor
  serialiseJson(temp_c, hum_percent, co_ppm, co2_ppm, jsonDocBuf);
  Serial.println(jsonDocBuf);

  // Setup Modem for NB-IoT
  setupModem();

  // Check if connected and if not, reconnect
  if (nbAccess.status() != NB_READY || gprsAccess.status() != GPRS_READY)
  {
    connectNB(nbAccess, gprsAccess);
  }

  // Get IP Address of CoAP Server
  getIPAddress(ipAddress, httpClient);

  // Setting up CoAP Functionality
  Serial.print("Setting Up CoAP Functionality...");
  coap.start();
  Serial.println("done.");

  // Sending JSON document to CoAP Server
  sendPacket(ipAddress, coap, jsonDocBuf);

  // Allow Time Between Sending Packet and Shutting Down Modem
  delay(1500);

  // Disconnecting from ISP and turning off Modem
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