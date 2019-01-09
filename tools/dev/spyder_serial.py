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
import threading

def config():
    global serial_file
    global baudrate
    global timeout
    global figure_name
    global max_data_length
    ### Serial optionen
    serial_file = '/dev/tty.usbmodem14203' # z.B.: COM1 bei Windows 
    baudrate = 115200
    timeout = 0.5
    
    ### Channel konfiguration
    # typ = UINT8, UINT16, UINT32, FLOAT)
    channels.append({'name' : "Address_32", 'address' : 536969196, 'type' :  "UINT32"})
    channels.append({'name' : "Address_FLOAT", 'address' : 536969192, 'type' :  "FLOAT"})
#    channels.append({'name' : "Address_16", 'address' : 1234, 'type' :  "UINT32"})
#    channels.append({'name' : "Address_FLOAT", 'address' : 1234, 'type' :  "FLOAT"})
    
 #   add_plot(["Address_32", "Address_16"], {'y_label' : 'Voltage', 'x_label' : 'Time'})
    add_plot(["Address_FLOAT"], {'y_label' : 'Voltage', 'x_label' : 'Time'})
    add_plot(["Address_32"], {'y_label' : 'Voltage', 'x_label' : 'Time'})
    
    
    figure_name = "Device data"

    max_data_length = 200



















##############################################################################
## Don't change any code after this line, or you might cause the code to break!
##############################################################################
    
def send_command(command):
    command = command.encode("utf-8")
 #   print(command)
    ser.write(command)
    time.sleep(2)
    
##############################################################################
    
def init_periph():
    global ser
    global thread_read
    ser = serial.Serial(serial_file, baudrate, timeout=timeout)
    ser.flushInput()
    ser.flushOutput()
 
    send_command(ev_announce.getCommand())

    #channels konfigurieren
    for i in range(len(channels)):
        send_command(cf_addr.getCommand(str(i), str(channels[i]['address']), 
                                        channels[i]['type']))
        send_command(cf_running.getCommand(str(i), "true"))
    
    #Clear data
    send_command(ev_clear.getCommand())
    
    
    thread_read = threading.Thread(target=read_data)
    thread_read.start()

##############################################################################

def add_plot(new_plot, labels):
    
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
        data = data.decode("utf-8")
        try:
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
    
    
    figure, ax = plt.subplots(len(plot_conf), 1)
    figure.suptitle(figure_name)    
    
    for i in range(len(ax)):
        ax[i].set_autoscaley_on(True)
        ax[i].grid()
        
    for i in range(len(channels) + 1):
       device_data.append([])
                          
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
    device_data[len(channels)] = \
        device_data[len(channels)] + ans["payload"]["sc_data"]["t_stmp"]
        
    # load channel data
    for i in range(len(channels)):
        if (str(i) in ans["payload"]["sc_data"]["cl_data"]) == True:
            device_data[i] = \
                device_data[i] + ans["payload"]["sc_data"]["cl_data"][str(i)]

##############################################################################

def read_data():
    while True:
        answer = ser.read_until(b'\0');
        prepare_data(answer)

##############################################################################
    
def plot_data(): 
    
    while True:
        for i in range(len(plot_conf)):
            for ch in plot_conf[i][0]:
                if len(device_data[len(channels)]) == len(device_data[ch]):
                    ax[i].plot(device_data[len(channels)], device_data[ch])   
                    print(device_data[ch])
                    
        figure.canvas.draw()

##############################################################################
    
def main():    
    
    ser.flushInput()
    ser.flushOutput()
    
    plot_data()

##############################################################################

if __name__ == "__main__":
   channels = []
   plot_conf = []
   device_data = []
       
   config()
   
   init_periph()
   init_plots()
   
   main()