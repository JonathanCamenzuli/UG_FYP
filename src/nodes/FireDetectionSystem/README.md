# Fire Detection System

## Hardware used
- Arduino MKR NB 1500
- DHT11 Temperature and Humidity Sensor
- MQ4 Gas Sensor
- Flame/IR Sensor

## Libraries and Dependencies
- [ArduinoHttpLibrary](https://github.com/arduino-libraries/ArduinoHttpClient) by [Arduino Libraries](https://github.com/arduino-libraries)
- [ArduinoLowPower](https://github.com/arduino-libraries/ArduinoLowPower) by [Arduino Libraries](https://github.com/arduino-libraries)
- [MKRNB](https://github.com/arduino-libraries/MKRNB) by [Arduino Libraries](https://github.com/arduino-libraries)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson) by [Benoît Blanchon](https://github.com/bblanchon)
- [CoAP-simple-library](https://github.com/hirotakaster/CoAP-simple-library) by [Hirotaka Niisato](https://github.com/hirotakaster)
- [MQSensorsLib](https://github.com/miguel5612/MQSensorsLib) by [Miguel Angel Califa Urquiza](https://github.com/miguel5612)
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) by [Adafruit](https://github.com/adafruit)
- Communication Related Functionality Header File which is found in `./../Comms.h`
- `arduino_secrets.h`

## `arduino_secrets.h`

Any variables related to accessing the NB-IoT network should be stored in `arduino_secrets.h`. Furthermore, it is not present in the remote repo. The following variables are stored in `arduino_secrets.h`:

```cpp
#define SECRET_PINNUMBER        // SIM card PIN number
#define SECRET_GPRS_APN         // ISP APN
#define SECRET_COAP_ENDPOINT    // CoAP Endpoint
#define SECRET_COAP_PORT        // CoAP Server Port
#define SECRET_HTTP_PORT        // IP Lookup Server Port
#define SECRET_HOSTNAME         // Server hostname
```