import serial
import datetime
import time

#Change the COM port below to match what the Arduino is on
serialPort = serial.Serial('COM7', 9600, timeout=0)

#Each packet is 4 bytes of data
packetsReceived = 0
tStart = datetime.datetime.now()

time.sleep(1)

while True:
    try:
        serialPort.readline()
        packetsReceived += 1
    except serialPort.SerialTimeoutException:
        pass

    tDelta = datetime.datetime.now() - tStart
    print(tDelta.microseconds)
    #8 bits per byte, 4 bytes per packet
    bitsReceived = 8 * 4 * packetsReceived
    dataRate = float(bitsReceived) / (float(tDelta.microseconds) / 1000.0)
    print("Bandwidth: {0} bps".format(dataRate))
