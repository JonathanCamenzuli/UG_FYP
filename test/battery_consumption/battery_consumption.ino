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

float current_mA = 0;

void setup(void)
{
  Serial.begin(9500);
  while (!Serial)
    ;

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Initialize the INA219.
  if (!ina219.begin())
  {
    Serial.println("Failed to find INA219 chip");
    while (1)
    {
      delay(10);
    }
  }

  // Using a 16V, 400mA range
  ina219.setCalibration_16V_400mA();
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

  delay(1000);
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