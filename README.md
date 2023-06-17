# Miniature implementation of an IoT-based Smart City

Final Year Project in Computer Engineering (2022/23)\
Jonathan Camenzuli

This repository contains the source code for the implementation discussed in the FYP write-up.

## Third-Party Libraries and Tools

### Server

- [Python Slim Docker Image Version 3.8](https://hub.docker.com/layers/library/python/3.8-slim/images/sha256-d017c2d3c37f312679e1c32bd6eb5b7bf101d29913833f4e74d14bf02425b57c?context=explore) by The Docker Community
- [InfluxDB Docker Image Version 2.6.1](https://hub.docker.com/layers/library/influxdb/2.6.1/images/sha256-62bb8be55010a76e14e3b11079cd4003e428ae90ec60497782e586dd9dbda94d?context=explore) by [InfluxData](https://github.com/influxdata)
- [Grafana Docker Image Version 9.3.6](https://hub.docker.com/layers/grafana/grafana/9.3.6/images/sha256-825065ced6a481f088d53b1c42fdfb9beadce6f87a07af614be1b1a78461e2bd?context=explore) by [Grafana Labs](https://grafana.com/)
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

### Testing and Evaluation
- [pySerial](https://github.com/pyserial/pyserial) by [pySerial](https://github.com/pyserial)
- [psutil](https://github.com/giampaolo/psutil) by [Giampaolo Rodola](https://github.com/giampaolo)
- [NumPy](https://github.com/numpy/numpy) by [NumPy](https://numpy.org/)
- [pandas](https://github.com/pandas-dev/pandas/) by [pandas](https://pandas.pydata.org/)
- [Matplotlib](https://github.com/matplotlib/matplotlib) by [Matplotlib](https://matplotlib.org/)
- [Adafruit INA219 Library](https://github.com/adafruit/Adafruit_INA219) by [Adafruit](https://github.com/adafruit)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306) by [Adafruit](https://github.com/adafruit)

## To-Do List

- [x] Architecture
  - [x] Establish communication protocol (CoAP)
  - [x] Establish what data is to be sent
- [ ] Arduinos
  - [x] Resolve hostname into IP address
  - [x] Libraries
    - [x] JSON serialisation
    - [x] CoAP packets
    - [x] MQ Sensors
      - [ ] MQ-4 Tuning
      - [ ] MQ-135 Tuning
  - [x] Boilerplate
    - [x] Car Park Sensor
    - ~~[ ] Fire Detection System~~
    - ~~[ ] Air Quality Monitoring~~
  - [x] Narrowband functionality
    - [x] Car Park Sensor
    - [x] Fire Detection System
    - [x] Air Quality Monitoring
- [x] Server Side Software
  - [x] Payload handler
  - [x] Database - InfluxDB
  - [x] Visualisation Platform - Grafana
  - [x] Containerization (Docker)
    - [x] CoAP Server
    - [x] InfluxDB
    - [x] Grafana
      - [x] Operational
      - [ ] Dashboard Setup
    - [x] Public IP Lookup
    - ~~[ ] DDNS?~~
  - [x] NoIP DNS
  - [x] Deployment
- [ ] Testing
  - [ ] Preperation
    - [ ] Power Consumption
    - [ ] Load Test
  - [ ] Power Consumption
  - [ ] Load Test