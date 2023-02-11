/**
 * @file AirQualitySystem_bp.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief Boilerplate for Air Quality Monitoring System Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 11/02/2023
 *
 * @section libraries Libraries
 * - ArduinoJson by @bblanchon (https://github.com/bblanchon/ArduinoJson)
 */

#ifndef __AQMS_BP_H
#define __AQMS_BP_H

#include <ArduinoJson.h>

/**
 * @brief Function returns average of an array
 *
 * @param array     The array itself
 * @param elems     Number of elements in array
 * @return float    Sum of array elements divided by number of elements
 */
float averageArray(int *array, int elems)
{
    long sum = 0L;
    for (int i = 0; i < elems - 1; i++)
        sum += array[i];
    return ((float)sum) / elems;
}

/**
 * @brief Function returns serialised JSON document for Air Quality Monitoring System
 *
 * @param temp      The `temperature_c` variable
 * @param hum       The `humidity_percent` variable
 * @param co        The `co_level_ppm` variable
 * @param co2       The `co2_level_ppm` variable
 * @return String   Serialised JSON document
 */
String serializeJson(float &temp, float &hum, float &co, float &co2)
{
    StaticJsonDocument<64> jsonDoc;

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

#endif