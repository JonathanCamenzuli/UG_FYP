/**
 * @file battery_consumption.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief Battery Consumption Monitor
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 12/05/2023
 *
 * @section hardware Hardware
 * -  Arduino Nano
 * -  INA219 DC Current Sensor Breakout Board
 * -  0.91" 128x32 Monochrome OLED Display
 */

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_INA219 ina219;

unsigned long previousMillis = 0;
unsigned long interval = 100;

// float shuntvoltage = 0;
// float busvoltage = 0;
float current_mA = 0;
// float loadvoltage = 0;
// float power_mW = 0;
// float energy = 0;

void setup(void)
{
  Serial.begin(9500);
  while (!Serial)
  {
    // will pause Zero, Leonardo, etc until serial console opens
    delay(1);
  }
  uint32_t currentFrequency;

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (!ina219.begin())
  {
    Serial.println("Failed to find INA219 chip");
    while (1)
    {
      delay(10);
    }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  // ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  // ina219.setCalibration_16V_400mA();
  Serial.println("Current (mA)");
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    ina219values();
    displaydata();
  }
  delay(500);
}

void ina219values()
{
  current_mA = ina219.getCurrent_mA();
  Serial.print(current_mA);
  Serial.println(" mA");
}

void displaydata()
{
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(current_mA);
  display.println("mA");
  display.display();
}