##
# @file resource_monitor.py
#
# @author Jonathan Camenzuli
#
# @brief Resource Monitor
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 11/06/2023
#
# @section libraries Libraries
# - psutil (https://github.com/giampaolo/psutil)
#

import psutil
import csv
import time

# Set filename for saving comma seperated values
FILENAME_CSV = 'resource_data.csv'
# Set the interval (in seconds) between resource measurements
TIME_INTERVAL_SECONDS = 1
# Set the total duration (in seconds) for resource measurement
TEST_DURATION_SECONDS = 1200

# Open a CSV file to store resource data
with open(FILENAME_CSV, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['Timestamp', 'CPU (%)', 'Memory (%)',
                    'Disk (%)', 'Temperature (C)'])

    # Start the resource measurement loop
    start_time = time.time()
    while time.time() - start_time <= TEST_DURATION_SECONDS:
        # Get the current timestamp
        timestamp = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())

        # Get CPU, memory, disk utilization, and temperature
        cpu_percent = psutil.cpu_percent(interval=TIME_INTERVAL_SECONDS)
        mem_percent = psutil.virtual_memory().percent
        disk_percent = psutil.disk_usage('/').percent
        # Adjust the sensor name if needed
        temperature = psutil.sensors_temperatures()['cpu_thermal'][0].current

        # Write the data to the CSV file
        writer.writerow([timestamp, cpu_percent, mem_percent,
                        disk_percent, temperature])

        # Wait for the specified interval
        time.sleep(TIME_INTERVAL_SECONDS)
