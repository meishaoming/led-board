
import serial
import struct

import crc16pure

import binascii

class Command(object):
    def __init__(self, action, led_data=None):
        sync_byte = 0xC0
        version = 0x0
        self.data = bytearray(b'AT')
        self.data.append(sync_byte)
        self.data.append(version)
        #print(self.data)

        cmd_data = bytearray(action)
        if led_data != None:
            cmd_data.extend(led_data)

        self.data.extend(struct.pack('H', len(cmd_data)))

        self.data.extend(cmd_data)

        crc = crc16pure.crc16xmodem(cmd_data)
        self.data.extend(struct.pack('H', crc)) #crc

        #print('crc', hex(crc))
        #print('length', len(self.data))
        #print('data', binascii.hexlify(self.data))


def Color(red, green, blue, white = 0):
    return (white << 24) | (red << 16) | (green << 8) | blue

class LED_Data(object):
    def __init__(self, size):
        self.size = size
        self.arr = bytearray(size*3)

    def __getitem__(self, pos):
        return self.arr[pos]

    def __setitem__(self, pos, value):
        self.arr[pos*3]   = (value >> 16) & 0xFF
        self.arr[pos*3+1] = (value >> 8) & 0xFF
        self.arr[pos*3+2] = value & 0xFF

class NeoPixel(object):
    def __init__(self, num):
        self._led_data = LED_Data(num)

    def begin(self, port='/dev/ttyUSB0'):
        try:
            self.ser = serial.Serial(
                    port     = port,
                    baudrate = 115200,
                    timeout  = None,
                    parity   = serial.PARITY_NONE,
                    stopbits = serial.STOPBITS_ONE,
                    bytesize = serial.EIGHTBITS,
                    xonxoff  = False,
                    rtscts   = False,
                    dsrdtr   = False,
                    )
        except:
            raise

    def close(self):
        self.ser.close()

    def reset(self):
        cmd = Command(b'R')
        self.ser.write(cmd.data)
        self.ser.flushOutput()
        self.ser.flushInput()

    def echo(self):
        cmd = Command(b'E')
        self.ser.write(cmd.data)
        self.ser.flushOutput()
        self.ser.flushInput()

    def show(self):
        # write data to serial
        cmd = Command(b'D', self._led_data.arr)
        self.ser.write(cmd.data)
        self.ser.flushOutput()
        self.ser.flushInput()

    def setPixelColor(self, n, color):
        self._led_data[n] = color

    def setPixelColorRGB(self, n, red, green, blue, white = 0):
        self.setPixelColor(n, Color(red, green, blue, white))

    def getPixels(self):
        return self._led_data

    def numPixels(self):
        return self._led_data.size

    def getPixelColor(self, n):
        return self._led_data[n]

if __name__ == '__main__':
    strip = NeoPixel(10)
    strip.begin()
    strip.echo()
    strip.reset()
    strip.close()
