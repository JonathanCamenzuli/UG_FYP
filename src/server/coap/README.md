# CoAP Server

The source code for the CoAP server is devised as follows:

- `server.py`: Initialises the CoAP Server. This source file was set as the Entrypoint when the container is initiated 
- `resources.py`: Contains the resources used
- `influxdb.py`: Contains wrapper functionality related to InfluxDB

## Libraries and Dependencies
- [Python Slim 3.8 Docker Image](https://hub.docker.com/layers/library/python/3.8-slim/images/sha256-d017c2d3c37f312679e1c32bd6eb5b7bf101d29913833f4e74d14bf02425b57c?context=explore) by The Docker Community
- [aiocoap](https://github.com/chrysn/aiocoap) by [Christian Amsüss](https://github.com/chrysn)
- [influxdb-client-python](https://github.com/influxdata/influxdb-client-python) by [InfluxData](https://github.com/influxdata)
- [python-dotenv](https://github.com/theskumar/python-dotenv) by [Saurabh Kumar](https://github.com/theskumar)
- Other dependencies which are used by the above packages can be found in `./requirements.txt`

## Environmental Variables

In this folder, a `.env` is used and is not available in the public repo. The following environment variables are used in said `.env` file:

```.env
INFLUXDB_URL    # URL of InfluxDB Instance
INFLUXDB_TOKEN  # Token provided by InfluxDB
INFLUXDB_ORG    # InfluxDB Organisation
INFLUXDB_BUCKET # InfluxDB Bucket
```
