#!/usr/bin/env python3
import sys
import os
import json


def main():
    command = "{\"payload\":{\"sc_cmd\":{\"cf_tgr\":{\"cl_id\":" + sys.argv[2] + ",\"mode\": \"" + sys.argv[
        3] + "\",\"level\": " + sys.argv[4] + ",\"edge\": \"" + sys.argv[5] + "\"}}}}"

    print(command)

    filename = os.path.abspath(sys.argv[1])
    file = open(filename, 'w')
    file.write(command)
    file.close()


def getCommand(id, mode, level, edge):
    command = "{\"payload\":{\"sc_cmd\":{\"cf_tgr\":{\"cl_id\":" + id + ",\"mode\": \"" + mode + "\",\"level\": " + level + ",\"edge\": \"" + edge + "\"}}}}"
    return command


if __name__ == "__main__":

    if ((len(sys.argv) < 5) or (sys.argv[1] == "--help")):
        print(
            """Configures the Trigger. To see all options, check the documentation at: https://bitbucket.org/sourceengineers/iot-scope-doc/src/master/
            Usage: ./cf_tgr INPUT_FILE CHANNEL_ID MODE LEVEL EDGE""")
        sys.exit()

    main()
