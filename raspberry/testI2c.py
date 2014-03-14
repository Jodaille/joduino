#
# Send a byte to Arduino from Raspberry
# and read arduino response message 
# of 255 characters
# @author Jodaille
# @licence GPLv3
#
# cf: http://www.pihomeserver.fr/2013/08/13/raspberry-pi-home-server-arduino-lier-les-deux-via-bus-i2c/
#

import smbus
import time

# for command line parameters reading
import sys, getopt


# Replace 1 by 0 for old Raspberry (without hole)
bus = smbus.SMBus(1)

arduinomessage = ""

arduino_slave_address = 0x12

# Arguments management
# Store input and output file names
to_send = 212
to_act = ''

# Read command line args
myopts, args = getopt.getopt(sys.argv[1:],"m:a:")


for o, a in myopts:
    if o == '-m':
        to_send=int(a)
    elif opt == '-a':
        to_act=a
    else:
        print("Usage: %s -m message -a action" % sys.argv[0])
 
# Display input and output file name passed as the args
print ("Message : %s and action: %s" % (to_send,to_act) )

print "Sending number: ", to_send, " to arduino"

bus.write_byte(arduino_slave_address, to_send)

# Waitng a bit for response
time.sleep(1)

for i in range(0, 255):
	arduinomessage += chr(bus.read_byte(arduino_slave_address));

print arduinomessage