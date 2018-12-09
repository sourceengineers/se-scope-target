#!/usr/bin/env python3
import collections
import sys
import json
import matplotlib.pyplot as plt

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

        if ("cl_data" in data["payload"]["sc_data"]) == False:
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
                self.data_collection[channel_name] = collections.deque(maxlen=self.maxlen);

            for y in range(0, len(channel_data)):
                self.data_collection[channel_name].append(channel_data[y]);

            if (len(list(self.timestamp)) == len(list(self.data_collection[channel_name]))):
                self.ax.plot(list(self.timestamp), list(self.data_collection[channel_name]), '-')

        self.f.canvas.draw()
