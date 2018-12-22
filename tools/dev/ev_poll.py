#!/usr/bin/env python3
import sys
import os
import json

def main():
    command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"ev_poll\":" + sys.argv[2] + "}}}";

    print(command);

    filename = os.path.abspath(sys.argv[1])
    file = open(filename, 'w')
    file.write(command)
    file.close()

if __name__ == "__main__":
    
    if ((len(sys.argv) < 3) or (sys.argv[1]=="--help")):
        print(
        """Commands the scope to poll values from its configured channels. The timestamp will only be used, if the scope is configured as TIMESTAMP_AUTOMATIC.
        Usage: ./ev_poll INPUT_FILE TIMESTAMP""") 
        sys.exit();
    
    main()
