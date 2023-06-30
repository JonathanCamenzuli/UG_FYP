/**
 * @file FireDetectionSystem.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief Fire Detection System Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 09/05/2023
 */

#ifndef __FDS_H
#define __FDS_H

#include <MKRNB.h>
#include <ArduinoHttpClient.h>
#include <coap-simple.h>
#include <MQUnifiedsensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define SLEEP_TIME_MS 90000 // Sleep time, in milliseconds (90 secs)

#define DHT11_PIN 0 // Attach Pin D0 Arduino MKR NB 1500 to pin DHT11 output

#define MQ4_BOARD "Arduino MKR NB 1500"
#define MQ4_VOLTAGE_RES 5
#define MQ4_PIN A1 // Attach Pin A1 Arduino MKR NB 1500 to pin MQ-4 output
#define MQ4_TYPE "MQ-4"
#define MQ4_ADC_BIT_RES 10
#define MQ4_R0_CALIBRATION 48.79

#define IR_PIN 1 // Attach Pin D1 Arduino MKR NB 1500 to pin IR output

#define ARRAY_MAX 50      // Number of elements for averaging array
#define JSON_BUF_SIZE 256 // JSON Buffer Size

/**
 * @brief Function which sets up the DHT11 sensor
 *
 * @param dht   DHT Object
 */
void setupDHT11(DHT &dht);

/**
 * @brief Function which sets up the MQ4 sensor
 *
 * @param mq4   MQUnifiedsensor Object
 */
void setupMQ4(MQUnifiedsensor &mq4);

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
 * @param mq4       MQUnifiedsensor Object
 * @return float    PPM level for Carbon Monoxide (CO)
 */
float getCO(MQUnifiedsensor &mq4);

/**
 * @brief
 *
 * @param mq4       MQUnifiedsensor Object
 * @return float    PPM level for Smoke
 */
float getSmokePPM(MQUnifiedsensor &mq4);

/**
 * @brief
 *
 * @return 1     Flame Detected!
 * @return 0    Flame not Detected!
 */
int getIR();

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
 * @param smoke_ppm     PPM level for Smoke
 * @param ir_detect     Boolean which indicates whether Flame is Present
 * @param nbAccess      NB Object
 * @param gprsAccess    GPRS Object
 * @param ipAddress     IP Address to Server (Object)
 * @param httpClient    HTTP Client Object
 * @param coap          Coap Object
 */
void sendFDSData(float &temp_c, float &hum_percent, float &co_ppm, float &smoke_ppm, bool &ir_detect, NB &nbAccess, GPRS &gprsAccess, IPAddress &ipAddress, NBClient &nbClient, char *server, uint32_t httpPort, Coap &coap);

/**
 * @brief Creates Serialised JSON document for Car Park Sensor
 *
 * @param temp_c        Temperature in Degrees Celsius (°C)
 * @param hum_percent   Humidity Percentage
 * @param co_ppm        PPM level for Carbon Monoxide (CO)
 * @param smoke_ppm     PPM level for Smoke
 * @param ir_detect     Boolean which indicates whether Flame is Present
 * @param buffer        Buffer to store serialised JSON document
 */
void serialiseJson(float &temp_c, float &hum_percent, float &co_ppm, float &smoke_ppm, bool &ir_detect, char *buffer);

#endif