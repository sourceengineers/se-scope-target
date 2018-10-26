import sys
import os
import json
import msgpack


command = "{\"transport\":null,\"payload\":{\"sc_cmd\":{\"cf_tgr\":{\"cl_id\":" + sys.argv[2] + ",\"mode\": \"" + sys.argv[3] + "\",\"level\": " + sys.argv[4] + ",\"edge\": \"" + sys.argv[5] + "\"}}}}";

print("FILE, cl_id, mode (NORMAL, CONTINOUS), level, edge (FALLING/RISING)")
print(command);

filename = os.path.abspath(sys.argv[1])
file = open(filename, 'wb')
file.write(msgpack.dumps(json.loads(command)))
file.close()
