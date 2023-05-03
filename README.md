# Miniature implementation of an IoT-based Smart City

Final Year Project in Computer Engineering (2022/23)\
Jonathan Camenzuli

## Libraries

### Server

- [aiocoap](https://github.com/chrysn/aiocoap) by [Christian Amsüss](https://github.com/chrysn)
- [influxdb-client-python](https://github.com/influxdata/influxdb-client-python) by [InfluxData](https://github.com/influxdata)
- [requests](https://github.com/psf/requests) by [Python Software Foundation](https://github.com/psf)
- [flask](https://github.com/pallets/flask) by [Pallets Projects](https://github.com/pallets)
- [python-dotenv](https://github.com/theskumar/python-dotenv) by [Saurabh Kumar](https://github.com/theskumar)

### Physical Nodes

- [ArduinoHttpLibrary](https://github.com/arduino-libraries/ArduinoHttpClient) by [Arduino Libraries](https://github.com/arduino-libraries)
- [ArduinoLowPower](https://github.com/arduino-libraries/ArduinoLowPower) by [Arduino Libraries](https://github.com/arduino-libraries)
- [MKRNB](https://github.com/arduino-libraries/MKRNB) by [Arduino Libraries](https://github.com/arduino-libraries)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson) by [Benoît Blanchon](https://github.com/bblanchon)
- [CoAP-simple-library](https://github.com/hirotakaster/CoAP-simple-library) by [Hirotaka Niisato](https://github.com/hirotakaster)
- [MQSensorsLib](https://github.com/miguel5612/MQSensorsLib) by [Miguel Angel Califa Urquiza](https://github.com/miguel5612)
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) by [Adafruit](https://github.com/adafruit)
- [Low-Power](https://github.com/rocketscream/Low-Power) by [Rocketscream Electronics](https://github.com/rocketscream)

## Payloads to be handled

### Test Payload

```json
{
  "nodetype": "TEST",
  "id": "test0001",
  "data": {
    "testValue": 1.1
  }
}
```

### Parking Sensor

```json
{
  "nodetype": "CPS",
  "id": "cps0001",
  "data": {
    "isCarParked": true
  }
}
```

### Air Quality Monitoring System

```json
{
  "nodetype": "AQMS",
  "id": "aqms0001",
  "data": {
    "temperature_c": 1,
    "humidity_percent": 1,
    "co_level_ppm": 1,
    "co2_level_ppm": 1
  }
}
```

### Fire Detection System

```json
{
  "nodetype": "FDS",
  "id": "fds0001",
  "data": {
    "temperature_c": 1,
    "isIRDetected": true,
    "isSmokeDetected": true
  }
}
```

## To-Do List

- [x] Architecture
  - [x] Establish communication protocol (CoAP)
  - [x] Establish what data is to be sent
- [ ] Arduinos
  - [x] Resolve hostname into IP address
  - [ ] Libraries
    - [x] JSON serialisation
    - [x] CoAP packets
    - [ ] MQ Sensors
  - [ ] Boilerplate
    - [x] Car Park Sensor
    - [ ] Fire Detection System
    - [ ] Air Quality Monitoring
  - [ ] Narrowband functionality
    - [ ] Car Park Sensor
    - [ ] Fire Detection System
    - [ ] Air Quality Monitoring
- [ ] Server Side Software
  - [x] Payload handler
  - [x] Database - InfluxDB
  - [x] Visualisation Platform - Grafana
  - [ ] Containerization (Docker)
    - [x] CoAP Server
    - [x] InfluxDB
    - [ ] Grafana
      - [x] Operational
      - [ ] Dashboard Setup
    - [x] Public IP Lookup
    - ~~[ ] DDNS?~~
  - [x] NoIP DNS
  - [ ] Deployment
