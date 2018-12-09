#!/usr/bin/env python3
import sys
import os
import json
import msgpack

def main():
    command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"cf_t_inc\":" + sys.argv[2] + "}}}";

    print(command);

    filename = os.path.abspath(sys.argv[1])
    file = open(filename, 'wb')
    file.write(msgpack.dumps(json.loads(command)))
    file.close()


if __name__ == "__main__":
    
    if ((len(sys.argv) < 2) or (sys.argv[1]=="--help")):
        print(
        """Configures the Scope to increment the timestep with the given integer. This only has an effect if the Scope is configures with TIMESTAMP_AUTOMATIC
        Usage: ./cf_t_inc INPUT_FILE TIMESTAMP_INCREMENT""") 
        sys.exit();
    
    main()
