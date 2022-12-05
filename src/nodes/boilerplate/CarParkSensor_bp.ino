/**
 * @file CarParkSensor.ino
 *
 * @section description Description
 * Source code for the Car Park Sensor
 *
 * @section project Project
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @section libraries Libraries
 * - None (none)
 *   - None
 *
 * @section hardware Hardware
 * -  Arduino Nano
 * -  HC-SR04 Ultrasonic Sensor
 *
 * @section author Author
 * Created by Jonathan Camenzuli on 26/11/2022.
 *
 */

#include "../arduino_secrets.h"

#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 2 // attach pin D3 Arduino to pin Trig of HC-SR04

#define arrayMAX 20

// defines variables
int distReadings_i = 0;
int distReadings[arrayMAX];
float average;
long duration; // variable for the duration of sound wave travel
int distance;  // variable for the distance measurement
int isCarParked;

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);  // Sets the echoPin as an INPUT
  Serial.begin(1200);       // // Serial Communication is starting with 9600 of baudrate speed
}

void loop()
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  distReadings[distReadings_i] = distance;

  if (distReadings_i == 9)
  {
    average = averageArray(distReadings);
    if (average < 5)
    {
      Serial.println("Car is parked");
    }
    else
    {
      Serial.println("Car is not parked");
    }
    distReadings_i = 0;
  }
  else
  {
    distReadings_i++;
  }
}

float averageArray(int *array) // assuming array is int.
{
  long sum = 0L; // sum will be larger than an item, long for safety.
  for (int i = 0; i < arrayMAX - 1; i++)
    sum += array[i];
  return ((float)sum) / arrayMAX; // average will be fractional, so float may be appropriate.
}