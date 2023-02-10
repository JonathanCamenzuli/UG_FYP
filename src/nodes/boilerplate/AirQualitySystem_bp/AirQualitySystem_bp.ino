/**
 * @file AirQualitySystem_bp.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief Boilerplate for Air Quality System
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date DD/MM/YYY
 *
 * @section libraries Libraries
 * - Low-Power by @rocketscream (https://github.com/rocketscream/Low-Power)
 * - ArduinoJson by @bblanchon (https://github.com/bblanchon/ArduinoJson)
 * - General Utility Header File (../../fyp_utils/fyp_utils.h)
 *
 * @section hardware Hardware
 * -  Arduino Nano
 * -  MQ135 Hazardous Gas Detection Module
 */

#include "LowPower.h"
#include "fyp_utils.h"
#include <ArduinoJson.h>

String serializeJson(float &temp, float &hum, float &co, float &co2)
{
    DynamicJsonDocument jsonDoc(64);

    // Set the values of the JSON packet
    jsonDoc["nodetype"] = "AQMS";
    jsonDoc["id"] = "aqms0001";

    // Creating and setting the value for the data nested object
    JsonObject data = jsonDoc.createNestedObject("data");
    data["temperature_c"] = temp;
    data["humidity_percent"] = hum;
    data["co_level_ppm"] = co;
    data["co2_level_ppm"] = co2;

    // Create a string for storing the serialized JSON document
    String buffer;

    // Serialize the JSON packet
    serializeJson(jsonDoc, buffer);

    // Free the memory occupied by the JSON document
    jsonDoc.clear();

    // Return the serialized JSON document as a string
    return buffer;
}