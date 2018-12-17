#!/usr/bin/env python3
from collections import ChainMap, deque
import sys
import json
import matplotlib.pyplot as plt

class DataPrinter:

    def __init__(self, length):
        self.maxlen = length;
        self.data_collection = ChainMap()
        self.timestamp = deque(maxlen=self.maxlen)
        self.f = plt.figure()
        self.ax = self.f.gca()
        self.f.show()
        plt.ion()
        plt.show()
        self.tgr_x = 0;
        self.tgr_y = 0;

    def plot_data(self, json_data, name_maping):
        plt.cla()

        data = json.loads(json_data)

        if ("sc_data" in data["payload"]) == False:
            return;
        if ("cl_data" in data["payload"]["sc_data"])== False:
            return;

        timestamp_tmp = data["payload"]["sc_data"]["t_stmp"];
        for x in range(0, len(timestamp_tmp)):
            self.timestamp.append(timestamp_tmp[x]);

        channels = data["payload"]["sc_data"]["cl_data"];
        for x in range(0, len(channels)):
            channel_name = list(channels.keys())[x]
            channel_data = channels[channel_name];

            if ((channel_name in self.data_collection) == False):
                print("Creating new map entry");
                self.data_collection[channel_name] = deque(maxlen=self.maxlen);

            for y in range(0, len(channel_data)):
                self.data_collection[channel_name].append(channel_data[y]);

            if (len(list(self.timestamp)) == len(list(self.data_collection[channel_name]))):
                label = name_maping[channel_name]; 
                self.ax.plot(list(self.timestamp), list(self.data_collection[channel_name]), '-', label = label)				
        if(data["payload"]["sc_data"]["tgr"]["found"] == True):
            self.tgr_x = data["payload"]["sc_data"]["tgr"]["cl_data_ind"]
            index_y = timestamp_tmp.index(self.tgr_x)
            tgr_channel = data["payload"]["sc_data"]["tgr"]["cl_id"]
            self.tgr_y = data["payload"]["sc_data"]["cl_data"][tgr_channel][index_y]
        
        if(self.tgr_y != 0 and self.tgr_x != 0):
            self.ax.plot(self.tgr_x, self.tgr_y, 'rx')
    
        self.ax.grid(linestyle='-', linewidth=1)
        self.f.canvas.draw()
        handles, labels = self.ax.get_legend_handles_labels()
        self.ax.legend(handles, labels, loc='upper right')
        plt.pause(0.001)
