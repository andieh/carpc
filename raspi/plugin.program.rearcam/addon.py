import sys
import os
import xbmc
import xbmcgui
import xbmcaddon
import threading
import time
import subprocess

addonW = 1280
addonH = 720

SWAP_AXES_THRESHOLD = 10
ACTION_PREVIOUS_MENU = 10
ACTION_SELECT_ITEM = 7
ACTION_BACKSPACE = 92

ACTION_MOUSE_START			= 100
ACTION_MOUSE_LEFT_CLICK     = 100
ACTION_MOUSE_RIGHT_CLICK    = 101
ACTION_MOUSE_MIDDLE_CLICK   = 102
ACTION_MOUSE_DOUBLE_CLICK   = 103
ACTION_MOUSE_WHEEL_UP       = 104
ACTION_MOUSE_WHEEL_DOWN     = 105
ACTION_MOUSE_DRAG           = 106
ACTION_MOUSE_MOVE           = 107
ACTION_MOUSE_END            = 109

TEXT_ALIGN_LEFT = 0
TEXT_ALIGN_RIGHT = 1
TEXT_ALIGN_CENTER_X = 2
TEXT_ALIGN_CENTER_Y = 4
TEXT_ALIGN_RIGHT_CENTER_Y = 5
TEXT_ALIGN_LEFT_CENTER_X_CENTER_Y = 6


###################################################################################################
###################################################################################################
# Global accessed controls
###################################################################################################
###################################################################################################
tempLabel = xbmcgui.ControlLabel(
        addonW/2 - 400, 5,
        800, 40,
        'T',
        textColor='0xffffffff',
        font='WeatherTemp',
        alignment=TEXT_ALIGN_LEFT_CENTER_X_CENTER_Y)



###################################################################################################
###################################################################################################
# Click Event Handler Thread
###################################################################################################
###################################################################################################
class clickHandlerClass(threading.Thread):
    def run(self):
        self.shutdown = False

        while not self.shutdown:
            """
            state = xbmcgui.getMouseAction()
            rawAction = xbmcgui.getMouseRawAction()
            pos = xbmcgui.getMousePosition()

            #tempLabel.setLabel(str(state) + " " + str(rawAction))

            pos_string ="x:%i y:%i" % (pos/10000, pos%10000)

            x_string = "x"
            if rawAction == 5:
            x_string = "xdotool mousemove %i %i mousedown 1&" % (pos/10000, pos%10000)
            if rawAction == 6:
            x_string = "xdotool mousemove %i %i mouseup 1&" % (pos/10000, pos%10000)
            print(x_string)
            os.system(x_string)

            #self.mouse_pos.setLabel(pos_string)
            """
            # Don't kill the CPU
            time.sleep(2)

class RunStream(threading.Thread):
    def setParent(self, parent):
        self.mplayer_pid = None
        self.parent = parent

    def getPid(self):
        return self.mplayer_pid

    def run(self):
        cmd = "/usr/bin/mplayer -vo x11 -fs -nosound tv:// -tv driver=v4l2:norm=NTSC:width=720:height=576:device=/dev/video0:input=0".split()
        proc = subprocess.Popen(cmd)
        self.mplayer_pid = proc.pid
        print("PID: {}".format(self.mplayer_pid))
        proc.wait()
        print("mplayer process finished, going back!")
        self.mplayer_pid = None
        #self.parent.onControl(self.parent.button_back)

class navigation(xbmcgui.WindowDialog):
    def __init__(self):
        os.system("DISPLAY=:0 xdotool mousemove 32 90 click 1&")
        os.system("DISPLAY=:0 xdotool mousemove 2000 2000&")

        # Store original window ID
        self.prevWindowId = xbmcgui.getCurrentWindowId()

        # Go to x11 skin page
        xbmc.executebuiltin("XBMC.ActivateWindow(1114)")

        self.retval=0

        # Background
        self.w = addonW#self.getWidth()
        self.h = addonH#self.getHeight()
        self.mediaPath=os.path.join(addon.getAddonInfo('path'),'resources','media') + '/'

        # Click button
        self.button_click=xbmcgui.ControlButton(0, 0, self.w, self.h, "", self.mediaPath + 'transparent.png', self.mediaPath + 'transparent.png', 0, 0)
        self.addControl(self.button_click)
        self.setFocus(self.button_click)

        # Back button
        self.button_back=xbmcgui.ControlButton(self.w - 50, self.h - 47, 50, 50, "", 'floor_buttonFO.png', 'floor_button.png', 0, 0)
        self.addControl(self.button_back)
        self.backImage = xbmcgui.ControlImage(self.w - 50, self.h - 47, 50, 50, 'icon_back_w.png')
        self.addControl(self.backImage)

        # Add temp label on stage
        #self.addControl(tempLabel)

        # Invisible button used to control focus
        self.buttonfocus = xbmcgui.ControlButton(500, 0, 1, 1, "")
        self.addControl(self.buttonfocus)
        self.setFocus(self.buttonfocus)

        # Start click events thread
        #self.clickHandlerThread = clickHandlerClass()
        #self.clickHandlerThread.start()

        cmd = "DISPLAY=:0 /usr/bin/mplayer -vo x11 -fs -nosound tv:// -tv driver=v4l2:norm=NTSC:width=720:height=576:device=/dev/video0:input=0&"
        os.system(cmd)

    def onControl(self, controlID):
        os.system("DISPLAY=:0 xdotool key q")
        os.system("killall -9 mplayer")

        if controlID == self.button_click:
            pos = xbmcgui.getMousePosition()
            print(pos)
            pos_string ="x:%i y:%i" % (pos/10000, pos%10000)
            print(pos_string)
            x_string = "DISPLAY=:0 xdotool mousemove %i %i click 1&" % (pos/10000, pos%10000)
            print(x_string)
            os.system(x_string)
            self.setFocus(self.buttonfocus)
            os.system("DISPLAY=:0 xdotool mousemove 2000 2000&")
            print("clicked")

        if controlID == self.button_back:
            self.removeControl(self.button_back)
            self.removeControl(self.backImage)
            os.system("DISPLAY=:0 xdotool mousemove 200 200 click 1&")
            os.system("DISPLAY=:0 xdotool mousemove 2000 2000&")
            strWndFnc = "XBMC.ActivateWindow(%i)" % self.prevWindowId
            print(strWndFnc)
            xbmc.executebuiltin(strWndFnc)
            print("done")
            self.setFocus(self.buttonfocus)
            print("set focus")
            # stop the temp thread
            #self.clickHandlerThread.shutdown = True
            #self.clickHandlerThread.join()
            self.close()
            print("closed")

addon = xbmcaddon.Addon(id='plugin.program.navigation')

dialog = navigation()
dialog.doModal()
del dialog
del addon
