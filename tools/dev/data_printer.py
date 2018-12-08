#!/usr/bin/env python3
import collections
import sys
import json
import matplotlib.pyplot as plt

json_data_1 = '{"transport":"...","payload":{"sc_data":{"cl_data":{"cl_id_0":[1.5,345,274,124.9],"cl_id_1":[2304,9183.1,3492.1,3894.1]},"sc_announce":{"var_name_0":["addr_0","addr_type_0"],"var_name_1":["addr_1","addr_type_1"],"var_name_2":["addr_2","addr_type_2"],"cl_amount":100},"t_stmp":[0,10,20,25],"t_inc":"Increment between values","tgr":{"found":"True/False (always false if Continous)","cl_id":"Channel Id","cl_data_ind":"Index on which value was triggered"}}}}'
json_data_2 = '{"transport":"...","payload":{"sc_data":{"cl_data":{"cl_id_0":[324,12,342,5234],"cl_id_1":[11111,213,36456,123]},"sc_announce":{"var_name_0":["addr_0","addr_type_0"],"var_name_1":["addr_1","addr_type_1"],"var_name_2":["addr_2","addr_type_2"],"cl_amount":100},"t_stmp":[30,40,55,60],"t_inc":"Increment between values","tgr":{"found":"True/False (always false if Continous)","cl_id":"Channel Id","cl_data_ind":"Index on which value was triggered"}}}}'

class DataPrinter:
    
    def __init__(self, length):    
        self.maxlen = length;
        self.data_collection = collections.ChainMap()
        self.timestamp = collections.deque(maxlen=self.maxlen)
        self.f = plt.figure()
        self.ax = self.f.gca()
        self.f.show()

    def plot_data(self, json_data):
        data = json.loads(json_data)

        timestamp_tmp = data["payload"]["sc_data"]["t_stmp"];
        for x in range(0, len(timestamp_tmp)):
            self.timestamp.append(timestamp_tmp[x]);

        channels = data["payload"]["sc_data"]["cl_data"];
        for x in range(0, len(channels)):
            channel_name = list(channels.keys())[x]
            channel_data = channels[channel_name];
            
            if ((channel_name in self.data_collection) == False):
                print("Creating new map entry");
                self.data_collection[channel_name] = collections.deque(maxlen=self.maxlen);
            
            for y in range(0, len(channel_data)):
                self.data_collection[channel_name].append(channel_data[y]);
                
            self.ax.plot(list(self.timestamp), list(self.data_collection[channel_name]), '-')

        self.f.canvas.draw()
