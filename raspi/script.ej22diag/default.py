# -*- coding: utf-8 -*-
# Licence: GPL v.3 http://www.gnu.org/licenses/gpl.html
# This is an XBMC addon for demonstrating the capabilities
# and usage of PyXBMCt framework.

import os
import xbmc
import xbmcaddon
import pyxbmct
import time
from thread import start_new_thread

from ej22 import FakeData

_addon = xbmcaddon.Addon(id = 'script.ej22diag')
ADDON = xbmcaddon.Addon(id = 'script.ej22diag')
_addon_path = _addon.getAddonInfo('path')


# Enable or disable Estuary-based design explicitly
# pyxbmct.skin.estuary = True


class MyAddon(pyxbmct.AddonFullWindow):
    def __init__(self, title=''):
        super(MyAddon, self).__init__(title)
        self.setGeometry(1280, 720, 10, 5)
        self.init_controls()
        self.connection = FakeData()
        self.connection.start()
        
        self.ignoreContinuousFields = ["romId"]
        self.ignoreBinaryFields = ["testMode", "memoryMode", "park", "california", "acSwitch", "acActive"]
        # Connect a key action (Backspace) to close the window.
        self.connect(pyxbmct.ACTION_NAV_BACK, self.close)
        start_new_thread(self.update_labels, ())

    def init_controls(self):
        self.placeControl(pyxbmct.Label('Continuous Parameters', alignment=pyxbmct.ALIGN_CENTER), 0, 0, 1, 4)
        
        volt = pyxbmct.Label('Voltage', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(volt, 1, 0)
        self.voltage = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.voltage, 1, 1)
        self.placeControl(pyxbmct.Label('Vehicle speed', alignment=pyxbmct.ALIGN_LEFT), 2, 0)
        self.groundSpeed = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.groundSpeed, 2, 1)
        self.placeControl(pyxbmct.Label('Engine speed', alignment=pyxbmct.ALIGN_LEFT), 3, 0)
        self.engineSpeed = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.engineSpeed, 3, 1)
        self.placeControl(pyxbmct.Label('Coolant temperature', alignment=pyxbmct.ALIGN_LEFT), 4, 0)
        self.coolantTemp = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.coolantTemp, 4, 1)
        self.placeControl(pyxbmct.Label('Ignition timing', alignment=pyxbmct.ALIGN_LEFT), 5, 0)
        self.timing = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.timing, 5, 1)
        self.placeControl(pyxbmct.Label('Airflow signal', alignment=pyxbmct.ALIGN_LEFT), 6, 0)
        self.airflow = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.airflow, 6, 1)
        self.placeControl(pyxbmct.Label('Load', alignment=pyxbmct.ALIGN_LEFT), 7, 0)
        self.load = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.load, 7, 1)
        self.placeControl(pyxbmct.Label('Throttle position signal', alignment=pyxbmct.ALIGN_LEFT), 8, 0)
        self.throttle = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.throttle, 8, 1)
        self.placeControl(pyxbmct.Label('Zeroed throttle position', alignment=pyxbmct.ALIGN_LEFT), 9, 0)
        self.zeroedThrottle = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.zeroedThrottle, 9, 1)
        
        self.placeControl(pyxbmct.Label('Injector pulse width', alignment=pyxbmct.ALIGN_LEFT), 1, 2)
        self.injectionWidth = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.injectionWidth, 1, 3)
        self.placeControl(pyxbmct.Label('IAC valve duty cycle', alignment=pyxbmct.ALIGN_LEFT), 2, 2)
        self.dutyCycle = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.dutyCycle, 2, 3)
        self.placeControl(pyxbmct.Label('Oxygen sensor signal', alignment=pyxbmct.ALIGN_LEFT), 3, 2)
        self.oxygen = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.oxygen, 3, 3)
        self.placeControl(pyxbmct.Label('Fuel trim', alignment=pyxbmct.ALIGN_LEFT), 4, 2)
        self.fuelTrim = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.fuelTrim, 4, 3)
        self.placeControl(pyxbmct.Label('Timing correction', alignment=pyxbmct.ALIGN_LEFT), 5, 2)
        self.timingCorrection = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.timingCorrection, 5, 3)
        self.placeControl(pyxbmct.Label('Boost control duty cycle', alignment=pyxbmct.ALIGN_LEFT), 6, 2)
        self.boostControl = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.boostControl, 6, 3)
        self.placeControl(pyxbmct.Label('Barometric pressure', alignment=pyxbmct.ALIGN_LEFT), 7, 2)
        self.barPressure = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.barPressure, 7, 3)
        self.placeControl(pyxbmct.Label('Boost/vacuum', alignment=pyxbmct.ALIGN_LEFT), 8, 2)
        self.boost = pyxbmct.Label('0.0', alignment=pyxbmct.ALIGN_LEFT)
        self.placeControl(self.boost, 8, 3)
    
        self.placeControl(pyxbmct.Label('Binary Parameters', alignment=pyxbmct.ALIGN_CENTER), 0, 4, 1, 1)
        self.ignition = pyxbmct.RadioButton('Ignition')
        self.placeControl(self.ignition, 1, 4)
        self.transmissionMode = pyxbmct.RadioButton('Automatic transmission mode')
        self.placeControl(self.transmissionMode, 2, 4)
        #Test mode
        #Read memory mode
        self.neutral = pyxbmct.RadioButton('Neutral switch')
        self.placeControl(self.neutral, 3, 4)
        #Park switch   
        #California mode
        self.idle = pyxbmct.RadioButton('Idle switch')
        self.placeControl(self.idle, 4, 4)
        #Air conditioning switch
        #Air conditioning relay active
        self.fanActive= pyxbmct.RadioButton('Radiator fan relay active')
        self.placeControl(self.fanActive, 5, 4)
        self.fuelActive = pyxbmct.RadioButton('Fuel pump relay active')
        self.placeControl(self.fuelActive, 6, 4)
        self.canActive = pyxbmct.RadioButton('Canister purge valve active')
        self.placeControl(self.canActive, 7, 4)
        self.pingingActive = pyxbmct.RadioButton('Pinging detected')
        self.placeControl(self.pingingActive, 8, 4)
        self.pressureMeas = pyxbmct.RadioButton('Pressure exchange')
        self.placeControl(self.pressureMeas, 9, 4)
    
        # Button
        self.button = pyxbmct.Button('Close')
        self.placeControl(self.button, 9, 2)
        # Connect control to close the window.
        self.connect(self.button, self.close)

        """
        # Demo for PyXBMCt UI controls.
        no_int_label = pyxbmct.Label('Information output', alignment=pyxbmct.ALIGN_CENTER)
        self.placeControl(no_int_label, 0, 0, 1, 2)
        #
        label_label = pyxbmct.Label('Label')
        self.placeControl(label_label, 1, 0)
        # Label
        self.label = pyxbmct.Label('Simple label')
        self.placeControl(self.label, 1, 1)
        #
        fadelabel_label = pyxbmct.Label('FadeLabel')
        self.placeControl(fadelabel_label, 2, 0)
        # FadeLabel
        self.fade_label = pyxbmct.FadeLabel()
        self.placeControl(self.fade_label, 2, 1)
        self.fade_label.addLabel('Very long string can be here.')
        #
        textbox_label = pyxbmct.Label('TextBox')
        self.placeControl(textbox_label, 3, 0)
        # TextBox
        self.textbox = pyxbmct.TextBox()
        self.placeControl(self.textbox, 3, 1, 2, 1)
        self.textbox.setText('It can display long text.\n'
                             'Lorem ipsum dolor sit amet, consectetur adipiscing elit.')
        # Set auto-scrolling for long TexBox contents
        self.textbox.autoScroll(1000, 1000, 1000)
        #
        image_label = pyxbmct.Label('Image')
        self.placeControl(image_label, 5, 0)
        # Image
        self.image = pyxbmct.Image(os.path.join(_addon_path, 'bbb-splash.jpg'))
        self.placeControl(self.image, 5, 1, 2, 1)
        """
        pass

    def set_active_controls(self):
        """int_label = pyxbmct.Label('Interactive Controls', alignment=pyxbmct.ALIGN_CENTER)
        self.placeControl(int_label, 0, 2, 1, 2)
        #
        radiobutton_label = pyxbmct.Label('RadioButton')
        self.placeControl(radiobutton_label, 1, 2)
        # RadioButton
        self.radiobutton = pyxbmct.RadioButton('Off')
        self.placeControl(self.radiobutton, 1, 3)
        self.connect(self.radiobutton, self.radio_update)
        #
        edit_label = pyxbmct.Label('Edit')
        self.placeControl(edit_label, 2, 2)
        # Edit
        self.edit = pyxbmct.Edit('Edit')
        self.placeControl(self.edit, 2, 3)
        # Additional properties must be changed after (!) displaying a control.
        self.edit.setText('Enter text here')
        #
        list_label = pyxbmct.Label('List')
        self.placeControl(list_label, 3, 2)
        #
        self.list_item_label = pyxbmct.Label('', textColor='0xFF808080')
        self.placeControl(self.list_item_label, 4, 2)
        # List
        self.list = pyxbmct.List()
        self.placeControl(self.list, 3, 3, 3, 1)
        # Add items to the list
        items = ['Item {0}'.format(i) for i in range(1, 8)]
        self.list.addItems(items)
        # Connect the list to a function to display which list item is selected.
        self.connect(self.list, lambda: xbmc.executebuiltin('Notification(Note!,{0} selected.)'.format(
            self.list.getListItem(self.list.getSelectedPosition()).getLabel())))
        # Connect key and mouse events for list navigation feedback.
        self.connectEventList(
            [pyxbmct.ACTION_MOVE_DOWN,
             pyxbmct.ACTION_MOVE_UP,
             pyxbmct.ACTION_MOUSE_WHEEL_DOWN,
             pyxbmct.ACTION_MOUSE_WHEEL_UP,
             pyxbmct.ACTION_MOUSE_MOVE],
            self.list_update)
        # Slider value label
        SLIDER_INIT_VALUE = 25.0
        self.slider_value = pyxbmct.Label(str(SLIDER_INIT_VALUE), alignment=pyxbmct.ALIGN_CENTER)
        self.placeControl(self.slider_value, 6, 3)
        #
        slider_caption = pyxbmct.Label('Slider')
        self.placeControl(slider_caption, 7, 2)
        # Slider
        self.slider = pyxbmct.Slider()
        self.placeControl(self.slider, 7, 3, pad_y=10)
        self.slider.setPercent(SLIDER_INIT_VALUE)
        # Connect key and mouse events for slider update feedback.
        self.connectEventList([pyxbmct.ACTION_MOVE_LEFT,
                               pyxbmct.ACTION_MOVE_RIGHT,
                               pyxbmct.ACTION_MOUSE_DRAG,
                               pyxbmct.ACTION_MOUSE_LEFT_CLICK],
                              self.slider_update)
        #
        button_label = pyxbmct.Label('Button')
        self.placeControl(button_label, 8, 2)"""
        
    def set_navigation(self):
        # Set navigation between controls
        self.button.controlUp(self.slider)
        self.button.controlDown(self.radiobutton)
        self.radiobutton.controlUp(self.button)
        self.radiobutton.controlDown(self.edit)
        self.edit.controlUp(self.radiobutton)
        self.edit.controlDown(self.list)
        self.list.controlUp(self.edit)
        self.list.controlDown(self.slider)
        self.slider.controlUp(self.list)
        self.slider.controlDown(self.button)
        # Set initial focus
        self.setFocus(self.radiobutton)

    def slider_update(self):
        # Update slider value label when the slider nib moves
        try:
            if self.getFocus() == self.slider:
                self.slider_value.setLabel('{:.1F}'.format(self.slider.getPercent()))
        except (RuntimeError, SystemError):
            pass

    def radio_update(self):
        pass
        # Update radiobutton caption on toggle
        if self.radiobutton.isSelected():
            self.radiobutton.setLabel('On')
        else:
            self.radiobutton.setLabel('Off')

    def update_labels(self):
        self.running = True
        while self.running:
            time.sleep(0.1)
            for field in self.connection.continuousFields:
                if field in self.ignoreContinuousFields:
                    continue
                getattr(self, field).setLabel("{}".format(self.connection.get(field)))

            for field in self.connection.binaryFields:
                if field in self.ignoreBinaryFields:
                    continue
                getattr(self, field).setSelected(self.connection.get(field))
                getattr(self, field).setEnabled(self.connection.get(field))
        
    def stop(self):
        window.connection.stop()
        self.running = False
        # maybe wait?

    def list_update(self):
        # Update list_item label when navigating through the list.
        try:
            if self.getFocus() == self.list:
                self.list_item_label.setLabel(self.list.getListItem(self.list.getSelectedPosition()).getLabel())
            else:
                self.list_item_label.setLabel('')
        except (RuntimeError, SystemError):
            pass

    def setAnimation(self, control):
        # Set fade animation for all add-on window controls
        control.setAnimations([('WindowOpen', 'effect=fade start=0 end=100 time=500',),
                                ('WindowClose', 'effect=fade start=100 end=0 time=500',)])


if __name__ == '__main__':
    window = MyAddon('EJ22 Diagnostics')
    window.doModal()
    # Destroy the instance explicitly because
    # underlying xbmcgui classes are not garbage-collected on exit.
    window.stop()
    del window
