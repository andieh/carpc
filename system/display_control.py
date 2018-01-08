#!/usr/bin/python2

import signal
import RPi.GPIO as GPIO
import time

IGNORE_STATE = True

checkPIN = 22
servoPIN = 18

open_pos = 50
close_pos = 120
step = 2

running = True
GPIO.setmode(GPIO.BCM)
GPIO.setup(servoPIN, GPIO.OUT)
GPIO.setup(checkPIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

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

def displayClosed():
    return GPIO.input(checkPIN)

def checkDisplayState():
    if not displayClosed():
        return

    p = GPIO.PWM(servoPIN, 50) # GPIO 17 als PWM mit 50Hz
    p.start(5) # Initialisierung
    p.ChangeDutyCycle(open_pos/float(10))
    time.sleep(.1)
    p.stop()

signal.signal(signal.SIGTERM, exit_me)
signal.signal(signal.SIGINT, exit_me)

if __name__ == "__main__":

    if IGNORE_STATE or displayClosed():
        open()

    while running:
        if not IGNORE_STATE: checkDisplayState()
        time.sleep(1)

    if IGNORE_STATE or not displayClosed():
        close()
