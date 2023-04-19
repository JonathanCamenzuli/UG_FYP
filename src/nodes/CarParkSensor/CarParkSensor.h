/**
 * @file CarParkSensor.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief Car Park Sensor Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 11/02/2023
 */

#ifndef __CPS_H
#define __CPS_H

#include <MKRNB.h>
#include <ArduinoHttpClient.h>
#include <coap-simple.h>

#define SLEEP_TIME_MS 40000           // Sleep time, in milliseconds
#define PARKED_VEHICLE_THRESHOLD_CM 5 // Distance, in centimetres which indicates vehicle is parked

#define ECHO_PIN 7        // Attach Pin D5 Arduino MKR NB 1500 to pin Echo of HC-SR04
#define TRIG_PIN 6        // Attach Pin D4 Arduino MKR NB 1500 to pin Trig of HC-SR04
#define ARRAY_MAX 25      // Number of elements for averaging array
#define JSON_BUF_SIZE 256 // JSON Buffer Size

/**
 * @brief Function returns average of an array
 *
 * @param array     The array itself
 * @param elems     Number of elements in array
 * @return float    Sum of array elements divided by number of elements
 */
int getUltrasonicReading();

/**
 * @brief Function returns average of an array
 *
 * @param array     The array itself
 * @param elems     Number of elements in array
 * @return float    Sum of array elements divided by number of elements
 */
float averageArray(int *array, int elems);

void changeSendParkingState(bool &isVehicleParked, NB &nbAccess, GPRS &gprsAccess, IPAddress &ipAddress, HttpClient &httpClient, Coap &coap);

/**
 * @brief Creates Serialised JSON document for Car Park Sensor
 *
 * @param isCarParked   Boolean which indicates whether a vehicle is parked
 * @param buffer        Buffer to store serialised JSON document
 */
void serialiseJson(bool &isVehicleParked, char *buffer);

#endif