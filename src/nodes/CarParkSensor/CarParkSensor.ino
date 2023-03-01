/**
 * @file CarParkSensor.ino
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
 * - ArduinoLowPower by Arduino (https://github.com/arduino-libraries/ArduinoLowPower)
 * - Car Park Sensor Header File (./CarParkSensor.h)
 *
 * @section hardware Hardware
 * -  Arduino MKR NB 1500
 * -  HC-SR04 Ultrasonic Sensor
 */

#include <ArduinoLowPower.h>
#include <MKRNB.h>
#include <coap-simple.h>
#include "CarParkSensor.h"
#include "Communication.h"

// Library Instances
NBClient client;
GPRS gprsAccess;
NB nbAccess;
NBUDP Udp;
Coap coap(Udp);

int distReadings[arrayMAX];   // Array which is used for averaging
int distReadings_i = 0;       // Index to be used with distReadings[]
float average;                // Average of distReadings[]
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
  distReadings[distReadings_i] = getUltrasonicReading();

  if (distReadings_i == arrayMAX - 1)
  {
    average = averageArray(distReadings, arrayMAX);
    if (average < PARKED_VEHICLE_THRESHOLD_CM)
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

    delay(7000);                   // Give ample time to uploading sketches
    USBDevice.detach();            // Terminating Serial Connection
    LowPower.sleep(SLEEP_TIME_MS); // Putting Arduino to sleep
    USBDevice.attach();            // Restarting Serial Connection
    delay(2000);                   // Give time for Serial Connection to take place
    Serial.println("\nI am awaken!");
  }
  else
    distReadings_i++;
}