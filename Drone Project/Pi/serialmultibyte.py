import serial
import struct
import time

ser = serial.Serial('/dev/ttyACM0', 9600)

# let it initialize
time.sleep(2)

# send the first int in binary format
# write to arduino as raw binary
ser.write(struct.pack('>BB',45,99))
