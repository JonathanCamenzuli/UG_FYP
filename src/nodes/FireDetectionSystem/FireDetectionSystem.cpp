#include "FireDetectionSystem.h"
#include <ArduinoJson.h>
#include <Comms.h>
#include <MQUnifiedsensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "arduino_secrets.h"

void setupDHT11(DHT &dht) {
  Serial.print("DHT11: Setting up...");
  dht.begin();
  Serial.println("done.");
}

void setupMQ4(MQUnifiedsensor &mq4) {
  Serial.print("MQ4: Setting up...");
  float calcR0 = MQ4_R0_CALIBRATION;
  delay(20000);  // Preheat Time
  mq4.setRegressionMethod(0);
  mq4.init();
  mq4.setRL(1);
  mq4.setR0(calcR0 / 10);
  Serial.println("done.");

  if (isinf(calcR0)) {
    Serial.println("MQ4: WARNING: Conection issue, R0 is infinite - Check your wiring and supply and press RESET on your board");
    while (1)
      ;
  }

  if (calcR0 == 0) {
    Serial.println("MQ4: WARNING: Conection issue, R0 is zero - Check your wiring and supply and press RESET on your board");
    while (1)
      ;
  }
}

float getHumidity(DHT &dht) {
  float hum_percent = dht.readHumidity();
  if (isnan(hum_percent)) {
    Serial.println("DHT - getHumidity: Failed to read from DHT sensor! - Returning 0");
    return 0;
  }
  return hum_percent;
}

float getTemperature(DHT &dht) {
  float temp_c = dht.readTemperature();
  if (isnan(temp_c)) {
    Serial.println("DHT - getTemperature: Failed to read from DHT sensor! - Returning 0");
    return 0;
  }
  return temp_c;
}

float getCO(MQUnifiedsensor &mq4) {
  mq4.setA(-0.05849699);
  mq4.setB(0.75427267);
  float co_ppm = mq4.readSensor();
  return co_ppm;
}

float getSmokePPM(MQUnifiedsensor &mq4) {
  mq4.setA(-0.036579755);
  mq4.setB(0.6076452);
  float smoke_ppm = mq4.readSensor();
  return smoke_ppm;
}

int getIR() {
  int ir = digitalRead(IR_PIN);
  if (ir == HIGH)
    return 1;
  else
    return 0;
}

float averageArray(float *array, int elems) {
  float sum = 0;
  for (int i = 0; i < elems; i++)
    sum += array[i];
  return sum / elems;
}

void sendFDSData(float &temp_c, float &hum_percent, float &co_ppm, float &smoke_ppm, bool &ir_detect, NB &nbAccess, GPRS &gprsAccess, IPAddress &ipAddress, HttpClient &httpClient, Coap &coap) {
  // Create a string for storing the serialized JSON document
  char jsonDocBuf[JSON_BUF_SIZE];

  // Serialise and show JSON Document on Serial Monitor
  serialiseJson(temp_c, hum_percent, co_ppm, smoke_ppm, ir_detect, jsonDocBuf);
  Serial.println(jsonDocBuf);

  // Setup Modem for NB-IoT
  setupModem();

  // Check if connected and if not, reconnect to ISP
  if (nbAccess.status() != NB_READY || gprsAccess.status() != GPRS_READY) {
    connectNB(nbAccess, gprsAccess, SECRET_PINNUMBER, SECRET_GPRS_APN);
  }

  // Get IP Address of CoAP Server
  getIPAddress(ipAddress, httpClient);

  // Setting up CoAP Functionality
  Serial.print("Setting Up CoAP Functionality...");
  coap.start();
  Serial.println("done.");

  // Sending JSON document to CoAP Server
  sendPacket(ipAddress, coap, jsonDocBuf, SECRET_COAP_PORT, SECRET_COAP_ENDPOINT);

  // Allow Time Between Sending Packet and Shutting Down Modem
  delay(2000);

  // Disconnecting from ISP and turning off Modem
  Serial.print("Disconnecting from ISP and turning off Modem...");
  nbAccess.shutdown();
  Serial.println("done.");
}

void serialiseJson(float &temp_c, float &hum_percent, float &co_ppm, float &smoke_ppm, bool &ir_detect, char *buffer) {
  // Size calculated on https://arduinojson.org/v6/assistant/
  StaticJsonDocument<192> jsonDoc;

  // Set the values of the JSON packet
  jsonDoc["nodetype"] = "FDS";
  jsonDoc["id"] = "fds0001";

  // Creating and setting the value for the data nested object
  JsonObject data = jsonDoc.createNestedObject("data");
  data["temperature_c"] = temp_c;
  data["humidity_percent"] = hum_percent;
  data["co_level_ppm"] = co_ppm;
  data["smoke_level_ppm"] = smoke_ppm;
  data["isIRDetected"] = ir_detect;

  // Serialize the JSON document
  serializeJson(jsonDoc, buffer, JSON_BUF_SIZE);

  // Free the memory occupied by the JSON document
  jsonDoc.clear();
}