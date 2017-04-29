import serial
import signal

from time import sleep
import sys

ser = serial.Serial()

def closeHandler(signal, frame):
    print('Pressed ctrl+c, closing port')
    ser.close();
    sys.exit(0)

print('Trying to open serial port\n')
try:
    ser = serial.Serial('/dev/ttyACM0', 38400,timeout=1, parity=serial.PARITY_NONE, rtscts=1)
except:
    print('Error openning serial port!')
    exit()

print('Serial port openned\n')

ser.write(b'status\n')
logFile = open("LogFile.txt", "w")

signal.signal(signal.SIGINT, closeHandler)

running = True
while running:
    try:
        line = ser.readline()
        logFile.write(str(line)+"\n")
        print(line)
    except:
        running = False
        sleep(0.1)


print('Closing Serial port\n')
ser.close()             # close port
print('Program ended\n')