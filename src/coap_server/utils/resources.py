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
import logging
import json


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


class CPS_Resource(BasicResource):
    """Car Park Sensor Resource

    Args:
        BasicResource (_type_): _description_
    """

    def __init__(self):
        super().__init__()

        self.node_id = ""
        self.status_isCarParked = False

    async def render_get(self, request):
        print(f'Is Car Parked?: {self.status_isCarParked}')

        json_obj = {
            "nodetype": "CPS",
            "id": {self.node_id},
            "data":
                {
                    "isCarParked": {self.status_isCarParked}
            }
        }
        payload = json.dumps(json_obj)
        payload = payload.encode('ascii')

        return aiocoap.Message(payload=payload)

    async def render_put(self, request):
        payload = request.payload.decode('ascii')
        print(payload)
        payload_json = json.loads(payload)
        self.node_id = payload_json['id']
        self.status_isCarParked = payload_json['data']['isCarParked']

        logging.info(
            f'⚠️ Payload from {self.node_id}: isCarParked is {self.status_isCarParked}')

        return aiocoap.Message(code=aiocoap.CHANGED, payload=payload.encode('ascii'))
