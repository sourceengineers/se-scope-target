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
import matplotlib.pyplot as plt
import collections
import os
import numpy as np

def config():
    # Do not change these definitions!
    global serial_file
    global image_path
    global baudrate
    global timeout
    global figure_name
    global x_width
    global map_file
    global trigger_conf
    global step_size
    ##########################################################################
    ### Serial optionen
    ##########################################################################
    serial_file = '/dev/ttyACM0' # z.B.: COM1 bei Windows
    baudrate = 115200
    timeout = 2

    ##########################################################################
    ### Pfade
    ##########################################################################
    # Pfad zum speichern der plots nach dem herunterfahren.
    # Wenn kein pfad angegeben ist, wird automatisch auf den Desktop gespeichert
    image_path = None
    #image_path = "/Users/USER/Documents"

    # Pfad zum map file
    map_file = "/home/schuepbs/Documents/Projects/cmake_embedded/build/nucleo.map"

    ##########################################################################
    ### Channel Konfiguration
    ##########################################################################
    # typ = UINT8, UINT16, UINT32, FLOAT)
    # Die Addressen koennen entweder aus der map File ausgelesen werden, falls sie
    # als statisch deklariert sind, oder manuell eigetragen werden.
    channels.append({'name' : "Sinus", 'address' : get_address_from_map("sinus"), 'type' :  "FLOAT"})
    channels.append({'name' : "Cosinus", 'address' : get_address_from_map("cosinus"), 'type' :  "FLOAT"})
    channels.append({'name' : "Leistung", 'address' : get_address_from_map("leistung"), 'type' :  "FLOAT"})
    channels.append({'name' : "Schmitt triggered", 'address' : get_address_from_map("schmitttriggered"), 'type' :  "UINT8"})

    ##########################################################################
    ### Trigger Konfiguration
    ##########################################################################
    #trigger_conf = {'mode' : 'Continous', 'level' : 1.4, 'edge' : 'rising', 'cl_id' : 1}
    trigger_conf = {'mode' : 'Normal', 'level' : 0.75, 'edge' : 'rising', 'cl_id' : 3}

    # Mit der step size, kann eingestellt werden, wie häufig gepollt werden soll.
    # Wird z.B. der timestmap pro Millisekunde incrementiert, und die step_size ist 10,
    # so wird nur jede 10. Millisekunde gepollt.
    step_size = 1

    ##########################################################################
    ### Grafik Konfiguration
    ##########################################################################
    add_subplot(["Sinus", "Cosinus", "Leistung"], \
             {'title' : 'Sin and Cos', 'y_label' : 'Value', 'x_label' : 'Time [ms]'})
    add_subplot(["Schmitt triggered"], \
             {'title' : 'Schmitt triggered', 'y_label' : 'Strange values', 'x_label' : 'Time [ms]'})

    figure_name = "Device data"

    # wenn die x_width auf None gesetzt wird, wird die Achse automatisch
    # skaliert
    #x_width = None

    x_width = 100
    x_width = x_width * step_size

##############################################################################
## Don't change any code after this line, or you might cause the code to break!
##############################################################################
def transmit_data(data):
    checksum = 0;
    for d in data:
        checksum = checksum + ord(d);

    data_to_send = data + "transport:" + ''.join('{:02X}'.format(checksum))[-2:] + '\0'
    data_to_send = data_to_send.encode("utf-8")
    print(data_to_send)
    ser.write(data_to_send)

##############################################################################

def send_command(command, wait_for_ack):

    while True:
        transmit_data(command)
        time.sleep(1)
        ser.flush()
        if wait_for_ack == False:
            break;
        if found_flow_ctrl() == "ACK" :
            break;
        print("Something went wrong. Trying to send again")

##############################################################################

def found_flow_ctrl():
    while True:
        received_data = ser.read_until(b'\0')
        received_data = received_data[:-1].split(b'transport')[0];
        #print(received_data) # uncomment this to line to see reived ack nak data
        data = process_data(received_data)
        if not data == None:
            if ("flow_ctrl" in data["payload"]) == True:
                print(data)
                if(data["payload"]["flow_ctrl"] == "ACK"):
                    return "ACK";
                if(data["payload"]["flow_ctrl"] == "NAK"):
                    return "NAK";
            else:
                return "NAK";
            return "NAK";
        return "NAK";

##############################################################################

def init_periph(send_commands):
    global ser

    ser = serial.Serial(serial_file, baudrate, timeout=timeout)
    ser.flushInput()
    ser.flushOutput()

    addresses = []
    types = []
    new_state = []
    ids = []

    if(send_commands == False):
        return;

    #channels konfigurieren
    for i in range(len(channels)):
        addresses.append(str(channels[i]['address']))
        types.append(channels[i]['type'])
        ids.append(str(i))
        new_state.append("true")

    send_command(cf_tgr.getCommand(str(trigger_conf['cl_id']), trigger_conf['mode'], str(trigger_conf['level']), str(trigger_conf['edge'])), True);
    send_command(cf_addr.getCommand(ids, addresses, types, len(channels)), True)
    send_command(cf_t_inc.getCommand(str(step_size)), True)
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
    global trigger_lines
    global trigger_on_axis
    lines = []
    trigger_lines = {'vline' : [], 'hline' : []}

    figure, ax = plt.subplots(len(plot_conf), 1)
    figure.suptitle(figure_name)
    ax = list(ax)

    trigger_on_axis = None;

    for i in range(len(ax)):
        ax[i].set_title(plot_conf[i][1]['title'])
        ax[i].set_xlabel(plot_conf[i][1]['x_label'])
        ax[i].set_ylabel(plot_conf[i][1]['y_label'])
        ax[i].grid(which='minor', alpha=0.2)
        ax[i].grid(which='major', alpha=0.5)
        for ch in plot_conf[i][0]:
            lines = lines + ax[i].plot([], [])
            if(trigger_conf['cl_id'] == ch):
                trigger_on_axis = i;

    if not trigger_conf['mode'] =='Continous':
        trigger_lines['vline'] = ax[trigger_on_axis].axvline(0, linestyle='dashed', color='r');
        trigger_lines['hline'] = ax[trigger_on_axis].axhline(0, linestyle='dashed', color='r');

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
    for data_package in data.split(b'transport:00\0'):
        data_package = data_package.split(b'transport')[0];
        ans = process_data(data_package)
        if ans is None:
            continue;
        if not data_is_present(ans):
            continue;

        trigger_data = get_trigger_data(ans)

        if trigger_data['found'] == True:
            clear_data();

        # load tmestamp data
        device_data[len(channels)] = append_to_channel(device_data[len(channels)], ans["payload"]["sc_data"]["t_stmp"])

        # load channel data
        for i in range(len(channels)):
            if channel_data_is_present(i, ans) == True:
                device_data[i] = append_to_channel(device_data[i], ans["payload"]["sc_data"]["cl_data"][str(i)])

    return True
##############################################################################
def clear_data():
    for data in device_data:
        data.clear()
##############################################################################

def read_data():
    answer = ser.read_until(b'transport:00\0');

    #print(answer)    # Uncomment this print if you want to see the complete
    # output of the scope
    if ((b'{\"payload' in answer[0:10]) and (b'transport:00\0' in answer)):
        return prepare_data(answer)
    elif len(answer) > 0:
        print("Data pack corrupted...")
        print(answer);
    return False

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
    return {'x' : trigger_data['cl_data_ind'], 'y' : trigger_conf['level']}

##############################################################################
def plot_data():

    for i in range(len(plot_conf)):
        for ch in plot_conf[i][0]:
            if len(device_data[len(channels)]) == len(device_data[ch]):
                if len(device_data[len(channels)]) > 0:

                    # Plot all lines
                    lines[ch].set_xdata(list(device_data[len(channels)]))
                    lines[ch].set_ydata(list(device_data[ch]))

           # if not the same amount of data is present in the channels,
           # the data gets resetted, until they match
            else:
                clear_data()

    # Scale the plot
    for i in range(len(plot_conf)):

        # Calculate values vor x and y lim

        extrem_values = []

        for ch in plot_conf[i][0]:
            extrem_values.append(max(list(device_data[ch])));
            extrem_values.append(min(list(device_data[ch])));

        y_min = min(extrem_values);
        y_max = max(extrem_values);
        d_y = np.absolute(y_max - y_min)
        y_min = y_min - (d_y / 10);
        y_max = y_max + (d_y / 10);
        ax[i].set_ylim(y_min, y_max)

        # Manual or autoscaling
        if not x_width == None and len(list(device_data[len(channels)])) > 0:
            if x_width < len(device_data[len(channels)]):
                ax[i].set_xlim(device_data[len(channels)][-x_width], device_data[len(channels)][-1])
            else:
                ax[i].set_xlim(device_data[len(channels)][0], device_data[len(channels)][-1])

        else:
            ax[i].set_xlim(device_data[len(channels)][0], device_data[len(channels)][-1])

        # Draw grid
        left, right = ax[i].get_xlim()
        dx = (right-left)/10
        x_major_ticks = np.arange(left, right, dx)
        x_minor_ticks = np.arange(left, right, dx/5)
        bottom, top = ax[i].get_ylim()
        dy = np.absolute(top-bottom)/10
        y_major_ticks = np.arange(bottom, top, dy)
        y_minor_ticks = np.arange(bottom, top, dy/5)

        ax[i].set_xticks(x_major_ticks)
        ax[i].set_xticks(x_minor_ticks, minor=True)
        ax[i].set_yticks(y_major_ticks)
        ax[i].set_yticks(y_minor_ticks, minor=True)
        ax[i].grid(which='minor', alpha=0.5, linestyle='--')
        ax[i].grid(which='major', alpha=1, linestyle='--')

        for text in ax[i].texts:
            text.remove();

        ax[i].text(0, 0, "dx:{:10.2f}, dy:{:10.2f}".format(dx,dy), size=10, ha="left", transform=ax[i].transAxes)

        # Plot the trigger point
    if trigger_data['found'] == True and not trigger_conf['mode'] =='Continous':
        trigger_point = get_trigger_point()
        trigger_lines['vline'].remove()
        trigger_lines['hline'].remove()
        trigger_lines['hline'] = ax[trigger_on_axis].axhline(trigger_point['y'], color='r', linestyle='dashed');
        trigger_lines['vline'] = ax[trigger_on_axis].axvline(trigger_point['x'], color='r', linestyle='dashed');

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
        data_ready_to_plot = read_data()
        if(data_ready_to_plot == True):
            plot_data()

##############################################################################

if __name__ == "__main__":
    channels = []
    plot_conf = []
    device_data = []
    global trigger_data;
    trigger_data = {'found' : None, 'cl_data_ind' : None, 'cl_id' : None};

    config()

    try:
        print("Reinitialize preipherie: " + sys.argv[1]);
        init_periph(eval(sys.argv[1]))
    except:
        init_periph(True)

    init_plots()

    try:
        main()
    except KeyboardInterrupt:
        cleanup()
