# carpc
Digitalization of an old VW T3 Bulli

My old VW bulli needs some fancy power control. Mainly control of lights inside the van, disconnecting 5V power supply and so on. As an Add-on, my van has a "new" engine. The subaru EJ22 is also a boxer engine, but with way more power. And as this engine has some sort of diagnostics (pre-ODB2) included, it would be nice to visualize those informations as well.

so this project consists mainly of 3 parts:
- An Arduino (only for the easy for programming, code is plain C)
- A Raspberry Pi with Touchscreen and CarPC Kodi Interface
- ESP8266 IoT Chip for Wireless control of the functionality

## Atmega328
This is the main part of the projects. It has a lot of buttons, LEDs, Transistors connected and communicates with the EJ22 engine. "Always on", it also functions as a anti thieft module.

## Raspberry Pi
Mainly for music, but also displays diagnostics from the engine, which is implemented as a new addon for kodi. Navigation is included, as well as a rear camera (tbd.)

## ESP8266
Nice small thing to play with. Is directly connected to the Atmega and sends the engine data as well as the state of the system wireless to the raspberry and a mobile phone / tablet whatever via html interface.


details and more to come soon.
