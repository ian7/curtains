#!/usr/bin/env python

import string
import os
import sys
import time
import mosquitto
import urlparse

# pip install evdev
from evdev import InputDevice
from select import select

# look for a /dev/input/by-id/usb...kbd or something similar
DEVICE = "/dev/input/by-id/usb-Telink_Wireless_Receiver-if01-event-kbd"


client = mosquitto.Mosquitto()
url_str = 'mqtt://10.10.1.3:1883'
url = urlparse.urlparse(url_str)



dev = InputDevice(DEVICE)

def receive( code ):
    if( code == 69 ):
	print('n')
	client.publish("/light/brightness","+")
    if( code == 71 ):
	print('n')
	client.publish("/light/brightness","-")
    if( code == 98 ):
	print('n')
	client.publish("/light/saturation","+")
    if( code == 72 ):
	print('n')
	client.publish("/light/saturation","-")
    if( code == 55 ):
	print('n')
	client.publish("/light/hue","+")
    if( code == 73 ):
	print('n')
	client.publish("/light/hue","-")
    if( code == 75 ):
	print('1')
	client.publish("/watering","1")
    if( code == 76 ):
	print('2')
	client.publish("/watering","2")
    if( code == 77 ):
	print('3')
	client.publish("/watering","3")
    if( code == 14 ):
	print('4')
	client.publish("/watering","4")
    if( code == 78 ):
	print('0')
	client.publish("/watering","0")
    if( code == 83 ):
        print('stop')
	client.publish("/curtains/command","stop")
    if( code == 82 ):
        print('open')
	client.publish("/curtains/command","open")
    if( code == 96 ):
        print('close')
	client.publish("/curtains/command","close")
    if( code == 79 ):
        print('slow')
	client.publish("/curtains/command","slow")
    if( code == 80 ):
        print('fast')
	client.publish("/curtains/command","fast")
    if( code == 81 ):
        print('3')
    if( code == 75 ):
        print('4')


while True:
    # wait for keypad command
    r, w, x = select([dev], [], [])

    # read keypad        
    for event in dev.read():
        if event.type==1 and event.value==1:
	    # Connect
	    client.connect(url.hostname, url.port)
            print "KEY CODE: " + str(event.code)
            receive( event.code )
	    client.disconnect()

