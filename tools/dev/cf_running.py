#!/usr/bin/env python3
import sys
import os
import json

def addState(index, id, isRunning, first_element):
    package = "";
    if index != first_element:
        package = ","
    package += "\"" + id + "\" : " + isRunning + "";
    return package;

def getCommand(id, isRunning, amount):
    command = "{\"payload\":{\"sc_cmd\":{\"cf_running\":{";
    for i in range(amount):
        command += addState(i, id[i], isRunning[0], 0)
    command += "}}}}";
    return command;

def main():
    command = "{\"payload\":{\"sc_cmd\":{\"cf_running\":{";
    for i in range(2,  len(sys.argv), 2):
        command += addState(i, sys.argv[i], sys.argv[i+1], 2)
    command += "}}}}";
    print(command);
    filename = os.path.abspath(sys.argv[1])
    file = open(filename, 'w')
    file.write(command)
    file.close()

if __name__ == "__main__":
    
    if ((len(sys.argv) < 3) or (sys.argv[1]=="--help")):
        print(
        """Sets the given channels to either running or stopped. The channels have to be configured before, or the channels will not be started.
        Usage: ./cf_running INPUT_FILE CHANNEL_ID_1 BOOLEAN_1 CHANNEL_ID_N BOOLEAN_N
            true: Channel set to running 
            false: Channel set to stopped""") 
        sys.exit();
    
    main()
