import sys
import os
import json
import msgpack


command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"ev_poll\":" + sys.argv[2] + "}}}";

print(command);

filename = os.path.abspath(sys.argv[1])
file = open(filename, 'wb')
file.write(msgpack.dumps(json.loads(command)))
file.close()
