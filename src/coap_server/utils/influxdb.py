##
# @file influxdb.py
#
# @author Jonathan Camenzuli
#
# @brief InfluxDB Functionality Classes
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 19/01/2023
#
# @section libraries Libraries
# - influxdb-client-python by @influxdata (https://github.com/influxdata/influxdb-client-python)
# - python-dotenv by @theskumar (https://github.com/theskumar/python-dotenv)
#

from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
from dotenv import load_dotenv
import os

load_dotenv()

influxdb_bucket = os.getenv('INFLUXDB_BUCKET')
influxdb_url = os.getenv('INFLUXDB_URL')
influxdb_token = os.getenv('INFLUXDB_TOKEN')
influxdb_org = os.getenv('INFLUXDB_ORG')


class Influx:
    def __init__(self, bucket, url, token, org):
        self.client = InfluxDBClient(url=url, token=token, org=org)
        self.write_api = self.client.write_api(write_options=SYNCHRONOUS)
        self.bucket = bucket

    def write(self, point):
        self.write_api.write(bucket=self.bucket, record=point)


class Sensor:
    def __init__(self, name, influx_client):
        self.name = name
        self.client = influx_client
        self.point = Point(name)

    def add_value(self, name, value):
        self.point.field(name, value)

    def write(self):
        self.influx.write(self.point)
