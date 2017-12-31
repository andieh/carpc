import serial
import time 
import random
from thread import start_new_thread

class CarPC:
    def __init__(self):
        self.con = serial.Serial('/dev/serial0', 9600, timeout=1)  # open serial port
        print self.con.name         # check which port was really used
        self.doListen = False
        self.disconnected = True

    def listen(self):
        self.doListen = True
        self.disconnected = False
        while self.doListen:
            r = self.con.readline()
            if not r:
                continue
            print r
        self.disconnected = True

    def stopListen(self):
        self.doListen = False

    def close(self):
        self.stopListen()
        while self.disconnected:
            print "waiting for closed connection"
            time.sleep(1)
        self.con.close()

    def send(self, what):
        self.con.write(what)

if __name__ == "__main__":
    car = CarPC()
    start_new_thread(car.listen, ())
    
    colorChange = False
    cnt = 0
    while 1:
        if colorChange:
            car.send("<8|{:c}>".format(random.randint(0,255)))
            car.send("<9|{:c}>".format(random.randint(0,255)))
            car.send("<@|{:c}>".format(random.randint(0,255)))
            time.sleep(0.2)
            cnt += 1
            if cnt < 0:# 255:
                print("color change finished")
                colorChange = False
                                   
        else:
            nc = raw_input()
            if nc:
                if nc == "colorChange":
                    colorChange = True
                    continue

                car.send(nc)

    car.close()

    """ser = serial.Serial(
    nc = "a"
    while 1:
        ser.write(nc)     # write a string
        if nc:
            print "sending ", nc, ord(nc), hex(ord(nc))
        nc = ""
        while True:
            r = ser.readline()
            for a in r:
                o = ord(a)
                c = chr(o)
                print a, o, c
            break
        #nc = raw_input()
        #print type(nc)
    #print int(r)
    ser.close()"""
