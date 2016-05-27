import struct
import serial
import sys

serialPort = 'COM7'

ser = serial.Serial(serialPort, baudrate=115200)

while (1):
    inString = raw_input("Enter the message: ")

    while(len(inString) < 9):
        inString += '0'
    
    msg = ""

    version = 1
    msgType = 1

    # Pouch values
    firstPouchVal = int(inString[0])

    msg += struct.pack('<B', (version << 6) + (msgType << 4) + firstPouchVal)
    
    for i in range(len(inString)/2):
        num1 = int(inString[2 * i + 1])
        num2 = int(inString[2 * i + 2])

        assert (num1 >= 0 and num1 < 8)
        assert (num2 >= 0 and num2 < 8)

        msg += struct.pack('<B', (num1 << 4) + num2)

    msg += struct.pack('<B', 255)
    print ':'.join(x.encode('hex') for x in msg)
    ser.write(msg)
