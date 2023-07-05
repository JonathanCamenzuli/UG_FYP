# Miniature implementation of an IoT-based Smart City
_Final Year Project (FYP) in Computer Engineering (2022/23)_

This repository contains the source code for the implementation discussed in the Final Year Project (FYP) report. The project was submitted as part of the requirements for the degree of B.Sc. (Hons.) in Computer Engineering at the University of Malta.

## Project Description

The project presents a scalable miniature implementation of an IoT-based smart city model, consisting of three physical nodes that perform air quality monitoring, parking sensing and fire detection. Data is sent to a server by making use of NB-IoT as an LPWAN and CoAP as a Application Layer Protocol.

The purpose of this project is to demonstrate the feasibility of an IoT-based smart city system and showcase the integration of various sensors and communication protocols. The provided source code serves as a reference for understanding the implementation details discussed in the FYP report.

Please refer to the FYP report for a comprehensive explanation of the project's objectives, methodologies, and findings.

## Repo Structure

The repo is mainly devised as follows:

```markdown
- src
    - nodes
    - server
- test
```

- The `src` directory contains the source code for the project.
    - The `src/nodes` subdirectory contains the source code related to the physical nodes.
    - The `src/server` subdirectory contains the source code related to the server.
- The `test` directory contains the source code related to testing components, including sensors, load testing, and battery consumption analysis.

## Third-Party Libraries and Tools

### Server

- [Python Slim 3.7 Docker Image](https://hub.docker.com/layers/library/python/3.7-slim/images/sha256-05be27b7e8722b0dfd691d8352e7b7c6bedacc8c43343711db98bd3b1a2d8177?context=explore) by The Docker Community
- [Python Slim 3.8 Docker Image](https://hub.docker.com/layers/library/python/3.8-slim/images/sha256-d017c2d3c37f312679e1c32bd6eb5b7bf101d29913833f4e74d14bf02425b57c?context=explore) by The Docker Community
- [InfluxDB 2.6.1 Docker Image](https://hub.docker.com/layers/library/influxdb/2.6.1/images/sha256-62bb8be55010a76e14e3b11079cd4003e428ae90ec60497782e586dd9dbda94d?context=explore) by [InfluxData](https://github.com/influxdata)
- [Grafana 9.3.6 Docker Image](https://hub.docker.com/layers/grafana/grafana/9.3.6/images/sha256-825065ced6a481f088d53b1c42fdfb9beadce6f87a07af614be1b1a78461e2bd?context=explore) by [Grafana Labs](https://grafana.com/)
- [aiocoap](https://github.com/chrysn/aiocoap) by [Christian Amsüss](https://github.com/chrysn)
- [influxdb-client-python](https://github.com/influxdata/influxdb-client-python) by [InfluxData](https://github.com/influxdata)
- [requests](https://github.com/psf/requests) by [Python Software Foundation](https://github.com/psf)
- [flask](https://github.com/pallets/flask) by [Pallets Projects](https://github.com/pallets)
- [python-dotenv](https://github.com/theskumar/python-dotenv) by [Saurabh Kumar](https://github.com/theskumar)
- [python-ipify](https://github.com/rdegges/python-ipify) by [Randall Degges](https://github.com/rdegges)

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