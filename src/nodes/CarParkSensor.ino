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
 * -  Arduino Nano (Original)
 * -  Arduino MKR NB 1500 board
 * -  HC-SR04 Ultrasonic Sensor
 *
 * @section author Author
 * Created by Jonathan Camenzuli on 26/11/2022.
 *
 */

#include "arduino_secrets.h"

#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 2 // attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
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
  // Displays the distance on the Serial Monitor

  if (distance > 5)
    isCarParked = 0;
  else
    isCarParked = 1;

  Serial.print("isCarParked:");
  Serial.println(isCarParked);
}