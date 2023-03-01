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
 *
 * @section libraries Libraries
 * - ArduinoJson by Benolt Blanchon (https://github.com/bblanchon/ArduinoJson)
 */

#ifndef __CPS_H
#define __CPS_H

#include <ArduinoJson.h>

#define SLEEP_TIME_MS 40000           // Sleep time, in milliseconds
#define PARKED_VEHICLE_THRESHOLD_CM 5 // Distance, in centimetres which indicates vehicle is parked

#define echoPin 7    // Attach Pin D5 Arduino MKR NB 1500 to pin Echo of HC-SR04
#define trigPin 6    // Attach Pin D4 Arduino MKR NB 1500 to pin Trig of HC-SR04
#define arrayMAX 300 // Number of elements for averaging array

/**
 * @brief Function returns average of an array
 *
 * @param array     The array itself
 * @param elems     Number of elements in array
 * @return float    Sum of array elements divided by number of elements
 */
int getUltrasonicReading()
{
    long duration; // Duration of Ultrasonic wave travel
    int distance;  // Distance calculated

    // HC-SR04 shenanigans
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Distance Calculation
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    return distance
}

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
 * @brief Function returns serialised JSON document for Car Park Sensor
 *
 * @param isCarParked     The `isCarParked` boolean variable
 * @return String         Serialised JSON document
 */
String serializeJson(bool &isCarParked)
{
    // Size calculated on https://arduinojson.org/v6/assistant/
    StaticJsonDocument<96> jsonDoc;

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