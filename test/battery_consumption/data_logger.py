##
# @file data_logger.py
#
# @author Jonathan Camenzuli
#
# @brief Data Logger
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 12/05/2023
#

import serial
from datetime import datetime
import os
import csv

# Set Buad Rate
BAUD_RATE = 9600
# Set filename for saving comma seperated values
FILENAME_CSV = input("Enter the Filename for saving data: ")
# Set COM Port
COM_PORT = input("Enter the Serial Port that you are currently using: ")

# Create the file if it doesn't exist
if not os.path.exists(FILENAME_CSV):
    with open(FILENAME_CSV, 'w'):
        pass

ser = serial.Serial(COM_PORT, BAUD_RATE)

# Open the file in write (truncating) mode
with open(FILENAME_CSV, 'w') as file:
    writer = csv.writer(file)
    try:
        header = ['Time', 'Current (mA)']
        # Continuous loop
        while True:
            # Read a line from the serial port
            line = ser.readline().decode().strip()

            if line == "Current (mA)":
                print(f"Time -> Current (mA)")
                writer.writerow(header)
            else:
                t = datetime.now()
                s = t.strftime('%H:%M:%S.%f')
                print(f"{s[:-3]} -> {line}")
                line = line.replace(' mA', '')
                data = [s[:-3], line]
                writer.writerow(data)

    except KeyboardInterrupt:
        print("Keyboard interrupt detected. Stopping the data logger...")

    except serial.SerialException:
        print("Logger disconnected. Stopping the data logger...")

    finally:
        # Close the serial port and file
        ser.close()
        file.close()
