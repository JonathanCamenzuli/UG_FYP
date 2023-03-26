/**
 * @file IPLookup.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief IP Lookup Test Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 09/03/2023
 *
 * @section libraries Libraries
 * - MKRNB by Arduino Libraries (https://github.com/arduino-libraries/MKRNB)
 * - ArduinoHttpClient by Arduino Libraries (https://github.com/arduino-libraries/ArduinoHttpClient)
 */

#ifndef __IPLOOKUP_H
#define __IPLOOKUP_H

#include <MKRNB.h>
#include <ArduinoHttpClient.h>
#include "arduino_secrets.h"

/**
 * @brief Provides IP Address from IP Lookup Server
 *
 * @param ipAddress IPAddress Object where IP Address will be stored
 * @param httpClient HTTP Client Object
 */
void getIPAddress(IPAddress &ipAddress, HttpClient &httpClient);

#endif