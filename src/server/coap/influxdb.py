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

from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import ASYNCHRONOUS


class Influx:
    """
    InfluxDB Client Wrapper

    A wrapper class for interacting with InfluxDB. It provides methods for writing data points to the specified InfluxDB bucket.

    Args:
        bucket (str): The name of the InfluxDB bucket.
        url (str): The URL of the InfluxDB instance.
        token (str): The authorization token for accessing InfluxDB.
        org (str): The organization name associated with the InfluxDB instance.

    Attributes:
        client (InfluxDBClient): The InfluxDB client instance.
        write_api (WriteApi): The write API for writing data points asynchronously.
        bucket (str): The name of the InfluxDB bucket.
    """

    def __init__(self, bucket, url, token, org):
        self.client = InfluxDBClient(url=url, token=token, org=org)
        self.write_api = self.client.write_api(write_options=ASYNCHRONOUS)
        self.bucket = bucket

    def write(self, point):
        """
        Write a data point to the InfluxDB bucket.

        Args:
            point (Point): The data point to be written to InfluxDB.
        """
        self.write_api.write(bucket=self.bucket, record=point)


class Sensor:
    """
    InfluxDB Sensor

    Represents a sensor for writing data points to InfluxDB. It is associated with a specific measurement name and uses an InfluxDB client for writing data.

    Args:
        name (str): The name of the sensor.
        influx_client (Influx): An instance of the Influx class for writing data to InfluxDB.

    Attributes:
        name (str): The name of the sensor.
        client (Influx): The InfluxDB client used for writing data.
        point (Point): The data point associated with the sensor.
    """

    def __init__(self, name, influx_client):
        self.name = name
        self.client = influx_client
        self.point = Point(name)

    def add_value(self, name, value):
        """
        Add a field value to the sensor's data point.

        Args:
            name (str): The name of the field.
            value: The value to be added to the field.
        """
        self.point.field(name, value)

    def write(self):
        """
        Write the sensor's data point to InfluxDB.
        """
        self.client.write(self.point)
