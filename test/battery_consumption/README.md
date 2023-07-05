# Battery Consumption Analysis
## Methodology - Data Collection
Source code used: `battery_consumption.ino`

Hardware used:
- Arduino Nano
- [INA219 DC Current Sensor Breakout Board](https://learn.adafruit.com/adafruit-ina219-current-sensor-breakout)
- [0.91" 128x32 Monochrome OLED Display](https://www.adafruit.com/product/661)

Libraries and Dependencies:
- [Adafruit INA219 Library](https://github.com/adafruit/Adafruit_INA219) by [Adafruit](https://github.com/adafruit)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306) by [Adafruit](https://github.com/adafruit)

The Arduino Nano collects load current data using the INA219 DC Current Sensor through the means of an I<sup>2</sup>C interface. It initializes hardware connections, sets up the INA219 sensor, and establishes serial communication. The code reads the current from the sensor every seconds. It is then shared by the means of Serial Connection and the OLED display.

In terms of connections, the positive input pin (V<sub>in+</sub>) on the INA219 board is connected to the 5V pin on the MKR NB 1500 board. The negative input pin (V<sub>in-</sub>) on the INA219 board is connected to the V<sub>CC</sub> pin on the MKR NB 1500 board.

## Methodology - Data Logging
Source code used: `data_logger.py`

Libraries and Dependencies:
- [pySerial](https://github.com/pyserial/pyserial) by [pySerial](https://github.com/pyserial)

The Python code acts as a data logger for the Arduino's load current measurements. It prompts the user for a filename and serial port, opens the serial port, and creates a file for logging data. The code continuously reads data from the serial port, parses it, and writes the timestamp and current value to a CSV file. It handles exceptions such as the serial connection stopping abruptly of keyboard interrupt by gracefully terminating and closing the serial port and file.

## Accessing Plots and Data
Any plots can either be accessed from the `./assets` directory or from the `battery_consumption.ipynb` notebook

Data can be accessed from the following directories/files:
- `./aqms`: Contains the battery consumption data for the Air Quality Monitoring System
- `./cps`: Contains the battery consumption data for the Car Park Sensor
- `./fds`: Contains the battery consumption data for the Fire Detection System
- `battery_consumption.ipynb`: Apart from the plots, it also contains descriptive statistics, which were featured in the FYP report.