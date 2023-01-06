# Miniature implementation of an IoT-based Smart City

Final Year Project in Computer Engineering (2022/23)
Jonathan Camenzuli

## Payloads to be handled

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
    "temperature": 1,
    "isIRDetected": true,
    "isSmokeDetected": true
  }
}
```

## To-Do List

- [x] Architecture
  - [x] Establish communication protocol (CoAP)
  - [x] Establish what data is to be sent
- [ ] Arduino Firmware
  - [ ] Boilerplate
    - [x] Car Park Sensor
    - [ ] Fire Detection System
    - [ ] Air Quality Monitoring
  - [ ] Narrowband functionality
    - [ ] Car Park Sensor
    - [ ] Fire Detection System
    - [ ] Air Quality Monitoring
- [ ] Server Side Software
  - [ ] Create CoAP Server
  - [ ] Database - InfluxDB
  - [ ] Visualisation Platform - Grafana
  - [ ] NoIP DNS
