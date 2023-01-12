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

        isCarParked_str = ("No vehicle is parked", "Vehicle is parked")[
            self.status_isCarParked]

        logging.info(
            f'⚠️  Payload from {self.node_id}: {isCarParked_str}')

        return aiocoap.Message(code=aiocoap.CHANGED, payload=payload.encode('ascii'))


class AQMS_Resource(BasicResource):
    """Air Quality Monitoring System Resource

    Args:
        BasicResource (_type_): _description_
    """

    def __init__(self):
        super().__init__()

        self.node_id = ""
        self.status_temp_cel = 0.0
        self.status_hum_percent = 0.0
        self.status_co_ppm = 0.0
        self.status_co2_ppm = 0.0

    async def render_get(self, request):
        print('Air Quality Monitoring System')
        print(f'Node ID: {self.node_id}')
        print(f'Temperature: {self.status_temp_cel}°C')
        print(f'Humidity: {self.status_hum_percent}%')
        print(f'CO: {self.status_co_ppm} PPM')
        print(f'CO2: {self.status_co2_ppm} PPM')

        json_obj = {
            "nodetype": "AQMS",
            "id": {self.node_id},
            "data":
            {
                "temperature_c": {self.status_temp_cel},
                "humidity_percent": {self.status_hum_percent},
                "co_level_ppm": {self.status_co_ppm},
                "co2_level_ppm": {self.status_co2_ppm}
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
        self.status_temp_cel = payload_json['data']['temperature_c']
        self.status_hum_percent = payload_json['data']['humidity_percent']
        self.status_co_ppm = payload_json['data']['co_level_ppm']
        self.status_co2_ppm = payload_json['data']['co2_level_ppm']

        logging.info(
            f'⚠️  Payload from {self.node_id}: {self.status_temp_cel}°C, {self.status_hum_percent}% Hum, CO: {self.status_co_ppm} PPM, CO2: {self.status_co2_ppm} PPM')

        return aiocoap.Message(code=aiocoap.CHANGED, payload=payload.encode('ascii'))


class FDS_Resource(BasicResource):
    """Air Quality Monitoring System Resource

    Args:
        BasicResource (_type_): _description_
    """

    def __init__(self):
        super().__init__()

        self.node_id = ""
        self.status_temp_cel = 0.0
        self.status_isIRDetected = False
        self.status_isSmokeDetected = False

    async def render_get(self, request):
        print('Air Quality Monitoring System')
        print(f'Node ID: {self.node_id}')
        print(f'Temperature: {self.status_temp_cel}°C')
        print(f'Is IR Detected?: {self.status_isIRDetected}')
        print(f'Is Smoke Detected?: {self.status_isSmokeDetected}')

        json_obj = {
            "nodetype": "FDS",
            "id": {self.node_id},
            "data":
            {
                "temperature_c": {self.status_temp_cel},
                "isIRDetected": {self.status_isIRDetected},
                "isSmokeDetected": {self.status_isSmokeDetected}
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
        self.status_temp_cel = payload_json['data']['temperature_c']
        self.status_isIRDetected = payload_json['data']['isIRDetected']
        self.status_isSmokeDetected = payload_json['data']['isSmokeDetected']

        isIRDetected_str = ("IR not detected", "IR detected")[
            self.status_isIRDetected]
        isSmokeDetected_str = ("Smoke not detected", "Smoke detected")[
            self.status_isSmokeDetected]

        logging.info(
            f'⚠️  Payload from {self.node_id}: {self.status_temp_cel}°C, {isIRDetected_str}, {isSmokeDetected_str}')

        return aiocoap.Message(code=aiocoap.CHANGED, payload=payload.encode('ascii'))
