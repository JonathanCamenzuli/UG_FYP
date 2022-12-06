/**
 * @file CarParkSensor_bp.ino
 *
 * @section description Description
 * Boilerplate code for the Car Park Sensor
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

#define echoPin 3             // attach pin D3 Arduino to pin Echo of HC-SR04
#define trigPin 2             // attach pin D2 Arduino to pin Trig of HC-SR04

#define arrayMAX 20           // Number of elements for averaging array

#define parkedVehicle_cm 5    // Distance which indicates vehicle is parked

int distReadings_i = 0;       // Index to be used with distReadings[]  
int distReadings[arrayMAX];   // Array which is used for averaging
float average;                // Average of distReadings[]
long duration;                // Duration of Ultrasonic wave travel
int distance;                 // Distance calculated
bool isVehicleParked = false; // Pretty self explanatory haha

void setup()
{
  // HC-SR04 shenanigans
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // 1200 Baudrate
  Serial.begin(1200);
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

  if (distReadings_i == 9)
  {
    average = averageArray(distReadings);
    if (average < 5)
    {
      if (!isCarParked)
      {
        // Indicate that car is ACTUALLY parked and change state
        Serial.println("Car is parked");
        isCarParked = true;
        // Turn on radio and transmit change in parking state
      }
    }
    else
    {
      if (isCarParked)
      {
        // Indicate that car is ACTUALLY not parked and change state
        Serial.println("Car is not parked");
        isCarParked = false;
        // Turn on radio and transmit change in parking state
      }
    }
    // After finishing reset index to 0
    distReadings_i = 0;
  }
  else
    distReadings_i++;
}

float averageArray(int* array)
{
  long sum = 0L;
  for (int i = 0; i < arrayMAX - 1; i++)
    sum += array[i];
  return ((float)sum) / arrayMAX;
}