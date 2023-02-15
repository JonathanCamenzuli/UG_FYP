/**
 * @file CarParkSensor_bp.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief Car Park Sensor
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 11/02/2023
 *
 * @section libraries Libraries
 * - ArduinoLowPower by @arduino-libraries (https://github.com/arduino-libraries/ArduinoLowPower)
 * - Car Park Sensor Header File (./CarParkSensor.h)
 *
 * @section hardware Hardware
 * -  Arduino MKR NB 1500
 * -  HC-SR04 Ultrasonic Sensor
 */

#include <ArduinoLowPower.h>
#include "CarParkSensor.h"

#define echoPin 7 // Attach Pin D5 Arduino MKR NB 1500 to pin Echo of HC-SR04
#define trigPin 6 // Attach Pin D4 Arduino MKR NB 1500 to pin Trig of HC-SR04

#define arrayMAX 300       // Number of elements for averaging array
#define parkedVehicle_cm 5 // Distance which indicates vehicle is parked

int distReadings[arrayMAX];   // Array which is used for averaging
int distReadings_i = 0;       // Index to be used with distReadings[]
float average;                // Average of distReadings[]
long duration;                // Duration of Ultrasonic wave travel
int distance;                 // Distance calculated
bool isVehicleParked = false; // Pretty self explanatory haha
String jsonString;            // JSON Payload

void setup()
{
  // 9600 Baudrate
  Serial.begin(9600);

  // HC-SR04 shenanigans
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{
  // HC-SR04 shenanigans
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Distance Calculation
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  distReadings[distReadings_i] = distance;

  if (distReadings_i == arrayMAX - 1)
  {
    average = averageArray(distReadings, arrayMAX);
    if (average < parkedVehicle_cm)
    {
      if (!isVehicleParked)
      {
        // Indicate that car is ACTUALLY parked and change state
        Serial.println("Car is parked");

        isVehicleParked = true;
        jsonString = serializeJson(isVehicleParked);

        // Turn on radio and transmit change in parking state
        Serial.println(jsonString);
      }
    }
    else
    {
      if (isVehicleParked)
      {
        // Indicate that car is ACTUALLY not parked and change state
        Serial.println("Car is not parked");

        isVehicleParked = false;
        jsonString = serializeJson(isVehicleParked);

        // Turn on radio and transmit change in parking state
        Serial.println(jsonString);
      }
    }
    // After finishing reset index to 0
    distReadings_i = 0;

    // Forcing line to be printed before sleep
    Serial.flush();

    // Putting Arduino to sleep for 40secs
    //LowPower.sleep(SLEEP_TIME);
  }
  else
    distReadings_i++;
}