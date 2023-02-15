/**
 * @file UltrasonicSensor.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief HC-SR04 Sensor Test
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 15/02/2023
 *
 * @section hardware Hardware
 * -  Arduino Nano
 * -  HC-SR04 Ultrasonic Sensor
 */

// defines pins numbers
const int echoPin = 11;
const int trigPin = 10;

// defines variables
long duration;
int distance;

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

    // Prints distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
}