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
# @section libraries Libraries
# - Pyserial (https://github.com/pyserial/pyserial)
#

import serial
import os

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

# Open the file in append mode
with open(FILENAME_CSV, 'a') as file:
    try:
        # Continuous loop
        while True:
            # Read a line from the serial port
            line = ser.readline().decode().strip()

            # Print the received data
            print(line)

            # Write the data to the file
            file.write(line + '\n')
            file.flush()  # Flush the buffer to ensure data is written immediately

    except KeyboardInterrupt:
        # Handle keyboard interrupt
        print("Keyboard interrupt detected. Stopping the data logger...")

    finally:
        # Close the serial port and file
        ser.close()
        file.close()
