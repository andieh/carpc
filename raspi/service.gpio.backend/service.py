import os, sys, time
import xbmc, xbmcgui, xbmcaddon

__addon__        = xbmcaddon.Addon()
__addonid__      = __addon__.getAddonInfo('id').decode( 'utf-8' )
__addonname__    = __addon__.getAddonInfo('name').decode("utf-8")

SERVO_PIN = 18
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

    def __init__(self):
        pass

    def start(self):
        monitor = MyMonitor(self.on_settings_changed)
        self.setup()
        monitor.waitForAbort()
        self.cleanup()

    def cleanup(self):
        try:GPIO.remove_event_detect(self.pin)
        except: pass

        GPIO.setmode(GPIO.BCM)
        GPIO.setup(SERVO_PIN, GPIO.OUT)
        self.p = GPIO.PWM(SERVO_PIN, 50) # GPIO 17 als PWM mit 50Hz
        self.p.start(5) # Initialisierung
        self.p.ChangeDutyCycle(12)
        time.sleep(1)
        self.p.stop()

        try: GPIO.cleanup()
        except: pass

    def on_settings_changed(self):
        self.cleanup()
        self.setup()

    def pin_callback(self, pin):
        if self.silent == 'false':
            dialog = xbmcgui.Dialog()
            dialog.notification(__addonname__, 'Executing %s' % self.function, xbmcgui.NOTIFICATION_INFO, 2000)
        time.sleep(0.5)
        xbmc.executebuiltin(self.function)

    def setup(self):
        settings = xbmcaddon.Addon()

        self.pin = int(settings.getSetting("pin"))
        self.edge = int(settings.getSetting("edge"))
        self.function = settings.getSetting("function").strip()
        self.silent = settings.getSetting("silent").strip()

        GPIO.setmode(GPIO.BCM)
        GPIO.setup(SERVO_PIN, GPIO.OUT)
        self.p = GPIO.PWM(SERVO_PIN, 50) # GPIO 17 als PWM mit 50Hz
        self.p.start(5) # Initialisierung
        self.p.ChangeDutyCycle(3)
        time.sleep(1)
        self.p.stop()
        while 1:
            xbmc.log("andieh was here.....")
            break
            time.sleep(1)
            
        if self.pin == 0:
            return

        try:
            GPIO.setmode(GPIO.BOARD)

            if self.edge == 1:
                GPIO.setup(self.pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
                GPIO.add_event_detect(self.pin, GPIO.RISING, callback=self.pin_callback)
            else:
                GPIO.setup(self.pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
                GPIO.add_event_detect(self.pin, GPIO.FALLING, callback=self.pin_callback)
        except:
            dialog = xbmcgui.Dialog()
            dialog.notification(__addonname__, 'Error setting up pin: %s' % self.pin, xbmcgui.NOTIFICATION_ERROR, 2000)

if __name__ == '__main__':
    dialog = xbmcgui.Dialog()
    import RPi.GPIO as GPIO

    #dialog.ok(__addonname__, "Was geht ab?", "alter sack")

    main = Main()
    main.start()
