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
# - aiocoap by Christian Amsüss (https://github.com/chrysn/aiocoap)
# - python-dotenv by Saurabh Kumar (https://github.com/theskumar/python-dotenv)
# - InfluxDB Functionality Classes (./influxdb.py)
#

import aiocoap.resource as resource
import aiocoap
import logging
import json
from dotenv import load_dotenv
import os
from influxdb import Influx
from influxdb import Sensor

load_dotenv()

influxdb_bucket = os.getenv('INFLUXDB_BUCKET')
influxdb_url = os.getenv('INFLUXDB_URL')
influxdb_token = os.getenv('INFLUXDB_TOKEN')
influxdb_org = os.getenv('INFLUXDB_ORG')


class BasicResource(resource.ObservableResource):
    """A basic resource which supports GET and PUT methods.
    """

    def __init__(self):
        super().__init__()

        self.has_observers = False
        self.notify_observers = False
        self.influx_client = Influx(
            influxdb_bucket, influxdb_url, influxdb_token, influxdb_org)

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
        self.influx_sensor = Sensor("cps", self.influx_client)

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

        self.influx_sensor.add_value("node_id", self.node_id)
        self.influx_sensor.add_value("isCarParked", self.status_isCarParked)
        self.influx_sensor.write()

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
        self.influx_sensor = Sensor("aqms", self.influx_client)

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

        self.influx_sensor.add_value("node_id", self.node_id)
        self.influx_sensor.add_value("temperature_c", self.status_temp_cel)
        self.influx_sensor.add_value(
            "humidity_percent", self.status_hum_percent)
        self.influx_sensor.add_value("co_level_ppm", self.status_co_ppm)
        self.influx_sensor.add_value("co2_level_ppm", self.status_co2_ppm)
        self.influx_sensor.write()

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
        self.influx_sensor = Sensor("fds", self.influx_client)

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

        self.influx_sensor.add_value("node_id", self.node_id)
        self.influx_sensor.add_value("temperature_c", self.status_temp_cel)
        self.influx_sensor.add_value("isIRDetected", self.status_isIRDetected)
        self.influx_sensor.add_value(
            "isSmokeDetected", self.status_isSmokeDetected)
        self.influx_sensor.write()

        return aiocoap.Message(code=aiocoap.CHANGED, payload=payload.encode('ascii'))


class Test_Resource(BasicResource):
    """Test Resource

    Args:
        BasicResource (_type_): _description_
    """

    def __init__(self):
        super().__init__()

        self.node_id = ""
        self.status_testValue = 0.0
        self.influx_sensor = Sensor("test", self.influx_client)

    async def render_get(self, request):
        print('Test Endpoint')
        print(f'Node ID: {self.node_id}')
        print(f'Test Value: {self.testValue}')

        json_obj = {
            "nodetype": "TEST",
            "id": {self.node_id},
            "data":
            {
                "testValue": {self.status_isCarParked}
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
        self.testValue = payload_json['data']['testValue']

        logging.info(f'⚠️  Payload from {self.node_id}: TEST PACKET RECEIVED')

        self.influx_sensor.add_value("node_id", self.node_id)
        self.influx_sensor.add_value("testValue", self.testValue)
        self.influx_sensor.write()

        return aiocoap.Message(code=aiocoap.CHANGED, payload=payload.encode('ascii'))
