import RPi.GPIO as GPIO
import time
from threading import Thread
import os

REAR_GEAR_PIN = 27
ENABLE_CAMERA = 6
runningTime = None

def runVideo():
    os.system("/home/pi/run_stream.sh")

GPIO.setmode(GPIO.BCM)
# Set up the GPIO channels - one input and one output
GPIO.setup(REAR_GEAR_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(ENABLE_CAMERA, GPIO.OUT)

t = None
while 1:
    rear_gear = GPIO.input(REAR_GEAR_PIN)
    if not rear_gear: # low active
        if t is None:
            print("rear gear selected!")
            GPIO.output(ENABLE_CAMERA, 1)
            t = Thread(target=runVideo)
            t.setDaemon(True)
            t.start()
        else:
            runningTime = time.time()  
    else:
        if runningTime is not None:
            diff = time.time() - runningTime
            print("rear gear off, wait {}".format(diff))
            if diff > 10:
                os.system("killall mplayer")
                t.join()
                t = None
                GPIO.output(ENABLE_CAMERA, 0)
                runningTime = None

    time.sleep(0.1)

# Clean um GPIO settings
GPIO.cleanup()
