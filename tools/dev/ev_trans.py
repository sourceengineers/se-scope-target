#!/usr/bin/env python3
import sys
import os
import json
import msgpack

def main():
    command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"ev_trans\":null}}}";

    print(command);

    filename = os.path.abspath(sys.argv[1])
    file = open(filename, 'w')
    file.write(command)
    file.close()
        

if __name__ == "__main__":
    
    if ((len(sys.argv) < 2) or (sys.argv[1]=="--help")):
        print(
        """Commands the scope to transmit its polled data. This will only happen, if a proper transmit_function is passed to the scope.
        Usage: ./ev_trans INPUT_FILE """) 
        sys.exit();
    
    main()
