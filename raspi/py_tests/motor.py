import RPi.GPIO as GPIO
import time
import sys

servoPIN = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(servoPIN, GPIO.OUT)

p = GPIO.PWM(servoPIN, 50) # GPIO 17 als PWM mit 50Hz
p.start(5) # Initialisierung

# alter sportline 
starti = 50
endi = 120
step = 2

for i in range(starti,endi+step,step):
    p.ChangeDutyCycle(i/float(10))
    time.sleep(.05)

for i in range(endi,starti-step,-step):
    p.ChangeDutyCycle(i/float(10))
    time.sleep(.05)

p.stop()

sys.exit(1)

try:
  while True:
    for i in range(starti, endi, 1):
        p.ChangeDutyCycle(i/float(10))
        time.sleep(0.02)
    for i in range(endi, starti, -1):
        p.ChangeDutyCycle(i/float(10))
        time.sleep(0.02)
    
    continue

    p.ChangeDutyCycle(7.3)
    time.sleep(2)
    p.ChangeDutyCycle(11)
    time.sleep(2)
    continue

    for i in range(15,25,1):
        p.ChangeDutyCycle(i)
        time.sleep(1)
        print("lol")
except KeyboardInterrupt:
  p.stop()
  GPIO.cleanup()
