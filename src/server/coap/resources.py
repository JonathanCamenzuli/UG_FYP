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
    """
    Basic CoAP Resource

    Represents a basic resource that is observable by clients

    Attributes:
        has_observers (bool): Indicates whether there are observers subscribed to this resource.
        notify_observers (bool): Indicates whether the observers should be notified of updates.
        influx_client (Influx): An instance of the Influx client for interacting with InfluxDB.
    """

    def __init__(self):
        super().__init__()

        self.has_observers = False
        self.notify_observers = False
        self.influx_client = Influx(
            influxdb_bucket, influxdb_url, influxdb_token, influxdb_org)

    def notify_observers_check(self):
        """
        Check and notify observers if there are any pending updates.
        """
        while True:
            if self.has_observers and self.notify_observers:
                print('notifying observers')
                self.updated_state()
                self.notify_observers = False

    def update_observation_count(self, count):
        """
        Update the observation count and manage observer state.

        Args:
            count (int): The new observation count.
        """
        if count:
            self.has_observers = True
        else:
            self.has_observers = False


class CPS_Resource(BasicResource):
    """
    Car Park Sensor (CPS) CoAP Resource 

    Represents a resource for a car park sensor. It inherits from the BasicResource class.

    Args:
        `BasicResource (class)`: The parent class for the CPS resource.

    Attributes:
        node_id (str): The identifier of the CPS node.
        status_isCarParked (int): The status indicating whether a car is parked (0 for not parked, 1 for parked).
        influx_sensor (Sensor): An instance of the Sensor class for interacting with InfluxDB.
    """

    def __init__(self):
        super().__init__()

        self.node_id = ""
        self.status_isCarParked = 0
        self.influx_sensor = Sensor("cps", self.influx_client)

    async def render_get(self, request):
        """
        Handle GET requests to retrieve the status of the CPS.

        Args:
            request (aiocoap.Message): The GET request message.

        Returns:
            aiocoap.Message: The response message containing the JSON payload.
        """
        print(f'Is Car Parked?: {self.status_isCarParked}')

        json_obj = {
            "nodetype": "CPS",
            "id": {self.node_id},
            "data":
            {
                "isCarParked": bool({self.status_isCarParked})
            }
        }
        payload = json.dumps(json_obj)
        payload = payload.encode('utf8')

        return aiocoap.Message(payload=payload)

    async def render_put(self, request):
        """
        Handle PUT requests to update the status of the CPS.

        Args:
            request (aiocoap.Message): The PUT request message.

        Returns:
            aiocoap.Message: The response message indicating a successful update.
        """
        payload = request.payload.decode('utf8')
        print(payload)
        payload_json = json.loads(payload)
        self.node_id = payload_json['id']
        self.status_isCarParked = int(
            payload_json['data']['isCarParked'] == True)

        isCarParked_str = ("No vehicle is parked", "Vehicle is parked")[
            bool(self.status_isCarParked)]

        logging.info(f'⚠️  Payload from {self.node_id}: {isCarParked_str}')

        self.influx_sensor.add_value("node_id", self.node_id)
        self.influx_sensor.add_value("isCarParked", self.status_isCarParked)
        self.influx_sensor.write()

        return aiocoap.Message(code=aiocoap.CHANGED, payload=payload.encode('utf8'))


class AQMS_Resource(BasicResource):
    """
    Air Quality Monitoring System (AQMS) CoAP Resource 

    Represents a resource for an park air quality monitoring system. It inherits from the BasicResource class.

    Args:
        `BasicResource (class)`: The parent class for the AQMS resource.

    Attributes:
        node_id (str): The identifier of the AQMS node.
        status_temp_cel (float): The temperature in degrees Celsius.
        status_hum_percent (float): The humidity level in percentage.
        status_co_ppm (float): The carbon monoxide level in parts per million (PPM).
        status_co2_ppm (float): The carbon dioxide level in parts per million (PPM).
        influx_sensor (Sensor): An instance of the Sensor class for interacting with InfluxDB.
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
        """
        Handle GET requests to retrieve the status of the AQMS.

        Args:
            request (aiocoap.Message): The GET request message.

        Returns:
            aiocoap.Message: The response message containing the JSON payload.
        """
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
        payload = payload.encode('utf8')

        return aiocoap.Message(payload=payload)

    async def render_put(self, request):
        """
        Handle PUT requests to update the status of the AQMS.

        Args:
            request (aiocoap.Message): The PUT request message.

        Returns:
            aiocoap.Message: The response message indicating a successful update.
        """
        payload = request.payload.decode('utf8')
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

        return aiocoap.Message(code=aiocoap.CHANGED, payload=payload.encode('utf8'))


class FDS_Resource(BasicResource):
    """
    Fire Detection System (FDS) CoAP Resource

    Represents a resource for a fire detection system. It inherits from the BasicResource class.

    Args:
        BasicResource (class): The parent class for the FDS resource.

    Attributes:
        node_id (str): The identifier of the FDS node.
        status_temp_cel (float): The temperature in degrees Celsius.
        status_hum_percent (float): The humidity level in percentage.
        status_co_ppm (float): The carbon monoxide level in parts per million (PPM).
        status_smoke_level_ppm (float): The smoke level in parts per million (PPM).
        status_isIRDetected (int): The status indicating whether infrared (IR) is detected (0 for not detected, 1 for detected).
        influx_sensor (Sensor): An instance of the Sensor class for interacting with InfluxDB.
    """

    def __init__(self):
        super().__init__()

        self.node_id = ""
        self.status_temp_cel = 0.0
        self.status_hum_percent = 0.0
        self.status_co_ppm = 0.0
        self.status_smoke_level_ppm = 0.0
        self.status_isIRDetected = 0
        self.influx_sensor = Sensor("fds", self.influx_client)

    async def render_get(self, request):
        """
        Handle GET requests to retrieve the status of the FDS.

        Args:
            request (aiocoap.Message): The GET request message.

        Returns:
            aiocoap.Message: The response message containing the JSON payload.
        """
        print('Fire Detection System')
        print(f'Node ID: {self.node_id}')
        print(f'Temperature: {self.status_temp_cel}°C')
        print(f'Humidity: {self.status_hum_percent}%')
        print(f'CO: {self.status_co_ppm} PPM')
        print(f'Smoke: {self.status_smoke_level_ppm} PPM')
        print(f'Is IR Detected?: {self.status_isIRDetected}')

        json_obj = {
            "nodetype": "FDS",
            "id": {self.node_id},
            "data":
            {
                "temperature_c": {self.status_temp_cel},
                "humidity_percent": {self.status_hum_percent},
                "co_level_ppm": {self.status_co_ppm},
                "smoke_level_ppm": {self.status_smoke_level_ppm},
                "isIRDetected": bool({self.status_isIRDetected}),
            }
        }
        payload = json.dumps(json_obj)
        payload = payload.encode('utf8')

        return aiocoap.Message(payload=payload)

    async def render_put(self, request):
        """
        Handle PUT requests to update the status of the AQMS.

        Args:
            request (aiocoap.Message): The PUT request message.

        Returns:
            aiocoap.Message: The response message indicating a successful update.
        """
        payload = request.payload.decode('utf8')
        print(payload)
        payload_json = json.loads(payload)

        self.node_id = payload_json['id']
        self.status_temp_cel = payload_json['data']['temperature_c']
        self.status_hum_percent = payload_json['data']['humidity_percent']
        self.status_co_ppm = payload_json['data']['co_level_ppm']
        self.status_smoke_level_ppm = payload_json['data']['smoke_level_ppm']
        self.status_isIRDetected = int(
            payload_json['data']['isIRDetected'] == True)

        isIRDetected_str = ("IR not detected", "IR detected")[
            bool(self.status_isIRDetected)]

        logging.info(
            f'⚠️  Payload from {self.node_id}: {self.status_temp_cel}°C, {self.status_hum_percent}% Hum, CO: {self.status_co_ppm} PPM, Smoke: {self.status_smoke_level_ppm} PPM, {bool(isIRDetected_str)}')

        self.influx_sensor.add_value("node_id", self.node_id)
        self.influx_sensor.add_value("temperature_c", self.status_temp_cel)
        self.influx_sensor.add_value(
            "humidity_percent", self.status_hum_percent)
        self.influx_sensor.add_value("co_level_ppm", self.status_co_ppm)
        self.influx_sensor.add_value(
            "smoke_level_ppm", self.status_smoke_level_ppm)
        self.influx_sensor.add_value("isIRDetected", self.status_isIRDetected)
        self.influx_sensor.write()

        return aiocoap.Message(code=aiocoap.CHANGED, payload=payload.encode('utf8'))


class Test_Resource(BasicResource):
    """
    Test CoAP Resource

    Represents a test resource. It inherits from the BasicResource class.

    Args:
        BasicResource (class): The parent class for the Test resource.

    Attributes:
        node_id (str): The identifier of the test node.
        status_testValue (float): The test value, which is a pseudorandom number generated from the test physical node
        influx_sensor (Sensor): An instance of the Sensor class for interacting with InfluxDB.
    """

    def __init__(self):
        super().__init__()

        self.node_id = ""
        self.status_testValue = 0.0
        self.influx_sensor = Sensor("test", self.influx_client)

    async def render_get(self, request):
        """
        Handle GET requests to retrieve the test value.

        Args:
            request (aiocoap.Message): The GET request message.

        Returns:
            aiocoap.Message: The response message containing the JSON payload.
        """
        print('Test Endpoint')
        print(f'Node ID: {self.node_id}')
        print(f'Test Value: {self.testValue}')

        json_obj = {
            "nodetype": "TEST",
            "id": {self.node_id},
            "data":
            {
                "testValue": {self.status_testValue}
            }
        }
        payload = json.dumps(json_obj)
        payload = payload.encode('utf8')

        return aiocoap.Message(payload=payload)

    async def render_put(self, request):
        """
        Handle PUT requests to retrieve the test value.

        Args:
            request (aiocoap.Message): The PUT request message.

        Returns:
            aiocoap.Message: The response message indicating a successful update.
        """
        logging.info(f'Payload Received is as follows: {request.payload}')
        payload = request.payload.decode('utf8')
        payload_json = json.loads(payload)
        self.node_id = payload_json['id']
        self.testValue = payload_json['data']['testValue']

        logging.info(f'⚠️  Payload from {self.node_id}: TEST PACKET RECEIVED')

        self.influx_sensor.add_value("node_id", self.node_id)
        self.influx_sensor.add_value("testValue", self.testValue)
        self.influx_sensor.write()

        return aiocoap.Message(code=aiocoap.CHANGED, payload=payload.encode('utf8'))
