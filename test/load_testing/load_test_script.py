##
# @file load_test_script.py
#
# @author Jonathan Camenzuli
#
# @brief Load Testing Script
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 23/06/2023
#

import requests
import asyncio
import os
from aiocoap import *
from dotenv import load_dotenv


async def send_aqms_json(ip_address, coap_port):
    """
    Send AQMS JSON Data

    This function sends a CoAP PUT request with a predefined AQMS payload to the specified IP address and port.

    Args:
        ip_address (str): The IP address of the CoAP server.
        coap_port (int): The CoAP port 
    """

    url = f"coap://{ip_address}:{coap_port}/aqms"
    context = await Context.create_client_context()
    payload = b'{"nodetype": "AQMS", "id": "aqms_load_test", "data": {"temperature_c": 29, "humidity_percent": 79, "co_level_ppm": 0.3, "co2_level_ppm": 405}}'
    request = Message(code=PUT, payload=payload, uri=url)
    response = await context.request(request).response
    print('Result: %s\n%r' % (response.code, response.payload))


async def send_cps_json(ip_address, coap_port):
    """
    Send CPS JSON Data

    This function sends a CoAP PUT request with a predefined CPS payload to the specified IP address and port.

    Args:
        ip_address (str): The IP address of the CoAP server.
        coap_port (int): The CoAP port 
    """

    url = f"coap://{ip_address}:{coap_port}/cps"
    context = await Context.create_client_context()
    payload = b'{"nodetype": "CPS", "id": "cps_load_test", "data": {"isCarParked": true}}'
    request = Message(code=PUT, payload=payload, uri=url)
    response = await context.request(request).response
    print('Result: %s\n%r' % (response.code, response.payload))


async def send_fds_json(ip_address, coap_port):
    """
    Send FDS JSON Data

    This function sends a CoAP PUT request with a predefined FDS payload to the specified IP address and port.

    Args:
        ip_address (str): The IP address of the CoAP server.
        coap_port (int): The CoAP port 
    """

    url = f"coap://{ip_address}:{coap_port}/fds"
    context = await Context.create_client_context()
    payload = b'{"nodetype": "FDS", "id": "fds_load_test", "data": {"temperature_c": 29, "humidity_percent": 79, "co_level_ppm": 0.3, "smoke_level_ppm": 1, "isIRDetected": false}}'
    request = Message(code=PUT, payload=payload, uri=url)
    response = await context.request(request).response
    print('Result: %s\n%r' % (response.code, response.payload))


def make_http_get_request(hostname):
    """
    Make HTTP GET Request

    This function makes an HTTP GET request to retrieve the IP address from the IP Lookup Server.

    Args:
        hostname (str): The hostname to make the request to.

    Returns:
        str: The IP address retrieved from the HTTP response.
    """

    response = requests.get("http://" + hostname)
    ip_address = response.text
    return ip_address


async def simulate_load_test(hostname, coap_port):
    """
    Perform Load Test

    This function simulates a load test by repeatedly retrieving the IP address and sending AQMS, CPS, and FDS JSON data to the retrieved IP address using CoAP PUT requests.

    Args:
        hostname (str): The hostname of the IP Lookup server
        coap_port (int): The CoAP port
    """

    for i in range(3):
        ip_address = make_http_get_request(hostname)

    await send_aqms_json(ip_address, coap_port)
    await send_cps_json(ip_address, coap_port)
    await send_fds_json(ip_address, coap_port)

if __name__ == "__main__":
    load_dotenv()

    hostname = os.getenv('COAP_DOMAIN')
    coap_port = int(os.getenv('COAP_PORT'))

    # Interval between sending JSON documents (in seconds)
    interval = 5

    # Number of remote clients to simulate
    num_clients = 3

    # Run the simulation
    asyncio.run(simulate_load_test(hostname, coap_port))
