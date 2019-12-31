from se_scope_pb2 import *
from google.protobuf.json_format import MessageToJson
from google.protobuf import *

def pack_data_vector():
    timestamp = [0, 1, 2, 3, 4, 5]
    channel_data_0 = [-23.5354, 234534523.5354, 0.00000345, -0.00000345, 0.0, 1.5]
    channel_0 = SC_Channel(data=channel_data_0, id=0)
    channel_1 = SC_Channel(data=channel_data_0, id=1)
    trigger = SC_Trigger(cl_id=1, cl_data_ind=100, mode=ONE_SHOT)
    data = SC_Data(timestamps=timestamp, trigger=trigger, channels=[channel_0, channel_1], t_inc=10)
    return data
    

def pack_t_inc_vector():
    tinc = CF_TInc(t_inc=10)
    return tinc

def pack_trigger_vector():
    trigger = CF_Trigger(mode=0, level=1.5, cl_id = 2, edge=False)
    return trigger

def pack_poll_vector():
    poll = EV_Poll(timestamp=100)
    return poll

def pack_address_vector():
    address_1 = AddressConfig(cl_id=2, address=13423, type=0)
    address_2 = AddressConfig(cl_id=1, address=5000, type=1)
    address_3 = AddressConfig(cl_id=0, address=0000, type=2)
    address = CF_Address(addresses=[address_1, address_2, address_3])
    return address


def pack_running_vector():
    running_1 = RunningConfig(cl_id=1, new_state=False)
    running_2 = RunningConfig(cl_id=0, new_state=True)
    running_3 = RunningConfig(cl_id=2, new_state=True)
    running = CF_Running(running=[running_1, running_2, running_3])
    return running

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
    print_test_case(pack_t_inc_vector(), "t_inc")
    print_test_case(pack_trigger_vector(), "trigger")
    print_test_case(pack_poll_vector(), "poll")
    print_test_case(pack_address_vector(), "address")
    print_test_case(pack_running_vector(), "running")


if __name__== "__main__":
  main()