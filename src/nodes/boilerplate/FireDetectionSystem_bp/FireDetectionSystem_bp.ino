/**
 * @file FireDetectionSystem_bp.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief Boilerplate for Fire Detection System
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 13/12/2022
 *
 * @section libraries Libraries
 * - Low-Power by @rocketscream (https://github.com/rocketscream/Low-Power)
 * - ArduinoJson by @bblanchon (https://github.com/bblanchon/ArduinoJson)
 * - General Utility Header File (../../fyp_utils/fyp_utils.h)
 *
 * @section hardware Hardware
 * - Arduino Nano
 * - Flame Sensor Module
 * - MQ-135 Gas Sensor
 */

#include "LowPower.h"
#include "fyp_utils.h"
#include <ArduinoJson.h>

#define flameSensorPin 2 // Attach Pin D2 Arduino Nano to Flame Sensor
#define mqSensorPin 3    // Attach Pin D3 Arduino Nano to MQ-135 Sensor

#define arrayMAX 10  // Number of elements for averaging array
#define thresGas 600 // Threshold which indicates smoke is present

int isFlamePresent = HIGH;   // Flame sensor reading
int gasReading;              // Individual smoke reading
int gasReadings[arrayMAX];   // Array which is used for averaging
int gasReadings_i = 0;       // Index to be used with gasReadings[]
float gasAverage;            // Average of gasReadings[]
bool isFireDetected = false; // Is Fire Detected?

void setup()
{
    // Flame Sensor shenanigans
    pinMode(flameSensorPin, INPUT);

    // MQ-135 sensor shenanigans
    pinMode(mqSensorPin, INPUT);

    // 9600 Baudrate
    Serial.begin(9600);
}

void loop()
{
    // Flame sensor reading
    isFlamePresent = digitalRead(flameSensorPin);

    // MQ-135 readings
    gasReading = digitalRead(mqSensorPin);
    gasReadings[gasReadings_i] = gasReading;

    if (gasReadings_i == arrayMAX - 1)
    {
        gasAverage = averageArray(gasReadings, arrayMAX);
        if (isFlamePresent == LOW || gasAverage > thresGas)
        {
            if (!isFireDetected)
            {
                // Indicate that fire is ACTUALLY present and change state
                if (isFlamePresent == LOW)
                    Serial.println("Fire is DETECTED: Flame Sensor");
                if (gasAverage > thresGas)
                    Serial.println("Fire is DETECTED: Gas");

                isFireDetected = true;
                // Turn on radio and transmit change
            }
        }
        else
        {
            if (isFireDetected)
            {
                // Indicate that fire is ACTUALLY not present and change state
                Serial.println("No Fire is Detected");
                isFireDetected = false;
                // Turn on radio and transmit change
            }
        }
        // After finishing reset index to 0
        gasReadings_i = 0;

        // Forcing line to be printed before sleep
        Serial.flush();

        // Putting Arduino to sleep for 8secs
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
    else
        gasReadings_i++;
}

String serializeJson(float &temp, bool &isIRDetected, bool &isSmokeDetected)
{
    DynamicJsonDocument jsonDoc(64);

    // Set the values of the JSON packet
    jsonDoc["nodetype"] = "AQMS";
    jsonDoc["id"] = "aqms0001";

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