from ej22 import EJ22

import time
import sys

class FakeData(EJ22):
    def __init__(self):
        EJ22.__init__(self)

    def run(self):
        print("try to fake data")
        rpm = 0
        dir = 1
        active = True
        while self.running:
            rpm += dir*100
            if rpm > 6000:
                dir = -1
                time.sleep(10)
            if rpm < 0:
                dir = 1
                time.sleep(10)
            self.add("fanActive", active)
            self.add("pingingActive", not active)
            self.add("neutral", not active)
            self.add("ignition", not active)
            self.add("fanActive", active)
            self.add("fuelActive", not active)
            self.add("groundSpeed", rpm)
            self.add("engineSpeed", 95)
            time.sleep(0.1)
            if (rpm % 1000) == 0:
                active = not active
        print("stop receiving data")

if __name__ == "__main__":
    fd = FakeData()
    fd.setLifetime("engineSpeed", 1)
    fd.start()
    cnt = 10000

    while cnt > 0:
        time.sleep(.5)
        cnt -= 1
        sys.stdout.write("{}\r".format(fd.get("engineSpeed")))
        sys.stdout.flush()
    
    fd.stop()
    

