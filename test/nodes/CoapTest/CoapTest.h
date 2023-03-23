

/**
 * @file CarParkSensor.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief CoAP Test Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 15/03/2023
 *
 * @section libraries Libraries
 * - ArduinoJson by Benolt Blanchon (https://github.com/bblanchon/ArduinoJson)
 */

#ifndef __COAPTEST_H
#define __COAPTEST_H

#include <MKRNB.h>
#include <coap-simple.h>

#define SLEEP_TIME_MS 40000 // Sleep time, in milliseconds
#define BUF_SIZE 250        // Buffer Size

/**
 * @brief Function returns serialised JSON document for CoAP Test
 *
 * @return String Serialised JSON Document
 */
void serialiseJson(float &testVal, char *buffer);

bool connectNB(NB nbAccess, GPRS gprsAccess);

void sendPacket(IPAddress &coapServer_ip, Coap &coap, float &rand);

#endif