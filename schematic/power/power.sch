EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
EELAYER END
$Descr User 8500 4724
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Screw_Terminal_01x03 J?
U 1 1 5A55DCF1
P 1250 3200
F 0 "J?" H 1250 3400 50  0000 C CNN
F 1 "Screw_Terminal_01x03" H 1250 3000 50  0000 C CNN
F 2 "" H 1250 3200 50  0001 C CNN
F 3 "" H 1250 3200 50  0001 C CNN
	1    1250 3200
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x04_Male J?
U 1 1 5A55DE20
P 1250 2350
F 0 "J?" H 1250 2550 50  0000 C CNN
F 1 "Conn_01x04_Male" H 1250 2050 50  0000 C CNN
F 2 "" H 1250 2350 50  0001 C CNN
F 3 "" H 1250 2350 50  0001 C CNN
	1    1250 2350
	1    0    0    -1  
$EndComp
Text GLabel 1550 2250 2    60   Input ~ 0
rpi_running
Text GLabel 1550 2350 2    60   Output ~ 0
ignition_12v
Text GLabel 1550 2550 2    60   Input ~ 0
arduino_request
Text GLabel 1550 2450 2    60   Output ~ 0
5v_available
Wire Wire Line
	1450 2250 1550 2250
Wire Wire Line
	1450 2350 1550 2350
Wire Wire Line
	1450 2450 1550 2450
Wire Wire Line
	1450 2550 1550 2550
Text GLabel 1550 3100 2    60   Input ~ 0
Bat_12v
Text GLabel 1550 3200 2    60   Input ~ 0
ignition_12v
Text GLabel 1550 3300 2    60   Output ~ 0
Vss
Wire Wire Line
	1450 3100 1550 3100
Wire Wire Line
	1450 3200 1550 3200
Wire Wire Line
	1450 3300 1550 3300
Text GLabel 2850 1350 0    60   BiDi ~ 0
arduino_request
Text GLabel 3100 1000 2    60   BiDi ~ 0
5v_available
Wire Wire Line
	2850 1350 3100 1350
Wire Wire Line
	2950 1350 2950 1000
Wire Wire Line
	2950 1000 3100 1000
$Comp
L D D?
U 1 1 5A55E06C
P 3250 1350
F 0 "D?" H 3250 1450 50  0000 C CNN
F 1 "D" H 3250 1250 50  0000 C CNN
F 2 "" H 3250 1350 50  0001 C CNN
F 3 "" H 3250 1350 50  0001 C CNN
	1    3250 1350
	-1   0    0    1   
$EndComp
Connection ~ 2950 1350
$Comp
L D D?
U 1 1 5A55E13B
P 4350 700
F 0 "D?" H 4350 800 50  0000 C CNN
F 1 "D" H 4350 600 50  0000 C CNN
F 2 "" H 4350 700 50  0001 C CNN
F 3 "" H 4350 700 50  0001 C CNN
	1    4350 700 
	1    0    0    -1  
$EndComp
Text GLabel 4600 700  2    60   BiDi ~ 0
rpi_running
$Comp
L D D?
U 1 1 5A55E24B
P 3950 1350
F 0 "D?" H 3950 1450 50  0000 C CNN
F 1 "D" H 3950 1250 50  0000 C CNN
F 2 "" H 3950 1350 50  0001 C CNN
F 3 "" H 3950 1350 50  0001 C CNN
	1    3950 1350
	1    0    0    -1  
$EndComp
$Comp
L CP C?
U 1 1 5A55E258
P 4350 1050
F 0 "C?" H 4375 1150 50  0000 L CNN
F 1 "220u" H 4375 950 50  0000 L CNN
F 2 "" H 4388 900 50  0001 C CNN
F 3 "" H 4350 1050 50  0001 C CNN
	1    4350 1050
	0    -1   -1   0   
$EndComp
Text GLabel 4600 1050 2    60   BiDi ~ 0
Vss
Wire Wire Line
	4500 1050 4600 1050
Wire Wire Line
	4500 700  4600 700 
Wire Wire Line
	4200 700  4100 700 
Wire Wire Line
	4100 700  4100 1350
Wire Wire Line
	4200 1050 4100 1050
Connection ~ 4100 1050
Wire Wire Line
	3400 1350 3800 1350
Wire Wire Line
	3600 1350 3600 2000
Connection ~ 3600 1350
$Comp
L BS107 Q?
U 1 1 5A55E3C8
P 3800 2000
F 0 "Q?" H 4000 2075 50  0000 L CNN
F 1 "BS107" H 4000 2000 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 4000 1925 50  0001 L CIN
F 3 "" H 3800 2000 50  0001 L CNN
	1    3800 2000
	1    0    0    -1  
$EndComp
$Comp
L IRF4905 Q?
U 1 1 5A55E42F
P 4500 1650
F 0 "Q?" H 4750 1725 50  0000 L CNN
F 1 "IRF4905" H 4750 1650 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 4750 1575 50  0001 L CIN
F 3 "" H 4500 1650 50  0001 L CNN
	1    4500 1650
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A55E4D6
P 4300 1800
F 0 "R?" V 4380 1800 50  0000 C CNN
F 1 "10k" V 4300 1800 50  0000 C CNN
F 2 "" V 4230 1800 50  0001 C CNN
F 3 "" H 4300 1800 50  0001 C CNN
	1    4300 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 1950 4750 1950
Wire Wire Line
	4600 1950 4600 1850
Wire Wire Line
	3900 1800 3900 1650
Wire Wire Line
	3900 1650 4300 1650
Text GLabel 4750 1950 2    60   BiDi ~ 0
Bat_12v
Connection ~ 4600 1950
Text GLabel 7100 1000 2    60   BiDi ~ 0
Vdd_12v
$Comp
L IRF4905 Q?
U 1 1 5A55E79C
P 6800 1500
F 0 "Q?" H 7050 1575 50  0000 L CNN
F 1 "IRF4905" H 7050 1500 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 7050 1425 50  0001 L CIN
F 3 "" H 6800 1500 50  0001 L CNN
	1    6800 1500
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A55E7A2
P 6600 1650
F 0 "R?" V 6680 1650 50  0000 C CNN
F 1 "10k" V 6600 1650 50  0000 C CNN
F 2 "" V 6530 1650 50  0001 C CNN
F 3 "" H 6600 1650 50  0001 C CNN
	1    6600 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 1800 7050 1800
Wire Wire Line
	6900 1800 6900 1700
Wire Wire Line
	6200 1500 6600 1500
Connection ~ 6900 1800
$Comp
L D D?
U 1 1 5A55E820
P 5600 1350
F 0 "D?" H 5600 1450 50  0000 C CNN
F 1 "D" H 5600 1250 50  0000 C CNN
F 2 "" H 5600 1350 50  0001 C CNN
F 3 "" H 5600 1350 50  0001 C CNN
	1    5600 1350
	-1   0    0    1   
$EndComp
Wire Wire Line
	4600 1450 4600 1350
Wire Wire Line
	4600 1350 5450 1350
Text GLabel 5200 2500 0    60   BiDi ~ 0
ignition_12v
$Comp
L D D?
U 1 1 5A55E907
P 5600 2500
F 0 "D?" H 5600 2600 50  0000 C CNN
F 1 "D" H 5600 2400 50  0000 C CNN
F 2 "" H 5600 2500 50  0001 C CNN
F 3 "" H 5600 2500 50  0001 C CNN
	1    5600 2500
	-1   0    0    1   
$EndComp
Wire Wire Line
	5200 2500 5450 2500
Wire Wire Line
	5750 1350 5900 1350
Wire Wire Line
	5900 1350 5900 2500
Wire Wire Line
	5900 2500 5750 2500
$Comp
L IRLB8721PBF Q?
U 1 1 5A55EA97
P 6100 1850
F 0 "Q?" H 6350 1925 50  0000 L CNN
F 1 "IRLB8721PBF" H 6350 1850 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 6350 1775 50  0001 L CIN
F 3 "" H 6100 1850 50  0001 L CNN
	1    6100 1850
	1    0    0    -1  
$EndComp
Text GLabel 4050 2300 2    60   BiDi ~ 0
Vss
Wire Wire Line
	4050 2300 3900 2300
Wire Wire Line
	3900 2300 3900 2200
Text GLabel 6300 2200 2    60   BiDi ~ 0
Vss
Wire Wire Line
	6300 2200 6200 2200
Wire Wire Line
	6200 2200 6200 2050
Wire Wire Line
	6200 1650 6200 1500
Wire Wire Line
	6900 1300 6900 1000
Wire Wire Line
	6900 1000 7100 1000
Text GLabel 7050 1800 2    60   BiDi ~ 0
Bat_12v
Text Notes 4500 3950 0    60   ~ 12
Raspi Power Supply Board
Text Notes 5150 4100 0    60   ~ 0
09.01.2018
Text Notes 7600 4100 0    60   ~ 0
1
$EndSCHEMATC
