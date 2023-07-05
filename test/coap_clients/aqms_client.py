##
# @file aqms_client.py
#
# @author Jonathan Camenzuli
#
# @brief Test Air Quality Monitoring System CoAP Client
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 06/01/2023
#

import logging
import asyncio
import os
from aiocoap import *
from dotenv import load_dotenv

logging.basicConfig(level=logging.INFO)

load_dotenv()

coap_domain = os.getenv('COAP_DOMAIN')
coap_port = os.getenv('COAP_PORT')

endpoint = "aqms"
uri = f"coap://{coap_domain}:{coap_port}/{endpoint}"


async def main():
    """
    Send AQMS JSON Data

    This function sends a CoAP PUT request with a predefined AQMS payload to an IP address and port.
    """

    context = await Context.create_client_context()

    await asyncio.sleep(2)

    payload = b'{"nodetype": "AQMS", "id": "aqmsTestClient", "data": {"temperature_c": 40, "humidity_percent": 55, "co_level_ppm": 55, "co2_level_ppm": 56}}'
    request = Message(code=PUT, payload=payload,
                      uri=uri)

    response = await context.request(request).response

    print('Result: %s\n%r' % (response.code, response.payload))

if __name__ == "__main__":
    asyncio.run(main())
