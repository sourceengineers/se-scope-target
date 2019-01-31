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
    global map_file
    global trigger_conf
    ##########################################################################
    ### Serial optionen
    ##########################################################################
    serial_file = '/dev/tty.usbmodem14103' # z.B.: COM1 bei Windows 
    baudrate = 57600
    timeout = 2 
    
    ##########################################################################
    ### Pfade
    ##########################################################################
    # Pfad zum speichern der plots nach dem herunterfahren. 
    # Wenn kein pfad angegeben ist, wird automatisch auf den Desktop gespeichert
    image_path = None
    #image_path = "/Users/USER/Documents"
   
    # Pfad zum map file
    map_file = "/Users/schuepbs/Documents/Projects/nucleo/cmake-build-debug/nucleo.map"

    ##########################################################################
    ### Channel Konfiguration
    ##########################################################################
    # typ = UINT8, UINT16, UINT32, FLOAT)
    # Die Addressen koennen entweder aus der map File ausgelesen werden, falls sie
    # als statisch deklariert sind, oder manuell eigetragen werden.
    channels.append({'name' : "Address_32", 'address' : get_address_from_map("intVar"), 'type' :  "UINT32"})
    channels.append({'name' : "Address_FLOAT", 'address' : get_address_from_map("floatVar"), 'type' :  "FLOAT"})
    channels.append({'name' : "Double Byte Value", 'address' : 536969154, 'type' :  "UINT16"})
    channels.append({'name' : "Byte Value", 'address' : get_address_from_map("byteVar"), 'type' :  "UINT8"})
 
    ##########################################################################
    ### Trigger Konfiguration
    ##########################################################################   
    #trigger_conf = {'mode' : 'Continous', 'level' : 1.4, 'edge' : 'rising', 'cl_id' : 1} 
    trigger_conf = {'mode' : 'Normal', 'level' : 40000, 'edge' : 'rising', 'cl_id' : 1} 

    ##########################################################################
    ### Grafik Konfiguration
    ##########################################################################   
    add_subplot(["Address_FLOAT", "Address_32"], \
             {'title' : 'Float values', 'y_label' : 'Voltage', 'x_label' : 'Time'})
    add_subplot(["Double Byte Value", "Byte Value"], \
             {'title' : 'Integer values', 'y_label' : 'Bytes', 'x_label' : 'Time'})
     
    figure_name = "Device data"

    # wenn die x_width auf None gesetzt wird, wird die Achse automatisch 
    # skaliert
    x_width = 200
    #x_width = None

## Don't change any code after this line, or you might cause the code to break!
##############################################################################
    
def send_command(command, wait_for_ack):
    
    command = command.encode("utf-8")

    while True:
        print(command)
        ser.write(command)
        time.sleep(3)
        if wait_for_ack == False:
            break;
        if found_flow_ctrl() == "ACK" :
            break;
        print("NAK was received. Trying to send again")
   
##############################################################################

def found_flow_ctrl(): 
    while True:
        answer = ser.read_until(b'\0');
        data = process_data(answer)
        if not data == None:
            if ("flow_ctrl" in data["payload"]) == True:
                print(data)
                if(data["payload"]["flow_ctrl"] == "ACK"):
                    return "ACK";
                if(data["payload"]["flow_ctrl"] == "NAK"):
                    return "NAK";

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
    
    send_command(cf_tgr.getCommand(str(trigger_conf['cl_id']), trigger_conf['mode'], str(trigger_conf['level']), str(trigger_conf['edge'])), True);
    send_command(cf_addr.getCommand(ids, addresses, types, len(channels)), True)
    send_command(cf_running.getCommand(ids, new_state, len(channels)), True)

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
            #print(data)    # Uncomment this print if you want to see the parsed output of the scope
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

def channel_data_is_present(index, ans):
    if (str(index) in ans["payload"]["sc_data"]["cl_data"]) == False:
        return False

    if len(ans["payload"]["sc_data"]["cl_data"][str(index)]) <= 0:
        return False

    return True

##############################################################################

def append_to_channel(channel, data):
    for d in data:
        channel.append(d)
    return channel;

##############################################################################

def prepare_data(data):
    ans = process_data(data)
    if ans is None:
        return;    
    
    if not data_is_present(ans):
        return;
    
    trigger_data = get_trigger_data(ans)

    if not trigger_data['found'] == True:
        clear_data();
        
    # load tmestamp data
    device_data[len(channels)] = append_to_channel(device_data[len(channels)], ans["payload"]["sc_data"]["t_stmp"])
    

    # load channel data
    for i in range(len(channels)):
        if channel_data_is_present(i, ans) == True:
            device_data[i] = append_to_channel(device_data[i], ans["payload"]["sc_data"]["cl_data"][str(i)])

##############################################################################
def clear_data():
    for data in device_data:
        data.clear()
##############################################################################

def read_data():
    answer = ser.read_until(b'\0');
    prepare_data(answer)

##############################################################################

def get_trigger_data(data):
    trigger_data['found'] = data["payload"]["sc_data"]["tgr"]["found"]
    if(trigger_data['found'] == True):
        trigger_data['cl_data_ind'] = data["payload"]["sc_data"]["tgr"]["cl_data_ind"]
        trigger_data['cl_id'] = data["payload"]["sc_data"]["tgr"]["cl_id"]
    else:
        trigger_data['cl_data_ind'] = 0
        trigger_data['cl_id'] = 0 
    return trigger_data;

##############################################################################

def get_trigger_point():
    index = list(device_data[len(channels)]).index(trigger_data['cl_data_ind'])
    
    return {'x' : trigger_data['cl_data_ind'], 'y' : list(device_data[trigger_data['cl_id']])[index]}

##############################################################################
def plot_data(): 
    
    for i in range(len(plot_conf)):
        for ch in plot_conf[i][0]:
            if len(device_data[len(channels)]) == len(device_data[ch]):
                if len(device_data[len(channels)]) > 0:
                    lines[ch].set_xdata(list(device_data[len(channels)]))
                    lines[ch].set_ydata(list(device_data[ch]))
                    
                    # Plot Trigger on the correct axis if found 
                    if trigger_data['found'] == True and trigger_data['cl_id'] == ch:
                        trigger_point = get_trigger_point()
                        ax[i].plot(trigger_point['x'], trigger_point['y'], 'rx');

           # if not the same amount of data is present in the channels,
           # the data gets resetted, until they match
            else:
                clear_data()

        # Manual or autoschaling
        if not x_width == None and len(list(device_data[len(channels)])) > 0:
            last_time_stamp = device_data[len(channels)].pop()
            device_data[len(channels)].append(last_time_stamp)
            ax[i].set_xlim(last_time_stamp - x_width, last_time_stamp)
        else:
            ax[i].relim()
            ax[i].autoscale_view()


    plt.draw()
    plt.pause(1e-17)

##############################################################################

def get_address_from_map(var_name):
    with open(map_file, 'r') as f:
        for line in f:
            if var_name in line:
                value = next(f).split()[0]
                return int(value, 0)
                
    raise Exception("""The searched value \"" + var_name + "\" couldn't be 
                        found in the map file. Sending a faulty address 
                        might break the controller""")    

##############################################################################

def cleanup():
    print('Script interrupted\nCleaning up...')
    try:
        save_path  = os.path.abspath(image_path)
    except:
        save_path = os.path.expanduser("~/Desktop")

    save_path = os.path.join(save_path, figure_name) 
    figure.savefig(save_path)
    ser.close()

    print("Safed plot to: " + save_path)


##############################################################################
    
def main():    
    
    ser.flushInput()
    ser.flushOutput()

    while True:
        read_data()
        plot_data()
        time.sleep(0.001)

##############################################################################

if __name__ == "__main__":    
    channels = []
    plot_conf = []
    device_data = []
    global trigger_data;
    trigger_data = {'found' : False, 'cl_data_ind' : 0, 'cl_id' : 0};

    config()
    init_periph()
    init_plots()
   
    try:
        main()
    except KeyboardInterrupt:
        cleanup()
