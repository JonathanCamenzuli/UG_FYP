/**
 * @file AirQualityMonitoringSystem.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief Air Quality Monitoring System
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 02/05/2023
 *
 * @section libraries Libraries
 * - Air Quality Monitoring System Header File (./AirQualityMonitoringSystem.h)
 *
 * @section hardware Hardware
 * -  Arduino MKR NB 1500
 * -  DHT11 Temperature and Humidity Sensor
 * -  MQ135 Air Quality Sensor
 */

#include <ArduinoLowPower.h>
#include <ArduinoHttpClient.h>
#include <MKRNB.h>
#include <MQUnifiedsensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "AirQualityMonitoringSystem.h"
#include "arduino_secrets.h"

/* ----------------------------------- ARDUINO SECRETS ----------------------------------- */
char pinNumber[] = SECRET_PINNUMBER;
char apn[] = SECRET_GPRS_APN;
char server[] = SECRET_HOSTNAME;
char endpoint[] = "/";
uint32_t httpPort = SECRET_HTTP_PORT;
char coapEndpoint[] = SECRET_COAP_ENDPOINT;
uint32_t coapPort = SECRET_COAP_PORT;

/* ----------------------------------- LIBRARY OBJECTS ----------------------------------- */
NBClient nbClient;
NBUDP udp;
Coap coap(udp, JSON_BUF_SIZE);
GPRS gprsAccess;
NB nbAccess(false);
IPAddress coapServer_ip;
DHT dht(DHT11_PIN, DHT11);
MQUnifiedsensor mq135(MQ135_BOARD, MQ135_VOLTAGE_RES, MQ135_ADC_BIT_RES, MQ135_PIN, MQ135_TYPE);

/* ----------------------------------- VARIABLES ----------------------------------- */
float humReadings[ARRAY_MAX];
float humidity_percent;
float tempReadings[ARRAY_MAX];
float temperature_c;
float coReadings[ARRAY_MAX];
float co_level_ppm;
float co2Readings[ARRAY_MAX];
float co2_level_ppm;
int idx = 0;

void setup() {
  // 9600 Baudrate
  Serial.begin(9600);

  // Waiting for Serial port to Connect - COMMENT FOR DEPLOYMENT
  while (!Serial)
    ;

  Serial.println("Hello from setup!");

  // DHT11 and MQ135 shenanigans
  setupDHT11(dht);
  setupMQ135(mq135);
}

void loop() {
  // Update data, Voltage from MQ135_PIN will be read
  mq135.update();

  delay(500);

  humReadings[idx] = getHumidity(dht);
  tempReadings[idx] = getTemperature(dht);
  coReadings[idx] = getCO(mq135);
  co2Readings[idx] = getCO2(mq135);

  if (idx == ARRAY_MAX - 1) {
    humidity_percent = averageArray(humReadings, ARRAY_MAX);
    temperature_c = averageArray(tempReadings, ARRAY_MAX);
    co_level_ppm = averageArray(coReadings, ARRAY_MAX);
    co2_level_ppm = averageArray(co2Readings, ARRAY_MAX);

    sendAQMSData(temperature_c, humidity_percent, co_level_ppm, co2_level_ppm, nbAccess, gprsAccess, coapServer_ip, nbClient, server, httpPort, coap);

    // After finishing reset index to 0
    idx = 0;

    Serial.println("I am going to sleep!");

    Serial.flush();                 // Forcing any lines to be printed before sleep
    USBDevice.detach();             // Terminating Serial Connection
    LowPower.sleep(SLEEP_TIME_MS);  // Putting Arduino to sleep
    USBDevice.attach();             // Restarting Serial Connection
    delay(3000);                    // Give time for Serial Connection to take place
    Serial.println("\nI am awaken!");
  } else
    idx++;
}