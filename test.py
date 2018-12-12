
import serial

#ser = serial.Serial('/dev/cu.SLAB_USBtoUART', 115200)

#b = bytearray([ 'A', 'T', 0xC0, 0 ])

led_nr = 87
led_buffer = bytearray(led_nr*3)


cmd = bytearray(b'AT')
cmd.append(0xC0)
cmd.append(0x0)

#cmd.append('T')
#cmd.append(0xC0)
#cmd.append(0x0)

print(cmd)
