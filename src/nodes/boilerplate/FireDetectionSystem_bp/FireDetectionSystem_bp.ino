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
 * - General Utility Header File (../../fyp_utils/fyp_utils.h)
 *
 * @section hardware Hardware
 * -  Arduino Nano
 * -  isFlamePresent Sensor Module
 */

#include "LowPower.h"
#include "fyp_utils.h"

#define flameSensorPin 2 // Attach Pin D2 Arduino Nano to isFlamePresent Sensor

// #define arrayMAX 10        // Number of elements for averaging array

int isFlamePresent = HIGH;

void setup()
{
    // isFlamePresent Sensor shenanigans
    pinMode(flameSensorPin, INPUT);

    // 9600 Baudrate
    Serial.begin(9600);
}

void loop()
{
    isFlamePresent = digitalRead(flameSensorPin);
    if (isFlamePresent == LOW)
    {
        Serial.println("Fire is Detected");
    }
    else
    {
        Serial.println("No Fire is Detected");
    }
}