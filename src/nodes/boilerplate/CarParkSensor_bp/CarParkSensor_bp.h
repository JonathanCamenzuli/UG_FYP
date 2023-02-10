/**
 * @file CarParkSensor_bp.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief Car Park Sensor Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 11/02/2023
 *
 * @section libraries Libraries
 * - ArduinoJson by @bblanchon (https://github.com/bblanchon/ArduinoJson)
 */

#ifndef __CPS_BP_H
#define __CPS_BP_H

#include <ArduinoJson.h>

/**
 * @brief Function returns serialised JSON document
 *
 * @param isCarParked     The `isCarParked` boolean variable
 * @return String         Serialised JSON document
 */
String serializeJson(bool &isCarParked)
{
    DynamicJsonDocument jsonDoc(64);

    // Set the values of the JSON packet
    jsonDoc["nodetype"] = "CPS";
    jsonDoc["id"] = "cps0001";

    // Creating and setting the value for the data nested object
    JsonObject data = jsonDoc.createNestedObject("data");
    data["isCarParked"] = isCarParked;

    // Create a string for storing the serialized JSON document
    String buffer;

    // Serialize the JSON document
    serializeJson(jsonDoc, buffer);

    // Free the memory occupied by the JSON document
    jsonDoc.clear();

    // Return the serialized JSON document as a string
    return buffer;
}

#endif