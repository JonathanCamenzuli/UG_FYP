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
# - CoAP Resource Classes (./resources.py)
#

import aiocoap.resource as resource
import aiocoap
import threading
import logging
import asyncio
import resources

logging.basicConfig(level=logging.INFO,format='(%(asctime)s) %(message)s', datefmt='%d/%m/%Y %T %z %Z')
logging.getLogger("coap-server").setLevel(logging.DEBUG)


def main():
    # Resource tree creation
    root = resource.Site()
    basicResource = resources.BasicResource()
    cpsResource = resources.CPS_Resource()
    aqmsResource = resources.AQMS_Resource()
    fdsResource = resources.FDS_Resource()

    # TESTING PURPOSES ONLY
    testResource = resources.Test_Resource()

    root.add_resource(['cps'], cpsResource)
    root.add_resource(['aqms'], aqmsResource)
    root.add_resource(['fds'], fdsResource)

    # TESTING PURPOSES ONLY
    root.add_resource(['test'], testResource)

    asyncio.Task(aiocoap.Context.create_server_context(
        root, bind=('0.0.0.0', 5683)))

    # Spawn a daemon to notify observers when alarm status changes
    observers_notifier = threading.Thread(
        target=basicResource.notify_observers_check)
    observers_notifier.daemon = True
    observers_notifier.start()

    asyncio.get_event_loop().run_forever()


if __name__ == "__main__":
    main()
