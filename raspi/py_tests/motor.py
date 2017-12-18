import RPi.GPIO as GPIO
import time

servoPIN = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(servoPIN, GPIO.OUT)

p = GPIO.PWM(servoPIN, 50) # GPIO 17 als PWM mit 50Hz
p.start(5) # Initialisierung
try:
  while True:
    for i in range(5,11,1):
        p.ChangeDutyCycle(i)
        time.sleep(1)
        print("lol")
except KeyboardInterrupt:
  p.stop()
  GPIO.cleanup()
