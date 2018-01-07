#!/usr/bin/python2

import signal
import RPi.GPIO as GPIO
import time

servoPIN = 18
open_pos = 50
close_pos = 120
step = 2

running = True
GPIO.setmode(GPIO.BCM)
GPIO.setup(servoPIN, GPIO.OUT)

def exit_me(signum, frame):
    global running
    print "uuuu, un schuess"
    running = False

def close():
    p = GPIO.PWM(servoPIN, 50) # GPIO 17 als PWM mit 50Hz
    p.start(5) # Initialisierung
    for i in range(open_pos,close_pos+step,step):
        p.ChangeDutyCycle(i/float(10))
        time.sleep(.05)
    p.stop()

def open():
    p = GPIO.PWM(servoPIN, 50) # GPIO 17 als PWM mit 50Hz
    p.start(5) # Initialisierung
    for i in range(close_pos,open_pos+step,-step):
        p.ChangeDutyCycle(i/float(10))
        time.sleep(.05)
    p.stop()

def check():
    p = GPIO.PWM(servoPIN, 50) # GPIO 17 als PWM mit 50Hz
    p.start(5) # Initialisierung
    p.ChangeDutyCycle(open_pos/float(10))
    time.sleep(.1)
    p.stop()

signal.signal(signal.SIGTERM, exit_me)
signal.signal(signal.SIGINT, exit_me)

if __name__ == "__main__":

    open()

    while running:
        #check()
        time.sleep(1)

    close()
