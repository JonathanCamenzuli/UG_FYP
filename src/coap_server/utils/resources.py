##
# @file resources.py
#
# @author Jonathan Camenzuli
#
# @brief CoAP Resource Classes
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 23/12/2022
#
# @section libraries Libraries
# - aiocoap by @chrysn (https://github.com/chrysn/aiocoap)
#

import aiocoap.resource as resource
import aiocoap


class BasicResource(resource.ObservableResource):
    """A basic resource which supports GET and PUT methods.
    """

    def __init__(self):
        super().__init__()

        self.has_observers = False
        self.notify_observers = False

    def notify_observers_check(self):
        while True:
            if self.has_observers and self.notify_observers:
                print('notifying observers')
                self.updated_state()
                self.notify_observers = False

    def update_observation_count(self, count):
        if count:
            self.has_observers = True
        else:
            self.has_observers = False


class AQS_Resource(BasicResource):
    """Air Quality System Resource

    Args:
        BasicResource (_type_): _description_
    """

    def __init__(self):
        super().__init__()

        self.status_ppm = 0

    async def render_get(self, request):
        print(f'Air Quality System State: {self.status_ppm}')
        payload = b'%s PPM' % str(self.status_ppm).encode('ascii')

        return aiocoap.Message(payload=payload)

    async def render_put(self, request):
        payload = request.payload.decode('ascii')
        print(payload)
        self.status_ppm = payload

        return aiocoap.Message(code=aiocoap.CHANGED, payload=b'%s' str(self.status_ppm).encode('ascii'))
