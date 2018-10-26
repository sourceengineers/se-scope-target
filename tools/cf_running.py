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

command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"cf_running\":{";

for i in range(2,  len(sys.argv), 2):
    command += addState(i, sys.argv[i], sys.argv[i+1])

command += "}}}}";

print(command);

filename = os.path.abspath(sys.argv[1])
file = open(filename, 'wb')
file.write(msgpack.dumps(json.loads(command)))
file.close()
