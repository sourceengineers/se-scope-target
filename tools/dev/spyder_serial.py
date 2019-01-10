#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Pyserial installation:
# -> Anaconda Navigator
# ->-> Evnironments 
# ->-> Installed zu Not Installed wechseln
# ->-> Pyserial suchen und installieren
import serial
import time
import sys
import json
import cf_addr
import cf_running
import cf_t_inc
import cf_tgr
import ev_announce
import ev_clear
import ev_poll
import ev_trans
import matplotlib.pyplot as plt
import collections
import os

def config():
    global serial_file
    global image_path
    global baudrate
    global timeout
    global figure_name
    global x_width

    ### Serial optionen
    serial_file = '/dev/tty.usbmodem14203' # z.B.: COM1 bei Windows 
    baudrate = 115200
    timeout = 0.5

    # Pfad zum speichern der plots nach dem herunterfahren. 
    # Wenn kein pfad angegeben ist, wird automatisch auf den Desktop gespeichert
#    image_path = None
    image_path = "/Users/schuepbs/Documents"
    
    ### Channel konfiguration
    # typ = UINT8, UINT16, UINT32, FLOAT)
    channels.append({'name' : "Address_32", 'address' : 536969196, 'type' :  "UINT32"})
    channels.append({'name' : "Address_FLOAT", 'address' : 536969192, 'type' :  "FLOAT"})
    channels.append({'name' : "Double Byte Value", 'address' : 536969188, 'type' :  "UINT16"})
    channels.append({'name' : "Byte Value", 'address' : 536969191, 'type' :  "UINT8"})
    
    add_subplot(["Address_FLOAT", "Address_32"], \
             {'title' : 'Float values', 'y_label' : 'Voltage', 'x_label' : 'Time'})
    add_subplot(["Double Byte Value", "Byte Value"], \
             {'title' : 'Integer values', 'y_label' : 'Bytes', 'x_label' : 'Time'})
     
    figure_name = "Device data"

    # wenn die x_width auf None gesetzt wird, wird die Achse automatisch 
    # skaliert
    x_width = 2000
#    x_width = None














##############################################################################
## Don't change any code after this line, or you might cause the code to break!
##############################################################################
    
def send_command(command):
    command = command.encode("utf-8")
    print(command)
    ser.write(command)
    time.sleep(3)
    
##############################################################################
    
def init_periph():
    global ser

    ser = serial.Serial(serial_file, baudrate, timeout=timeout)
    ser.flushInput()
    ser.flushOutput()

    addresses = []
    types = []
    new_state = []
    ids = []

    #channels konfigurieren
    for i in range(len(channels)):
        addresses.append(str(channels[i]['address']))
        types.append(channels[i]['type'])
        ids.append(str(i))
        new_state.append("true")
    
    send_command(cf_addr.getCommand(ids, addresses, types, len(channels)))
    send_command(cf_running.getCommand(ids, new_state, len(channels)))
    
    #Clear data
    send_command(ev_clear.getCommand())

##############################################################################

def add_subplot(new_plot, labels):
    
    subplot_conf = []
    
    for name in new_plot:
        for i in range(len(channels)):
            if channels[i]['name'] == name:
                subplot_conf.append(i)
                
    conf = (subplot_conf,labels)
    plot_conf.append(conf)
    
##############################################################################
    
def process_data(data):
    if len(data) > 2:
        data = data[0:-1]
        try:
            data = data.decode("utf-8")
            data = json.loads(data);
            return data
        except:
            sys.stdout.write("\nCouldn't parse: ");
            print(data)
            return None;
    
##############################################################################

def init_plots():
    global figure
    global ax
    global anim
    global lines
    lines = []
    
    figure, ax = plt.subplots(len(plot_conf), 1)
    figure.suptitle(figure_name)    
    ax = list(ax)

    for i in range(len(ax)):
        ax[i].set_title(plot_conf[i][1]['title'])
        ax[i].set_autoscaley_on(True)
        ax[i].set_xlabel(plot_conf[i][1]['x_label'])
        ax[i].set_ylabel(plot_conf[i][1]['y_label'])
        ax[i].grid()
        for j in range(len(plot_conf[i][0])):
            lines = lines + ax[i].plot([], [])

    for i in range(len(channels) + 1):
       device_data.append(collections.deque(maxlen=x_width))
    
    # Set labels for lines
    for i in range(len(plot_conf)):
        legend_list = []
        for ch in plot_conf[i][0]:
            lines[ch].set_label(channels[ch]['name'])
            legend_list.append(lines[ch])
        ax[i].legend(handles=list(legend_list), loc='upper left')

##############################################################################    
    
def data_is_present(data):
        if ("sc_data" in data["payload"]) == False:
            return False;
        if ("cl_data" in data["payload"]["sc_data"]) == False:
            return False;
        return True;    
    
##############################################################################

def prepare_data(data):
    ans = process_data(data)
    if ans is None:
        return;    
    print(ans)
    
    if not data_is_present(ans):
        return;
    
    # load timestamp data
    device_data[len(channels)].append(ans["payload"]["sc_data"]["t_stmp"][0])
        
    # load channel data
    for i in range(len(channels)):
        if (str(i) in ans["payload"]["sc_data"]["cl_data"]) == True:
            device_data[i].append(ans["payload"]["sc_data"]["cl_data"][str(i)][0])
    
##############################################################################
def clear_data():
    for data in device_data:
        data.clear()
##############################################################################

def read_data():
    answer = ser.read_until(b'\0');
    prepare_data(answer)

##############################################################################
    
def plot_data(): 
    
    for i in range(len(plot_conf)):
        for ch in plot_conf[i][0]:
            if len(device_data[len(channels)]) == len(device_data[ch]):
                lines[ch].set_xdata(list(device_data[len(channels)]))
                lines[ch].set_ydata(list(device_data[ch]))

            # if not the same amount of data is present in the channels,
            # the data gets resetted, until they match
            else:
                device_data()

        ax[i].relim()
        ax[i].autoscale_view()
        if not x_width == None and len(list(device_data[len(channels)])) > 0:
            last_time_stamp = device_data[len(channels)].pop()
            device_data[len(channels)].append(last_time_stamp)
            ax[i].set_xlim(last_time_stamp - x_width, last_time_stamp)

    plt.draw()
    plt.pause(1e-17)

##############################################################################
    
def main():    
    
    ser.flushInput()
    ser.flushOutput()

    while True:
        read_data()
        plot_data()
        time.sleep(0.25)

##############################################################################
def cleanup():
    try:
        save_path  = os.path.abspath(image_path)
    except:
        save_path = os.path.expanduser("~/Desktop")

    save_path = os.path.join(save_path, figure_name) 
    figure.savefig(save_path)
    ser.close()

    print("Safed plot to: " + save_path)

##############################################################################

if __name__ == "__main__":
    channels = []
    plot_conf = []
    device_data = []

    config()
    init_periph()
    init_plots()
   
    try:
        main()
    except KeyboardInterrupt:
        print('Script interrupted\nCleaning up...')
        cleanup()
