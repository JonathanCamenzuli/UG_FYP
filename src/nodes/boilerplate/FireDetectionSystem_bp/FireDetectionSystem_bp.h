/**
 * @file FireDetectionSystem_bp.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief Boilerplate for Air Quality System Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 11/02/2023
 */

#ifndef __FDS_BP_H
#define __FDS_BP_H

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
 * @brief Function returns serialised JSON document for Fire Detection System
 *
 * @param temp              The `temperature_c` variable
 * @param isIRDetected      The `isIRDetected` variable
 * @param isSmokeDetected   The `isSmokeDetected` variable
 * @return String           Serialised JSON document
 */
String serializeJson(float &temp, bool &isIRDetected, bool &isSmokeDetected)
{
    StaticJsonDocument<96> jsonDoc;

    // Set the values of the JSON packet
    jsonDoc["nodetype"] = "FDS";
    jsonDoc["id"] = "fds0001";

    // Creating and setting the value for the data nested object
    JsonObject data = jsonDoc.createNestedObject("data");
    data["temperature_c"] = temp;
    data["isIRDetected"] = isIRDetected;
    data["isSmokeDetected"] = isSmokeDetected;

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