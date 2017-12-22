#!/bin/bash

DESIRED_BRIGHTNESS=80

# turn on the display
#echo 0 > /sys/class/backlight/rpi_backlight/bl_power
# and to turn it off:
#echo 1 > /sys/class/backlight/rpi_backlight/bl_power
# the brightness can be adjusted using:
#echo 80 > /sys/class/backlight/rpi_backlight/brightness
# loop all possible values
#for b in `seq 10 10 250`; do echo $b; echo $b > /sys/class/backlight/rpi_backlight/brightness; sleep 1; done

while true; do 
    brightness=$(cat /sys/class/backlight/rpi_backlight/brightness)
    if [ "$brightness" -ne "$DESIRED_BRIGHTNESS" ]; then
        echo "set brightness to $DESIRED_BRIGHTNESS"
        echo $DESIRED_BRIGHTNESS > /sys/class/backlight/rpi_backlight/brightness
    #else
    #    echo "nothing to do"
    fi
    sleep 1;
done

