"""
Parameters readable from the EJ22 SSM.

good starting point can be found here:
http://www.4bc.org/vanagon/engine.html

there is a list of parameters with explanations.
the following list has a short name, so the parameter
can be accessed by get<shortname>. 

continuous parameters will return a float value, 
binary parameters true/false

continuous parameters:              short
---------------------------------------------------------
    ROM ID                          romId
    System voltage                  voltage
    Vehicle speed                   groundSpeed
    Engine speed                    engineSpeed
    Coolant temperature             coolantTemp
    Ignition timing                 timing
    Airflow signal                  airflow
    Load                            load
    Throttle position signal        throttle
    Zeroed throttle position        zeroedThrottle
    Injector pulse width            injectionWith
    IAC valve duty cycle            dutyCycle
    Oxygen sensor signal            oxygen
    Fuel trim                       fuelTrim
    Timing correction               timingCorrection
    Boost control duty cycle        boostControl
    Barometric pressure             pressure
    Boost/vacuum                    boost

binary parameters:                  short
---------------------------------------------------------
    Ignition switch                 ignition
    Automatic transmission mode     transmissionMode
    Test mode                       testMode
    Read memory mode                memoryMode
    Neutral switch                  neutral
    Park switch                     park
    California mode                 california
    Idle switch                     idle
    Air conditioning switch         acSwitch
    Air conditioning relay active   acActive
    Radiator fan relay active       fanActive
    Fuel pump relay active          fuelActive
    Canister purge valve active     canActive
    Pinging detected                pingingActive
    Pressure exchange               pressureMeas
"""

# default lifetime of values
# can be adjusted manually for every value
DEFAULT_LIFETIME = 10

import time
import sys
from thread import start_new_thread
from threading import RLock # good idea to mix thread libs?
from data import DataList


""" 
abstract data class with containers and stored data.
derive several models for:
    - fake data loader
    - direct connection to the SSM
    - with something in between (raspberry pi?)
"""
class EJ22:
    def __init__(self):
        # timestamp of the last received data
        self.lastUpdate = self.getCurrentTime()
        # thread safe lock?
        self.lock = RLock()
        # last cleanup, reduce runtime
        self.lastCleanup = self.getCurrentTime()

        # set this to true and start the main thread to 
        # start writing data
        self.running = False 

        self.continuousFields = ["romId", "voltage", \
            "groundSpeed", "engineSpeed", "coolantTemp",\
            "timing", "airflow", "load", "throttle", \
            "zeroedThrottle", "injectionWith", "dutyCycle",\
            "oxygen", "fuelTrim", "timingCorrection",\
            "boostControl", "pressure", "boost"]
        for cf in self.continuousFields:
            setattr(self, cf, DataList())

        self.binaryFields = ["ignition", "transmissionMode",\
            "testMode", "memoryMode",  "neutral", "park",\
            "california", "idle", "acSwitch", "acActive",\
            "fanActive", "fuelActive", "canActive",\
            "pingingActive", "pressureMeas"]
        for bf in self.binaryFields:
            setattr(self, bf, DataList(type=1,lifetime=DEFAULT_LIFETIME))

        self.fields = self.binaryFields + self.continuousFields

    def cleanupData(self):
        """
        cleanup data if too old
        lifetime can be set individually for every value 
        """
        ts = self.getCurrentTime()
        # only cleanup every second
        if (ts - self.lastCleanup) < 1:
            return 
        for field in self.fields:
            getattr(self,field).cleanup()
            
        self.lastCleanup = ts

    def setLifetime(self, field, lifetime):
        """ 
        set lifetime for a given field
        """
        if field not in self.fields:
            print("data field {} not available".format(field))
            sys.exit(1)
        getattr(self, field).setLifetime(lifetime)

    def getCurrentTime(self):
        return time.time()

    def add(self, field, value):
        if field not in self.fields:
            print("data field {} not available".format(field))
            sys.exit(1)

        self.lock.acquire()
        ts = self.getCurrentTime()
        try:
            getattr(self, field).add(ts, value)
        finally:
            self.lock.release()

        self.lastUpdate = ts
        self.cleanupData()

    def get(self, field):
        if field not in self.fields:
            print("data field {} not available".format(field))
            sys.exit(1)

        self.lock.acquire()
        try:
            value = getattr(self, field).get()
        finally:
            self.lock.release()
        self.cleanupData()
        return value

    def run(self):
        # implement this function in your derived class
        while self.running:
            print("do not run this, please implement something to feed me")
            time.sleep(1)
            
    def start(self):
        self.running = True
        start_new_thread(self.run, ())

    def stop(self):
        self.running = False

if __name__ == "__main__":
    ej22 = EJ22()
    print(ej22.get("pingingActive"))
