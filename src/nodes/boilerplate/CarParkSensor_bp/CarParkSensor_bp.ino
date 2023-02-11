/**
 * @file CarParkSensor_bp.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief Boilerplate for Car Park Sensor
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 26/11/2022
 *
 * @section libraries Libraries
 * - Low-Power by @rocketscream (https://github.com/rocketscream/Low-Power)
 * - General Utility Header File (../../fyp_utils/fyp_utils.h)
 * - Boilerplate for Car Park Sensor Header File (./CarParkSensor_bp.h)
 *
 * @section hardware Hardware
 * -  Arduino Nano
 * -  HC-SR04 Ultrasonic Sensor
 */

#include "LowPower.h"
#include "CarParkSensor_bp.h"

#define echoPin 3 // Attach Pin D3 Arduino Nano to pin Echo of HC-SR04
#define trigPin 2 // Attach Pin D2 Arduino Nano to pin Trig of HC-SR04

#define arrayMAX 10        // Number of elements for averaging array
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
  // HC-SR04 shenanigans
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // 9600 Baudrate
  Serial.begin(9600);
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

    // Putting Arduino to sleep for 40secs (supposedly)
    for (int i = 0; i < 5; i++)
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

    // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
  else
    distReadings_i++;
}