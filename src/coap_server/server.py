##
# @file server.py
#
# @author Jonathan Camenzuli
#
# @brief CoAP server implementation
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 23/12/2022
#
# @section libraries Libraries
# - aiocoap by @chrysn (https://github.com/chrysn/aiocoap)
# - General Utilities (./utils)
#

import aiocoap.resource as resource
import aiocoap
import threading
import logging
import asyncio
from utils import resources

logging.basicConfig(level=logging.INFO)
logging.getLogger("coap-server").setLevel(logging.DEBUG)


def main():
    # Resource tree creation
    root = resource.Site()
    basicResource = resources.BasicResource()
    cpsResource = resources.CPS_Resource()

    root.add_resource(['cps'], cpsResource)

    asyncio.Task(aiocoap.Context.create_server_context(
        root, bind=('localhost', 5683)))

    # Spawn a daemon to notify observers when alarm status changes
    observers_notifier = threading.Thread(
        target=basicResource.notify_observers_check)
    observers_notifier.daemon = True
    observers_notifier.start()

    asyncio.get_event_loop().run_forever()


if __name__ == "__main__":
    main()
