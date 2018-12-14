#!/usr/bin/env python3
import serial
import time
import os
import sys
import msgpack
from data_printer import DataPrinter
import json
from collections import Mapping, Iterable
from six import string_types
import yaml


printer = DataPrinter(100);

# Function needed to convert the dict, containing byte strings, to normal strings
def convert(data):
    try:
        return data.decode()
    except AttributeError:
        pass

    if isinstance(data, Mapping):
        return dict(map(convert, data.items()))
    elif isinstance(data, Iterable):
        return type(data)(map(convert, data))
    else:
        return data

def printAndParse(ans):
    if len(ans) > 0:
        ans = ans[0:-9];

        try:
            parsed = msgpack.unpackb(ans);
        except:
            sys.stdout.write("\nCouldn't parse: ");
            print(ans);
            return;

        converted = convert(parsed);
        sys.stdout.write("\nAnswer: ");
        json_data = json.dumps(converted);
        print(json_data);
        printer.plot_data(json_data);

        with open(outputFile, "a+") as o:
            if isinstance(parsed, str):
                o.write(parsed)

        return json_data;

def main():


    with open(conf_file, 'r') as f:
            yaml_parser = yaml.load(f)
            baudrate = yaml_parser["Serial"]["Baudrate"];
            timeout = yaml_parser["Serial"]["Timeout"];
            serial_file = yaml_parser["Serial"]["File"];
            legend = yaml_parser["Legend"];

    ser = serial.Serial(serial_file, baudrate, timeout=timeout)
    ser.flushInput()
    ser.flushOutput()

    while True:
        with open(inputFile, "rb") as f:
            if os.path.getsize(inputFile) > 0:
                sys.stdout.write("\nCommand: ")
                command = f.readline()
                print(msgpack.unpackb(command))
                ser.write(command)

        open(inputFile, "wb")

        time.sleep(0.1)
        
        with open(conf_file, 'r') as f:
                yaml_parser = yaml.load(f)
                legend = yaml_parser["Legend"];
                
        answer = ser.read_until(b"\0\0\0\0\0STOP")
        printAndParse(answer, legend)

    #    ser.flushInput()
    #    ser.flushOutput()


if __name__ == "__main__":

    if ((len(sys.argv) < 3) or (sys.argv[1]=="--help")):
        print(
        """Opens a connection to a Serial device and transmits the commands in "INPUT_FILE" to the device.
            The received data will be written to the "OUTPUT_FILE".
            If the received data is a cl_data package, the data will be plotted.
        Usage: ./uart_msgpack_tester.py SERIAL_FILE INPUT_FILE OUTPUT_FILE""")
        sys.exit();

    conf_file = os.path.abspath(sys.argv[1])
    inputFile = os.path.abspath(sys.argv[2])
    outputFile = os.path.abspath(sys.argv[3])
    main()
