# Miniature implementation of an IoT-based Smart City

Final Year Project in Computer Engineering (2022/23)
Jonathan Camenzuli

## Payloads to be handled

### Test Payload

```json
{
  "nodetype": "TEST",
  "id": "test0001",
  "data": {
    "test_value": 1.1
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
    - [ ] CoAP packets
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
