import os, sys, time
import xbmc, xbmcgui, xbmcaddon
from threading import Thread

__addon__        = xbmcaddon.Addon()
__addonid__      = __addon__.getAddonInfo('id').decode( 'utf-8' )
__addonname__    = __addon__.getAddonInfo('name').decode("utf-8")

SERVO_PIN           = 18
POWER_12_PIN        = 5
POWER_5_PIN         = 21
POWER_OUT_PIN       = 20
REARCAM_INPUT_PIN   = 27
REARCAM_OUTPUT_PIN   = 6

class MyMonitor(xbmc.Monitor):
    def __init__(self, settings_callback):
        self.settings_callback = settings_callback
        xbmc.Monitor.__init__(self)

    def onSettingsChanged(self):
        self.settings_callback()

class Main(object):
    pin = 0
    edge = 0
    silent = 'false'
    function = ''
    ignore_power = False
    dialog_active = False   
    running = True
    dialog = None
    rearcam_active = False

    def __init__(self):
        pass

    def start(self):
        monitor = MyMonitor(self.on_settings_changed)
        self.setup()
        self.thread = Thread(target=self.checker)
        self.thread.setDaemon(True)
        self.thread.start()
        monitor.waitForAbort()
        self.running = False
        self.finished = False
        self.cleanup()

    def checker(self):
        self.log("start thread")
        GPIO.setmode(GPIO.BCM)

        # set servo 
        GPIO.setup(SERVO_PIN, GPIO.OUT)
        self.p = GPIO.PWM(SERVO_PIN, 50) # GPIO 17 als PWM mit 50Hz
        self.p.start(5) # Initialisierung
        self.p.ChangeDutyCycle(3)
        time.sleep(1) # wait until position is reached
    
        # set power as in put
        self.log("init power stuff")
        GPIO.setup(POWER_12_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
        GPIO.setup(POWER_5_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
        GPIO.setup(POWER_OUT_PIN, GPIO.OUT)
        GPIO.output(POWER_OUT_PIN, 1)

        # check if rear gear is enabled
        GPIO.setup(REARCAM_INPUT_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
        GPIO.setup(REARCAM_OUTPUT_PIN, GPIO.OUT)

        while self.running:
            power12 = GPIO.input(POWER_12_PIN) # low active
            power5  = GPIO.input(POWER_5_PIN)  # low active
            if power12 and power5 and not self.ignore_power: # both power removed!
                self.power_removed()
            elif not power12 or not power5:
                self.ignore_power = False

            self.check_rearcam()

            time.sleep(0.01)

        self.p = GPIO.PWM(SERVO_PIN, 50) # GPIO 17 als PWM mit 50Hz
        self.p.start(5) # Initialisierung
        self.p.ChangeDutyCycle(12)
        time.sleep(1)
        self.p.stop()

        self.finished = True

    def cleanup(self):
        self.log("cleanup called, wait for thread to finish")
        cnt = 0
        self.running = False
        while not self.finished:
            cnt += 1
            time.sleep(0.1)
            if cnt > 15:
                self.log("abort waiting")
                break

        self.log("cleanup GPIO")
        try: 
            GPIO.cleanup()
        except: 
            pass

    def on_settings_changed(self):
        pass
        #    self.cleanup()
        #    self.setup()

    def log(self, txt):
        print("[GPIO-CHECK] {}".format(txt))

    def check_rearcam(self):
        state = GPIO.input(REARCAM_INPUT_PIN)
        current = time.time()
        diff = current-self.rearcam_active
        if self.rearcam_active and not state: # cam started and rear active
            self.log("rear cam already active for {}s!".format(diff))
            self.rearcam_active = current
            time.sleep(1)   
            return 
        
        elif self.rearcam_active and state: # cam started and not in rear mode
            if diff < 10:
                self.log("wait until switch off camera ({}s)".format(diff))
                time.sleep(1)
                return 
            self.log("rear gear switched, turn off camera")
            GPIO.output(REARCAM_OUTPUT_PIN, 0)
            self.rearcam_active = False
            time.sleep(1)
            return
        
        elif not state: # start cam
            self.log("i need to start the cam!")
            GPIO.output(REARCAM_OUTPUT_PIN, 1)
            #xbmc.executebuiltin("RunScript(plugin.program.rearcam)")
            xbmc.executebuiltin("XBMC.RunScript(/home/pi/carpc/raspi/plugin.program.rearcam/addon.py)")
            self.rearcam_active = time.time()
            time.sleep(1)
            return 
            
        else: # nothing to do, return
            return 

        self.log("this should never happen!")

    def power_removed(self):
        """
        power to the system was removed
        """
        if self.dialog_active:
            return 

        self.dialog_active = True
        self.log("power triggered")
        self.dialog = xbmcgui.Dialog()
        print(dir(self.dialog))
        if self.dialog.yesno(heading="Power removed", line1="Power was removed", line2="Shutdown System?"):
            GPIO.output(POWER_OUT_PIN, 0)
            os.system("sudo shutdown -h now")   
        else:
            self.ignore_power = True
        self.dialog_active = False
        self.dialog = None

    def setup(self):
        settings = xbmcaddon.Addon()

        self.pin = int(settings.getSetting("pin"))
        self.edge = int(settings.getSetting("edge"))
        self.function = settings.getSetting("function").strip()
        self.silent = settings.getSetting("silent").strip()
        #self.cleanup()
        
if __name__ == '__main__':
    dialog = xbmcgui.Dialog()
    import RPi.GPIO as GPIO

    print("init gpio check")
    #dialog.ok(__addonname__, "Was geht ab?", "alter sack")

    main = Main()
    print("main created")
    main.start()
    print("start ended")
