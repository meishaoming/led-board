
import serial
import struct

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

    def begin(self):
        try:
            self.ser = serial.Serial(
                    port     = '/dev/cu.SLAB_USBtoUART',
                    baudrate = 115200,
                    timeout  = None,
                    parity   = serial.PARITY_NONE,
                    stopbits = serial.STOPBITS_ONE,
                    bytesize = serial.EIGHTBITS,
                    xonxoff  = False,
                    rtscts   = False,
                    dsrdtr   = False,
                    exclusive = True,
                    )
        except:
            raise

    def show(self):
        # write data to serial
        cmd = bytearray()
        cmd.append(0x41)
        cmd.append(0x54)
        cmd.append(0xC0)
        cmd.append(0x0)
        cmd.extend(struct.pack('H', 1+len(self._led_data.arr))) #len
        cmd.append(0)
        cmd.extend(self._led_data.arr)
        cmd.extend(struct.pack('H', 0x1213)) #crc
#        print(len(cmd))
#        print(cmd)
        self.ser.write(cmd)
        self.ser.flush()
        pass

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
