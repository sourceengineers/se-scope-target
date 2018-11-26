import sys
import os
import json
import msgpack


def addAddr(index, id, addr, type):
    package = "";
    if index != 2:
        package = ","
    package += "\"" + id + "\":[" + addr +", \"" + type + "\"]"
    return package;


command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"cf_addr\":{";

for i in range(2,  len(sys.argv), 3):
    command += addAddr(i, sys.argv[i], sys.argv[i+1], sys.argv[i+2])

command += "}}}}";

print(command);

filename = os.path.abspath(sys.argv[1])
file = open(filename, 'wb')
file.write(msgpack.dumps(json.loads(command)))
file.close()
