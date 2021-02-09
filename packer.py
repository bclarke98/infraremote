#!/usr/bin/env python3
import sys
import time
import serial
import struct

this = sys.modules[__name__]
this._usb = None
this._dev = '/dev/ttyUSB0'

def usage():
    print('Usage: ./packer.py [encoding_type] [hexcode] [PATH_TO_USB (optional)]')

def get_serial():
    usb = getattr(this, '_usb', None)
    if usb is None:
        usb = this._usb = serial.Serial(this._dev, timeout=1)
        time.sleep(2)
    return usb

def i2ul(i):
    return struct.pack('I', i)

def sendPacket(protoType, hexdata):
    s = chr(protoType) + ':'
    get_serial().write(s.encode('utf-8') + i2ul(hexdata))

def main():
    if len(sys.argv) < 3:
        return usage()
    try:
        enc = int(sys.argv[1])
        hexdata = int(sys.argv[2], 16)
        if len(sys.argv) == 4:
            this._dev = sys.argv[3]
        sendPacket(enc, hexdata)
    except (KeyError, ValueError) as e:
        print(e)
        return usage()


if __name__ == '__main__':
    main()


