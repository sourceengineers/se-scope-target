from se_scope_pb2 import *
from google.protobuf.json_format import MessageToJson
from google.protobuf import *

def pack_data_vector():
    timestamp = [0, 1, 2, 3, 4, 5]
    channel_data_0 = [-23.5354, 234534523.5354, 0.00000345, -0.00000345, 0.0, 1.5]
    channel_0 = Channel(data=channel_data_0, id=0)
    channel_1 = Channel(data=channel_data_0, id=1)
    trigger = Trigger(cl_id=1, cl_data_ind=100, mode=ONE_SHOT)
    data = Data(timestamps=timestamp, trigger=trigger, channels=[channel_0, channel_1], t_inc=10)
    return data
    

def to_binary(m):
    b = [x for x in list(m)]
    return b

def print_test_case(m, name):
    print("Test case {}:".format(name))
    print("Binary: ")
    print(to_binary(m.SerializeToString()))
    print("Comparison in json: ")
    print(MessageToJson(m))
    print("\n\n")


def main():
    print_test_case(pack_data_vector(), "data")


if __name__== "__main__":
  main()