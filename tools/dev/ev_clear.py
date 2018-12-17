#!/usr/bin/env python3
import sys
import os
import json
import msgpack

def main():
    command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"ev_clear\":null}}}";

    print(command);

    filename = os.path.abspath(sys.argv[1])
    file = open(filename, 'wb')
    file.write(msgpack.dumps(json.loads(command)))
    file.close()
        

if __name__ == "__main__":
    
    if ((len(sys.argv) < 2) or (sys.argv[1]=="--help")):
        print(
        """Clears all the data in the channels and resets the timestamp data.
        Usage: ./ev_clear INPUT_FILE """) 
        sys.exit();
    
    main()
