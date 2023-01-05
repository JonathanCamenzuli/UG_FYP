# Final Year Project

## Payload to be handled

### Parking Sensor

```json
{
  "sensortype": "CPS",
  "id": 0001,
  "data": {
    "isCarParked": true
  }
}
```

### Air Quality System

```json
{
  "sensortype": "AQS",
  "id": 0001,
  "data": {
    "temperature": 0001,
    "humidity": 0001,
    "co_level": 0001,
    "co2_level": 0001
  }
}
```

### Fire Detection System

```json
{
  "sensortype": "FDS",
  "id": 0001,
  "data": {
    "temperature": 0001,
    "isIRDetected": true,
    "isSmokeDetected": true
  }
}
```

## To-Do List

- [x] Architecture
  - [x] Establish communication protocol (CoAP)
  - [ ] Establish what data is to be sent
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
