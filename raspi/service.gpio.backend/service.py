import os, sys, time
import xbmc, xbmcgui, xbmcaddon
from threading import Thread

__addon__        = xbmcaddon.Addon()
__addonid__      = __addon__.getAddonInfo('id').decode( 'utf-8' )
__addonname__    = __addon__.getAddonInfo('name').decode("utf-8")

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
    running = True
    power_dialog = None

    rearcam_active = False
    rearcam_thread = None

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
            xbmcgui.Window(10000).setProperty('Temp1','1')
            power12 = GPIO.input(POWER_12_PIN) # low active
            power5  = GPIO.input(POWER_5_PIN)  # low active
            if power12 and power5 and not self.ignore_power: # both power removed!
                self.power_removed()
            elif not power12 or not power5:
                self.power_active()

            self.check_rearcam()

            time.sleep(0.01)

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

    def start_rearcam(self):
        #xbmc.executebuiltin("RunScript(plugin.program.rearcam)")
        xbmc.executebuiltin("XBMC.RunScript(/home/pi/carpc/raspi/plugin.program.rearcam/addon.py)")
        self.log("rearcam plugin finished")

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
            os.system("killall mplayer")
            self.rearcam_thread.join()
            self.rearcam_thread = None
            GPIO.output(REARCAM_OUTPUT_PIN, 0)
            self.rearcam_active = False
            time.sleep(1)
            return
        
        elif not state: # start cam
            self.log("i need to start the cam!")
            GPIO.output(REARCAM_OUTPUT_PIN, 1)
            self.rearcam_thread = Thread(target=self.start_rearcam)
            self.rearcam_thread.setDaemon(True)
            self.rearcam_thread.start()
            self.rearcam_active = time.time()
            time.sleep(1)
            return 
            
        else: # nothing to do, return
            return 

        self.log("this should never happen!")

    def power_active(self):
        self.ignore_power = False
        if self.power_dialog:
            self.power_dialog.close()
            self.power_dialog = None

    def power_removed(self):
        """
        power to the system was removed
        """
        if self.power_dialog:
            if self.power_dialog.iscanceled():
                self.ignore_power = True
                self.power_dialog.close()
                self.power_dialog = None
                return

            WAIT_TIME = 30
            diff = time.time() - self.power_time
            p = int((1-(diff / float(WAIT_TIME)))*100)
            if p < 0:
                GPIO.output(POWER_OUT_PIN, 0)
                os.system("sudo shutdown -h now")
            self.power_dialog.update(p)
            return 

        self.log("power triggered")
        self.power_dialog = xbmcgui.DialogProgress()
        self.power_dialog.create(heading="Power removed", line1="Power was removed, system will shutdown in 30s.", line2="Press Cancel to abort shutdown progress")
        self.power_time = time.time()
            
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
