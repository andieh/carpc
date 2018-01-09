# carpc
Digitalization of an old VW T3 Bulli

My old VW bulli needs some fancy power control. Mainly control of lights inside the van, disconnecting 5V power supply, theft control and so on. As an Add-on, my van has a "new" engine. The subaru EJ22 is also a boxer engine, but with way more power. And as this engine has some sort of diagnostics (pre-ODB2) included, it would be nice to visualize those informations as well.

so this project consists mainly of 3 parts:
- An Arduino (only for the easy for programming, code is plain C) (work in progress)
- A Raspberry Pi with Touchscreen and CarPC Kodi Interface (work in progress)
- ESP8266 IoT Chip for Wireless control of the functionality (t.b.d)

## Atmega328
This is the main part of the projects. Located in the back, this device will be always on. controlled by a 3x2 button interface, it enables the LED stripes in the back, starts the heater, turn on or off the 5V supply and so on. A horn is connected for alarm purposes. It checks the door state, or if the car was locked or not. 

## Raspberry Pi
Mainly for music, but also displays diagnostics from the engine, shows a navigation plugin or shows a rear cam. i currently implemented 3 plugins:
- GPIO service (enable rear cam, power enable, etc).
- Rear cam display: shows an overlay for the connected camera (usb analog s-video grabber and v4l2 / mplayer)
- Display of the EJ22 data (finished, but connection missing)

### Used Components
- Raspberry PI v.3
- Optocoupler ltv847 for detecting 12V / 5V voltages
- Servo for open and closing the display lid 
- Switch for detecting the state of the display lid
- electromagnet for holding the display lid
- Levelshifter TE291 for USART communication between Arduino / Raspbi
- IRLB8721 N-Channel Mosfets for LED stripes (controlled by the arduino)
- IRF9Z P-Channel (and BS107 Transistor) switch the 12V rail for the rear cam
- DHT 22 / AM2302 Temperature sensor 

### Schematics
![rasp_schem](https://github.com/andieh/carpc/blob/master/schematic/raspi.png)

## ESP8266
Nice small thing to play with. Is directly connected to the EJ22 engine and sends data via broadcast. but currently not work in progress. to be implemented later.

Schematics (created with kicad) will be updated on time, details and more to come soon.
