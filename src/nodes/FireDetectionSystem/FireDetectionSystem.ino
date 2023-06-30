/**
 * @file FireDetectionSystem.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief Air Quality Monitoring System
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 09/05/2023
 *
 * @section libraries Libraries
 * - Fire Detection System Header File (./FireDetectionSystem.h)
 *
 * @section hardware Hardware
 * -  Arduino MKR NB 1500
 * -  DHT11 Temperature and Humidity Sensor
 * -  MQ4 Gas Sensor
 * -  Flame/IR Sensor
 */

#include <ArduinoLowPower.h>
#include <ArduinoHttpClient.h>
#include <MKRNB.h>
#include <MQUnifiedsensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <math.h>
#include "FireDetectionSystem.h"
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
MQUnifiedsensor mq4(MQ4_BOARD, MQ4_VOLTAGE_RES, MQ4_ADC_BIT_RES, MQ4_PIN, MQ4_TYPE);

/* ----------------------------------- VARIABLES ----------------------------------- */
float humReadings[ARRAY_MAX];
float humidity_percent;
float tempReadings[ARRAY_MAX];
float temperature_c;
float coReadings[ARRAY_MAX];
float co_level_ppm;
float smokeReadings[ARRAY_MAX];
float smoke_level_ppm;
float irReadings[ARRAY_MAX];
bool ir_detect;
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
  setupMQ4(mq4);
}

void loop() {
  // Update data, Voltage from MQ4_PIN will be read
  mq4.update();

  delay(500);

  humReadings[idx] = getHumidity(dht);
  tempReadings[idx] = getTemperature(dht);
  coReadings[idx] = getCO(mq4);
  smokeReadings[idx] = getSmokePPM(mq4);
  irReadings[idx] = getIR();

  if (idx == ARRAY_MAX - 1) {
    humidity_percent = averageArray(humReadings, ARRAY_MAX);
    temperature_c = averageArray(tempReadings, ARRAY_MAX);
    co_level_ppm = averageArray(coReadings, ARRAY_MAX);
    smoke_level_ppm = averageArray(smokeReadings, ARRAY_MAX);

    if (round(averageArray(irReadings, ARRAY_MAX)))
      ir_detect = true;
    else
      ir_detect = false;

    sendFDSData(temperature_c, humidity_percent, co_level_ppm, smoke_level_ppm, ir_detect, nbAccess, gprsAccess, coapServer_ip, nbClient, server, httpPort, coap);

    // After finishing reset index to 0
    idx = 0;

    // Forcing any lines to be printed before sleep
    Serial.flush();

    delay(5000);                    // Give ample time to uploading sketches
    USBDevice.detach();             // Terminating Serial Connection
    LowPower.sleep(SLEEP_TIME_MS);  // Putting Arduino to sleep
    USBDevice.attach();             // Restarting Serial Connection
    delay(5000);                    // Give time for Serial Connection to take place
    Serial.println("\nI am awaken!");
  } else
    idx++;
}