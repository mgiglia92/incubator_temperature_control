import numpy as np
import matplotlib.pyplot as plt
import csv
import serial
from threading import Thread
import time
import re
from datetime import date

ser = serial.Serial(port='COM4', baudrate=115200, timeout=1)
raw_data = []

class TimeSeriesData:
    def __init__(self):
        self.fields = ['time', 'T0', 'T1', 'T2', 'T3', 'H0']
        self.time = []
        self.T0 = []
        self.T1 = []
        self.T2 = []
        self.T3 = []
        self.H0 = []
    def append_data(self, data):
        self.time.append(data.time)
        self.T0.append(data.T0)
        self.T1.append(data.T1)
        self.T2.append(data.T2)
        self.T3.append(data.T3)
        self.H0.append(data.H0)
    def to_csv(self, filename):
        print("Saving file...")
        with open(filename, 'w', newline="") as f:
            write = csv.writer(f)
            write.writerow(self.fields)
            for i in range(0, len(self.time)):
                write.writerow([self.time[i], self.T0[i], self.T1[i], self.T2[i], self.T3[i], self.H0[i]])
        print("File saved as: " + filename)
class Data:
    pass

class Decoder:
    @staticmethod
    def decode(line: str):
        # Check if line has both start and end
        start = line.find(b'$')
        end = line.find(b'#')
        if(start == -1 or end == -1):
            #discard data
            return None
        line = line[start+1:end]
        splitdata = line.split(b',')
        data = Data()
        data.time = float(splitdata[0][1:])/1000000.0 # Convert us to seconds
        data.T0 = float(splitdata[1][1:])
        data.T1 = float(splitdata[2][1:])
        data.T2 = float(splitdata[3][1:])
        data.T3 = float(splitdata[4][1:])
        data.H0 = float(splitdata[5][1:])
        return data

    @staticmethod
    def loop_decode(line_array: list):
        tsd = TimeSeriesData()
        for i in range(0, len(line_array)):
            tsd.append_data(Decoder.decode(line_array[i]))
        return tsd
    
    @staticmethod
    def check_validity(line: str):
        start = line.find(b'$')
        end = line.find(b'#')
        if(start == -1 or end == -1):
            #discard data
            return False
        return True

def start_test():
    print("Starting test (stepping up)...")
    ser.write('R0%\0\n'.encode())
    time.sleep(.1)
    ser.write('R1%\0\n'.encode())

def end_test():
    print("Ending test (stepping down)...")
    ser.write('R0%\0\n'.encode())
    time.sleep(.1)
    ser.write('R0%\0\n'.encode())
    
def record_data():
    time.sleep(1)
    while True:
        rawline = ser.readline()
        if Decoder.check_validity(rawline):
            # print(rawline)
            raw_data.append(rawline)

class GetInput:
    def __init__(self):
        self.user_in = []
    def get_input(self):
        while True:
            self.user_in = input("Commands: q: quit and save csv, save: save csv(not implemented), start: step up, end: step down: \n")
    def reset_input(self):
        self.user_in = []

if __name__ == "__main__":

    data_thread=Thread(target=record_data, daemon=True)
    time.sleep(1)
    data_thread.start()
    get = GetInput()
    input_thread = Thread(target=get.get_input, daemon=True)
    time.sleep(0.1)
    input_thread.start()

    # Save data
    plotdata = TimeSeriesData()

    # efficient plotting method
    prev_length = 0

    import atexit
    filename = 'default_name_change_name' + '.csv'
    atexit.register(plotdata.to_csv, filename)
    atexit.register(end_test)


    #plotting
    fig1 = plt.figure(1)
    while True:
        try:
            # User input logic
            if get.user_in == 'q': # Quit the program and save csv
                get.reset_input()
                Decoder.loop_decode(raw_data).to_csv(filename)
                quit()
            if get.user_in == 'start':
                get.reset_input()
                start_test()
            if get.user_in == 'end':
                get.reset_input()
                end_test()
            if get.user_in == 'save':
                get.reset_input()
                tempname = input("input a file name with .csv at end: ")
                Decoder.loop_decode(raw_data).to_csv(tempname)
            
            #Plotting logic
            plotdata = Decoder.loop_decode(raw_data)
            # Get only data we haven't plotted yet
            t = np.array(plotdata.time[prev_length:])
            T0 = np.array(plotdata.T0[prev_length:])
            T1 = np.array(plotdata.T1[prev_length:])
            T2 = np.array(plotdata.T2[prev_length:])
            T3 = np.array(plotdata.T3[prev_length:])
            H0 = np.array(plotdata.H0[prev_length:])
            prev_length = len(plotdata.time)-1 # Get length of the current array
            if prev_length == -1: prev_length = 0

            plt.plot(t, T0, 'r.', markersize=1)
            plt.plot(t, T1, 'g.', markersize=1)
            plt.plot(t, T2, 'b.', markersize=1)
            plt.plot(t, T3, 'y.', markersize=1)
            plt.plot(t, H0, 'k.', markersize=1)
            plt.show(block=False)
            plt.pause(.1)

        except KeyboardInterrupt:
            Decoder.loop_decode(raw_data).to_csv(filename)
            break
        except Exception as e:
            print(e)