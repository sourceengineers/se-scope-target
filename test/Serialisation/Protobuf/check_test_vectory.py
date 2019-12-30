from se_scope_pb2 import *
from google.protobuf.json_format import MessageToJson
import sys

def parse_data(d):
    data = Data()
    data.ParseFromString(d)
    return MessageToJson(data, including_default_value_fields=False)

def main(c, d):

    m = [int(i, 10) for i in d]
    b = bytes(m)
    if c == "data":
        p = parse_data(b)

    print(p)

if __name__ == "__main__":

    case = sys.argv[1]
    data = sys.argv[2].split(',')

    main(case, data)