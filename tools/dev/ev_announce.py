#!/usr/bin/env python3
import sys
import os
import json
import msgpack

def main():
    command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"ev_announce\":null}}}";

    print(command);

    filename = os.path.abspath(sys.argv[1])
    file = open(filename, 'w')
    file.write(command)
    file.close()


if __name__ == "__main__":
    
    if ((len(sys.argv) < 2) or (sys.argv[1]=="--help")):
        print(
        """Commands the scope to send its pre defined addresses.
        Usage: ./ev_announce INPUT_FILE """) 
        sys.exit();
    
    main()
