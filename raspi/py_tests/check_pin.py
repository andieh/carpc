import RPi.GPIO as GPIO
import time

READ_PIN = 5
dialog_active = False

def test(pin):
    global dialog_active
    if dialog_active:
        return 
    dialog_active = True
    print("oh ha!")

GPIO.setmode(GPIO.BCM)
# Set up the GPIO channels - one input and one output
GPIO.setup(READ_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.add_event_detect(READ_PIN, GPIO.FALLING, callback=test)

while 1:
    # Input from pin 11
    input_value = GPIO.input(READ_PIN)
    print(input_value)
    time.sleep(1)

# Clean um GPIO settings
GPIO.cleanup()
