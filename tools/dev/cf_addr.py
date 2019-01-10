#!/usr/bin/env python3
import sys
import os
import json

def addAddr(index, id, addr, type, first_element):
    package = "";
    if index != first_element:
        package = ","
    package += "\"" + id + "\":[" + addr +", \"" + type + "\"]"
    return package;

def getCommand(id, addr, type, amount):
    command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"cf_addr\":{";
    
    for i in range(amount):
        command += addAddr(i, id[i], addr[i], type[i], 0)
    command += "}}}}";
    return command;


def main():
    command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"cf_addr\":{";

    for i in range(2,  len(sys.argv), 3):
        command += addAddr(i, sys.argv[i], sys.argv[i+1], sys.argv[i+2], 2)

    command += "}}}}";

    print(command);

    filename = os.path.abspath(sys.argv[1])
    file = open(filename, 'w')
    file.write(command)
    file.close()


if __name__ == "__main__":
    
    if ((len(sys.argv) < 4) or (sys.argv[1]=="--help")):
        print(
        """Configures the choosen channels to track the gives address
        Usage: ./cf_addr INPUT_FILE CHANNEL_ID_1 ADDRESS_1 DATA_TYPE_1 CHANNEL_ID_N ADDRESS_N DATA_TYPE_N
        DATA_TYPE may be: UINT8, UINT16, UINT32, UINT64, FLOAT, DUBLE""") 
        sys.exit();
    
    main()
