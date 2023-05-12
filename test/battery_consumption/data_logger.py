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

baud_rate = 9600
com_port = input("Enter the Serial Port that you are currently using: ")
output = input("Enter filename of where you want to store data")

ser = serial.Serial(com_port, baud_rate)

# Open the file in append mode
with open(output, 'a') as file:
    # Continuous loop
    while True:
        # Read a line from the serial port
        line = ser.readline().decode().strip()

        # Print the received data
        print(line)

        # Write the data to the file
        file.write(line + '\n')
        file.flush()  # Flush the buffer to ensure data is written immediately
