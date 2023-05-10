##
# @file fds_client.py
#
# @author Jonathan Camenzuli
#
# @brief Test Fire Detection System CoAP Client
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 12/01/2023
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

endpoint = "fds"
uri = f"coap://{coap_domain}:{coap_port}/{endpoint}"


async def main():
    """Perform a single PUT request to localhost on the default port.
    The request is sent 2 seconds after initialization"""

    context = await Context.create_client_context()

    await asyncio.sleep(2)

    payload = b'{"nodetype": "FDS", "id": "fdsTestClient", "data": {"temperature_c": 25, "humidity_percent": 69, "co_level_ppm": 1, "smoke_level_ppm": 1, "isIRDetected": true}}'
    request = Message(code=PUT, payload=payload, uri=uri)

    response = await context.request(request).response

    print('Result: %s\n%r' % (response.code, response.payload))

if __name__ == "__main__":
    asyncio.run(main())
