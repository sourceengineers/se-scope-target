#!/usr/bin/env python3
import serial
import time
import os
import sys
import msgpack
from data_printer import DataPrinter
import json
import collections
from six import string_types

printer = DataPrinter(100);

# Function needed to convert the dict, containing byte strings, to normal strings
def convert(data):
    try:
        return data.decode()
    except AttributeError:
        pass

    if isinstance(data, collections.Mapping):
        return dict(map(convert, data.items()))
    elif isinstance(data, collections.Iterable):
        return type(data)(map(convert, data))
    else:
        return data

def printAndParse(ans):
    if len(ans) > 0:
        ans = ans[0:-5]
        sys.stdout.write("\nAnswer: ")

        parsed = msgpack.unpackb(ans)
        converted = convert(parsed);
        json_data = json.dumps(converted);
        print(json_data)
        printer.plot_data(json_data);

def main():
    serialFile = os.path.abspath(sys.argv[1])
    inputFile = os.path.abspath(sys.argv[2])
    outputFile = os.path.abspath(sys.argv[3])

    ser = serial.Serial(serialFile, 115200, timeout=1)

    while True:
        with open(inputFile, "rb") as f:
            if os.path.getsize(inputFile) > 0:
                sys.stdout.write("\nCommand: ")
                command = f.readline()
                print(msgpack.unpackb(command))
                ser.write(command)

        open(inputFile, "wb")
        time.sleep(0.01)

        answer = ser.read_until(b'\0\0\0\0\0')
        printAndParse(answer)
        with open(outputFile, "r+b") as o:
            answer = ser.read_until(b'\0\0\0\0\0')
            o.write(answer)
            printAndParse(answer)

        ser.flushInput()
        ser.flushOutput()

if __name__ == "__main__":
    
    if ((len(sys.argv) < 3) or (sys.argv[1]=="--help")):
        print(
        """Opens a connection to a Serial device and transmits the commands in "INPUT_FILE" to the device.
            The received data will be written to the "OUTPUT_FILE". 
            If the received data is a cl_data package, the data will be plotted.
        Usage: ./uart_msgpack_tester.py SERIAL_FILE INPUT_FILE OUTPUT_FILE""") 
        sys.exit();
    
    main()
