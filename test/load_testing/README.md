# Load Testing Analysis

## Methodology - Load Testing
Source code used: `load_test_script.py`

Libraries and Dependencies:
- [aiocoap](https://github.com/chrysn/aiocoap) by [Christian Amsüss](https://github.com/chrysn)

This is an automated process which aims to simulate all three physical nodes making requests to the server. This is done by repeatedly retrieving the IP address and sending predefined JSON payloads.

### Environmental Variables
The script which load tests the server makes use of environmental variables which are stored in a `.env` file. Said `.env` file is not available in the public repo. The following environment variables are used in said `.env` file:

```.env
COAP_DOMAIN     # Domain of CoAP Server
COAP_PORT       # Port of CoAP Server
```
## Methodology - Resource Monitoring
Source code used: `resource_monitor.py`

Libraries and Dependencies:
- [psutil](https://github.com/giampaolo/psutil) by [Giampaolo Rodola](https://github.com/giampaolo)

The code utilizes the [psutil](https://github.com/giampaolo/psutil) library to measure the CPU, memory, disk, and temperature resources of the server at a specified time interval. The measurements are then recorded in a CSV file. The resource measurement loop runs for a predetermined duration, continuously gathering resource data and writing it to the CSV file. This methodology allows for the comprehensive monitoring of the server's resource utilization throughout the load test, enabling analysis and evaluation of its performance.

## Accessing Plots and Data
Any plots can either be accessed from the `./assets` directory or from the `load_testing.ipynb` notebook

Data can be accessed from the following files:
- `resource_data.csv`: Contains the resource utilisation information that was collected
- `timestamps.csv`: Contains the timestamps related to events such as receiving CoAP and HTTP requests
- `load_testing.ipynb`: Apart from the plots, it also contains descriptive statistics, some of which were featured in the FYP report.



