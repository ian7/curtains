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

# Connect
client.connect(url.hostname, url.port)


dev = InputDevice(DEVICE)

def receive( code ):
    if( code == 83 ):
        print('stop')
	client.publish("/off","1")
	client.publish("/off","2")
    if( code == 82 ):
        print('close')
	client.publish("/on","1")
	client.publish("/off","2")
    if( code == 96 ):
        print('open')
	client.publish("/off","1")
	client.publish("/on","2")
    if( code == 79 ):
        print('open')
	client.publish("/off","3")
	client.publish("/off","4")
    if( code == 80 ):
        print('open')
	client.publish("/off","3")
	client.publish("/on","4")
    if( code == 81 ):
        print('open')
	client.publish("/on","3")
	client.publish("/off","4")
    if( code == 75 ):
        print('open')
	client.publish("/on","3")
	client.publish("/on","4")


while True:
    # wait for keypad command
    r, w, x = select([dev], [], [])

    # read keypad        
    for event in dev.read():
        if event.type==1 and event.value==1:
            print "KEY CODE: " + str(event.code)
            receive( event.code )

