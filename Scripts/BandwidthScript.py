import serial
import datetime
import time

#Change the COM port below to match what the Arduino is on
serialPort = serial.Serial('COM7', 9600, timeout=0)

#Each packet is 4 bytes of data
packetsReceived = 0
tStart = datetime.datetime.now()

while True:
    try:
        serialPort.readline()
        packetsReceived += 1
    except serialPort.SerialTimeoutException:

    tDelta = datetime.datetime.now() - tStart

    #8 bits per byte, 4 bytes per packet
    bitsReceived = 8 * 4 * packetsReceived
    dataRate = (float)bitsReceived / (float)tDelta.seconds
    print("Bandwidth: {0} bps".format(dataRate))

    #Sleep for 50 milliseconds to avoid checking too often
    #Assuming sender is sending at about 10Hz
    time.sleep(0.05)