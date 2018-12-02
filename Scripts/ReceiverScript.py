import serial
import datetime
import time

#Change the COM port below to match what the Arduino is on
serialPort = serial.Serial('COM7', 9600, timeout=0)

while True:
    try:
        print(serialPort.readline())
        print(datetime.datetime.now())
        print("\n\n\n")
    except serialPort.SerialTimeoutException:
        print('Data could not be read')
    time.sleep(1)