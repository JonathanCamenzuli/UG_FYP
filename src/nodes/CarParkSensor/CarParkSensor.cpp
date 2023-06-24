#include "CarParkSensor.h"
#include <ArduinoJson.h>
#include <Comms.h>
#include "arduino_secrets.h"

int getUltrasonicReading()
{
  long duration; // Duration of Ultrasonic wave travel
  int distance;  // Distance calculated

  // HC-SR04 shenanigans
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Distance Calculation
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

float averageArray(int *array, int elems)
{
  float sum = 0;
  for (int i = 0; i < elems; i++)
    sum += array[i];
  return sum / elems;
}

void changeSendParkingState(bool &isVehicleParked, NB &nbAccess, GPRS &gprsAccess, IPAddress &ipAddress, HttpClient &httpClient, Coap &coap)
{
  // Create a string for storing the serialized JSON document
  char jsonDocBuf[JSON_BUF_SIZE];

  // Serialise and show JSON Document on Serial Monitor
  serialiseJson(isVehicleParked, jsonDocBuf);
  Serial.println(jsonDocBuf);

  // Setup Modem for NB-IoT
  setupModem();

  // Check if connected and if not, reconnect to ISP
  if (nbAccess.status() != NB_READY || gprsAccess.status() != GPRS_READY)
  {
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

void serialiseJson(bool &isCarParked, char *buffer)
{
  // Size calculated on https://arduinojson.org/v6/assistant/
  StaticJsonDocument<96> jsonDoc;

  // Set the values of the JSON packet
  jsonDoc["nodetype"] = "CPS";
  jsonDoc["id"] = "cps0001";

  // Creating and setting the value for the data nested object
  JsonObject data = jsonDoc.createNestedObject("data");
  data["isCarParked"] = isCarParked;

  // Serialize the JSON document
  serializeJson(jsonDoc, buffer, JSON_BUF_SIZE);

  // Free the memory occupied by the JSON document
  jsonDoc.clear();
}