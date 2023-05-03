/**
 * @file AirQualityMonitoringSystem.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief Air Quality Monitoring System Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 02/05/2023
 */

#ifndef __AQMS_H
#define __AQMS_H

#include <MKRNB.h>
#include <ArduinoHttpClient.h>
#include <coap-simple.h>
#include <MQUnifiedsensor.h>
#include <DHT.h>
#include <DHT_U.h>

//#define SLEEP_TIME_MS 900000 // Sleep time, in milliseconds

#define SLEEP_TIME_MS 5000 // Sleep time, in milliseconds

#define DHT11_PIN 0       // Attach Pin A0 Arduino MKR NB 1500 to pin DHT11 output
#define MQ135_PIN A1      // Attach Pin A1 Arduino MKR NB 1500 to pin MQ-135 output
#define ARRAY_MAX 100      // Number of elements for averaging array
#define JSON_BUF_SIZE 256 // JSON Buffer Size

void setupDHT11(DHT &dht);

void setupMQ135(MQUnifiedsensor &MQ135);

/**
 * @brief Get the Humidity object
 *
 * @param dht       DHT Object
 * @return float    Humidity Percentage
 */
float getHumidity(DHT &dht);

/**
 * @brief Get the Temperature object
 *
 * @param dht       DHT Object
 * @return float    Temperature in Degrees Celsius (°C)
 */
float getTemperature(DHT &dht);

/**
 * @brief
 *
 * @param MQ135     MQUnifiedsensor Object
 * @return float    PPM level for Carbon Monoxide (CO)
 */
float getCO(MQUnifiedsensor &MQ135);

/**
 * @brief
 *
 * @param MQ135     MQUnifiedsensor Object
 * @return float    PPM level for Carbon Dioxide (CO2)
 */
float getCO2(MQUnifiedsensor &MQ135);

/**
 * @brief Function returns average of an array
 *
 * @param array     The array itself
 * @param elems     Number of elements in array
 * @return float    Sum of array elements divided by number of elements
 */
float averageArray(float *array, int elems);

/**
 * @brief Sends Air Quality Data to the CoAP server
 *
 * @param temp_c        Temperature in Degrees Celsius (°C)
 * @param hum_percent   Humidity Percentage
 * @param co_ppm        PPM level for Carbon Monoxide (CO)
 * @param co2_ppm       PPM level for Carbon Dioxide (CO2)
 * @param nbAccess      NB Object
 * @param gprsAccess    GPRS Object
 * @param ipAddress     IP Address to Server (Object)
 * @param httpClient    HTTP Client Object
 * @param coap          Coap Object
 */
void sendAQMSData(float &temp_c, float &hum_percent, float &co_ppm, float &co2_ppm, NB &nbAccess, GPRS &gprsAccess, IPAddress &ipAddress, HttpClient &httpClient, Coap &coap);

/**
 * @brief Creates Serialised JSON document for Car Park Sensor
 *
 * @param temp_c        Temperature in Degrees Celsius (°C)
 * @param hum_percent   Humidity Percentage
 * @param co_ppm        PPM level for Carbon Monoxide (CO)
 * @param co2_ppm       PPM level for Carbon Dioxide (CO2)
 * @param buffer        Buffer to store serialised JSON document
 */
void serialiseJson(float &temp_c, float &hum_percent, float &co_ppm, float &co2_ppm, char *buffer);

#endif