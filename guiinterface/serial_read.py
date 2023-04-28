import struct
import serial

serialPort = serial.Serial(
    port='COM4', baudrate=9600, bytesize=8
)

while(serialPort.isOpen()):
    header = serialPort.read(1)
    if header[0] == 0x83:
        buffer = serialPort.read(17)
        print(buffer)
        received = struct.unpack('=bLffhh',buffer)
        print(received)      