#!/usr/bin/env python3
import sys
import os
import json
import msgpack


def addState(index, id, isRunning):
    package = "";
    if index != 2:
        package = ","
    package += "\"" + id + "\" : " + isRunning + "";
    return package;

def main():
    command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"cf_running\":{";
    for i in range(2,  len(sys.argv), 2):
        command += addState(i, sys.argv[i], sys.argv[i+1])
    command += "}}}}";
    print(command);
    filename = os.path.abspath(sys.argv[1])
    file = open(filename, 'wb')
    file.write(msgpack.dumps(json.loads(command)))
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
