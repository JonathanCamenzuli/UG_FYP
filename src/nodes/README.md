# Physical Nodes Source Code

This folder contains four seperate sketches:

- `./CarParkSensor`: Concerned with the Car Park Sensor
- `./boilerplate/CarParkSensor_bp`: Concerned with the Car Park Sensor, with the difference that it is boilerplate developed when the MKR NB 1500 was not at my disposal
- `./AirQualityMonitoringSystem`: Concerned with the Air Quality Monitoring System
- `./FireDetectionSystem`: Concerned with the Fire Detection System

## Environment

The sketches were uploaded to the MKR NB 1500 by making use of the Arduino IDE. The installation of the SAMD21 core for MKR boards is required. More info on that [here](https://docs.arduino.cc/software/ide-v1/tutorials/getting-started/cores/arduino-samd)

## Communication Related Functionality

### Libraries and Dependencies:
- [ArduinoHttpLibrary](https://github.com/arduino-libraries/ArduinoHttpClient) by [Arduino Libraries](https://github.com/arduino-libraries)
- [MKRNB](https://github.com/arduino-libraries/MKRNB) by [Arduino Libraries](https://github.com/arduino-libraries)
- [CoAP-simple-library](https://github.com/hirotakaster/CoAP-simple-library) by [Hirotaka Niisato](https://github.com/hirotakaster)

All source code related to communication is in the following source files:
- `Comms.cpp`
- `Comms.h`

The current setup requires both source files to be placed in the `libraries` Arduino folder. More info on that [here](https://docs.arduino.cc/hacking/software/Libraries)