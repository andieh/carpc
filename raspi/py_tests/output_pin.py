import RPi.GPIO as GPIO
import time

SET_PIN = 6
state = True

GPIO.setmode(GPIO.BCM)
GPIO.setup(SET_PIN, GPIO.OUT)

while 1:
    GPIO.output(SET_PIN, state)
    print(state)
    state = not state
    time.sleep(1)

# Clean um GPIO settings
GPIO.cleanup()
