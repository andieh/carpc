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
$Descr User 8500 5118
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
P 1050 4350
F 0 "J?" H 1050 4550 50  0000 C CNN
F 1 "Screw_Terminal_01x03" H 1050 4150 50  0000 C CNN
F 2 "" H 1050 4350 50  0001 C CNN
F 3 "" H 1050 4350 50  0001 C CNN
	1    1050 4350
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x04_Male J?
U 1 1 5A55DE20
P 1050 3500
F 0 "J?" H 1050 3700 50  0000 C CNN
F 1 "Conn_01x04_Male" H 1050 3200 50  0000 C CNN
F 2 "" H 1050 3500 50  0001 C CNN
F 3 "" H 1050 3500 50  0001 C CNN
	1    1050 3500
	1    0    0    -1  
$EndComp
Text GLabel 1350 3400 2    60   Input ~ 0
rpi_running
Text GLabel 1350 3500 2    60   Output ~ 0
ignition_12v
Text GLabel 1350 3700 2    60   Input ~ 0
arduino_request
Text GLabel 1350 3600 2    60   Output ~ 0
5v_available
Wire Wire Line
	1250 3400 1350 3400
Wire Wire Line
	1250 3500 1350 3500
Wire Wire Line
	1250 3600 1350 3600
Wire Wire Line
	1250 3700 1350 3700
Text GLabel 1350 4250 2    60   Input ~ 0
Bat_12v
Text GLabel 1350 4350 2    60   Input ~ 0
ignition_12v
Text GLabel 1350 4450 2    60   Output ~ 0
Vss
Wire Wire Line
	1250 4250 1350 4250
Wire Wire Line
	1250 4350 1350 4350
Wire Wire Line
	1250 4450 1350 4450
Text GLabel 750  1600 3    60   BiDi ~ 0
arduino_request
Text GLabel 900  1150 2    60   BiDi ~ 0
5v_available
Wire Wire Line
	750  1150 750  1500
Wire Wire Line
	750  1500 750  1600
Wire Wire Line
	750  1150 900  1150
$Comp
L D D?
U 1 1 5A55E06C
P 1050 1500
F 0 "D?" H 1050 1600 50  0000 C CNN
F 1 "D" H 1050 1400 50  0000 C CNN
F 2 "" H 1050 1500 50  0001 C CNN
F 3 "" H 1050 1500 50  0001 C CNN
	1    1050 1500
	-1   0    0    1   
$EndComp
$Comp
L D D?
U 1 1 5A55E13B
P 2150 850
F 0 "D?" H 2150 950 50  0000 C CNN
F 1 "D" H 2150 750 50  0000 C CNN
F 2 "" H 2150 850 50  0001 C CNN
F 3 "" H 2150 850 50  0001 C CNN
	1    2150 850 
	1    0    0    -1  
$EndComp
Text GLabel 2400 850  2    60   BiDi ~ 0
rpi_running
$Comp
L D D?
U 1 1 5A55E24B
P 1750 1500
F 0 "D?" H 1750 1600 50  0000 C CNN
F 1 "D" H 1750 1400 50  0000 C CNN
F 2 "" H 1750 1500 50  0001 C CNN
F 3 "" H 1750 1500 50  0001 C CNN
	1    1750 1500
	1    0    0    -1  
$EndComp
$Comp
L CP C?
U 1 1 5A55E258
P 2150 1200
F 0 "C?" H 2175 1300 50  0000 L CNN
F 1 "220u" H 2175 1100 50  0000 L CNN
F 2 "" H 2188 1050 50  0001 C CNN
F 3 "" H 2150 1200 50  0001 C CNN
	1    2150 1200
	0    -1   -1   0   
$EndComp
Text GLabel 2400 1200 2    60   BiDi ~ 0
Vss
Wire Wire Line
	2300 1200 2400 1200
Wire Wire Line
	2300 850  2400 850 
Wire Wire Line
	2000 850  1900 850 
Wire Wire Line
	1900 850  1900 1200
Wire Wire Line
	1900 1200 1900 1500
Wire Wire Line
	2000 1200 1900 1200
Connection ~ 1900 1200
Wire Wire Line
	1200 1500 1400 1500
Wire Wire Line
	1400 1500 1600 1500
Wire Wire Line
	1400 1500 1400 2150
Connection ~ 1400 1500
$Comp
L BS107 Q?
U 1 1 5A55E3C8
P 1600 2150
F 0 "Q?" H 1800 2225 50  0000 L CNN
F 1 "BS107" H 1800 2150 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 1800 2075 50  0001 L CIN
F 3 "" H 1600 2150 50  0001 L CNN
	1    1600 2150
	1    0    0    -1  
$EndComp
$Comp
L IRF4905 Q?
U 1 1 5A55E42F
P 2300 1800
F 0 "Q?" H 2550 1875 50  0000 L CNN
F 1 "IRF4905" H 2550 1800 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 2550 1725 50  0001 L CIN
F 3 "" H 2300 1800 50  0001 L CNN
	1    2300 1800
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A55E4D6
P 2100 1950
F 0 "R?" V 2180 1950 50  0000 C CNN
F 1 "10k" V 2100 1950 50  0000 C CNN
F 2 "" V 2030 1950 50  0001 C CNN
F 3 "" H 2100 1950 50  0001 C CNN
	1    2100 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 2100 2400 2100
Wire Wire Line
	2400 2100 2550 2100
Wire Wire Line
	2400 2100 2400 2000
Wire Wire Line
	1700 1950 1700 1800
Wire Wire Line
	1700 1800 2100 1800
Text GLabel 2550 2100 2    60   BiDi ~ 0
Bat_12v
Connection ~ 2400 2100
Text GLabel 4700 1100 1    60   BiDi ~ 0
Vdd_12v
$Comp
L IRF4905 Q?
U 1 1 5A55E79C
P 4600 1650
F 0 "Q?" H 4850 1725 50  0000 L CNN
F 1 "IRF4905" H 4850 1650 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 4850 1575 50  0001 L CIN
F 3 "" H 4600 1650 50  0001 L CNN
	1    4600 1650
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A55E7A2
P 4400 1800
F 0 "R?" V 4480 1800 50  0000 C CNN
F 1 "10k" V 4400 1800 50  0000 C CNN
F 2 "" V 4330 1800 50  0001 C CNN
F 3 "" H 4400 1800 50  0001 C CNN
	1    4400 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 1950 4700 1950
Wire Wire Line
	4700 1950 4850 1950
Wire Wire Line
	4700 1950 4700 1850
Wire Wire Line
	4000 1650 4400 1650
Connection ~ 4700 1950
$Comp
L D D?
U 1 1 5A55E820
P 3400 1500
F 0 "D?" H 3400 1600 50  0000 C CNN
F 1 "D" H 3400 1400 50  0000 C CNN
F 2 "" H 3400 1500 50  0001 C CNN
F 3 "" H 3400 1500 50  0001 C CNN
	1    3400 1500
	-1   0    0    1   
$EndComp
Wire Wire Line
	2400 1600 2400 1500
Wire Wire Line
	2400 1500 3250 1500
Text GLabel 3000 2650 0    60   BiDi ~ 0
ignition_12v
$Comp
L D D?
U 1 1 5A55E907
P 3400 2650
F 0 "D?" H 3400 2750 50  0000 C CNN
F 1 "D" H 3400 2550 50  0000 C CNN
F 2 "" H 3400 2650 50  0001 C CNN
F 3 "" H 3400 2650 50  0001 C CNN
	1    3400 2650
	-1   0    0    1   
$EndComp
Wire Wire Line
	3000 2650 3250 2650
Wire Wire Line
	3550 1500 3700 1500
Wire Wire Line
	3700 1500 3700 2650
Wire Wire Line
	3700 2650 3550 2650
$Comp
L IRLB8721PBF Q?
U 1 1 5A55EA97
P 3900 2000
F 0 "Q?" H 4150 2075 50  0000 L CNN
F 1 "IRLB8721PBF" H 4150 2000 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 4150 1925 50  0001 L CIN
F 3 "" H 3900 2000 50  0001 L CNN
	1    3900 2000
	1    0    0    -1  
$EndComp
Text GLabel 1850 2450 2    60   BiDi ~ 0
Vss
Wire Wire Line
	1850 2450 1700 2450
Wire Wire Line
	1700 2450 1700 2350
Text GLabel 4100 2350 2    60   BiDi ~ 0
Vss
Wire Wire Line
	4100 2350 4000 2350
Wire Wire Line
	4000 2350 4000 2200
Wire Wire Line
	4000 1800 4000 1650
Wire Wire Line
	4700 1100 4700 1450
Text GLabel 4850 2050 3    60   BiDi ~ 0
Bat_12v
Text Notes 4550 4350 0    60   ~ 12
Raspi Power Supply Board
Text Notes 5200 4500 0    60   ~ 0
09.01.2018
Text Notes 7650 4500 0    60   ~ 0
1
$Comp
L Conn_01x02_Male J?
U 1 1 5A560535
P 6250 1350
F 0 "J?" H 6250 1450 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6250 1150 50  0000 C CNN
F 2 "" H 6250 1350 50  0001 C CNN
F 3 "" H 6250 1350 50  0001 C CNN
	1    6250 1350
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x02_Male J?
U 1 1 5A5606C6
P 6250 950
F 0 "J?" H 6250 1050 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6250 750 50  0000 C CNN
F 2 "" H 6250 950 50  0001 C CNN
F 3 "" H 6250 950 50  0001 C CNN
	1    6250 950 
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x02_Male J?
U 1 1 5A5607A4
P 6900 850
F 0 "J?" H 6900 950 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6900 650 50  0000 C CNN
F 2 "" H 6900 850 50  0001 C CNN
F 3 "" H 6900 850 50  0001 C CNN
	1    6900 850 
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x02_Male J?
U 1 1 5A5607AA
P 6900 1250
F 0 "J?" H 6900 1350 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6900 1050 50  0000 C CNN
F 2 "" H 6900 1250 50  0001 C CNN
F 3 "" H 6900 1250 50  0001 C CNN
	1    6900 1250
	1    0    0    -1  
$EndComp
Text Notes 7800 1450 1    60   ~ 0
Buck Converter\nMP1584
Text GLabel 5900 1300 0    60   BiDi ~ 0
Vss
Text GLabel 7250 1300 2    60   BiDi ~ 0
Vss
Wire Wire Line
	5900 1300 6000 1300
Wire Wire Line
	6000 1250 6000 1300
Wire Wire Line
	6000 1300 6000 1350
Wire Wire Line
	6000 1250 6050 1250
Wire Wire Line
	6000 1350 6050 1350
Connection ~ 6000 1300
Wire Wire Line
	7100 1250 7150 1250
Wire Wire Line
	7150 1250 7150 1300
Wire Wire Line
	7150 1300 7150 1350
Wire Wire Line
	7150 1350 7100 1350
Wire Wire Line
	7150 1300 7250 1300
Connection ~ 7150 1300
Text GLabel 7200 900  2    60   BiDi ~ 0
5v_rpi
Text GLabel 5900 900  0    60   BiDi ~ 0
Vdd_12v
Wire Wire Line
	5900 900  5950 900 
Wire Wire Line
	5950 850  5950 900 
Wire Wire Line
	5950 900  5950 950 
Wire Wire Line
	5950 850  6050 850 
Wire Wire Line
	5950 950  6050 950 
Connection ~ 5950 900 
Wire Wire Line
	7100 850  7150 850 
Wire Wire Line
	7150 850  7150 900 
Wire Wire Line
	7150 900  7150 950 
Wire Wire Line
	7150 900  7200 900 
Wire Wire Line
	7150 950  7100 950 
Connection ~ 7150 900 
Wire Wire Line
	4850 1950 4850 2050
Wire Wire Line
	750  1500 900  1500
Connection ~ 750  1500
Wire Notes Line
	5350 700  5350 1500
Wire Notes Line
	5350 1500 7850 1500
Wire Notes Line
	5350 700  7850 700 
$Comp
L Conn_01x02_Male J?
U 1 1 5A561559
P 6250 2200
F 0 "J?" H 6250 2300 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6250 2000 50  0000 C CNN
F 2 "" H 6250 2200 50  0001 C CNN
F 3 "" H 6250 2200 50  0001 C CNN
	1    6250 2200
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x02_Male J?
U 1 1 5A56155F
P 6250 1800
F 0 "J?" H 6250 1900 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6250 1600 50  0000 C CNN
F 2 "" H 6250 1800 50  0001 C CNN
F 3 "" H 6250 1800 50  0001 C CNN
	1    6250 1800
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x02_Male J?
U 1 1 5A561565
P 6900 1700
F 0 "J?" H 6900 1800 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6900 1500 50  0000 C CNN
F 2 "" H 6900 1700 50  0001 C CNN
F 3 "" H 6900 1700 50  0001 C CNN
	1    6900 1700
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x02_Male J?
U 1 1 5A56156B
P 6900 2100
F 0 "J?" H 6900 2200 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6900 1900 50  0000 C CNN
F 2 "" H 6900 2100 50  0001 C CNN
F 3 "" H 6900 2100 50  0001 C CNN
	1    6900 2100
	1    0    0    -1  
$EndComp
Text GLabel 5900 2150 0    60   BiDi ~ 0
Vss
Text GLabel 7250 2150 2    60   BiDi ~ 0
Vss
Wire Wire Line
	5900 2150 6000 2150
Wire Wire Line
	6000 2100 6000 2150
Wire Wire Line
	6000 2150 6000 2200
Wire Wire Line
	6000 2100 6050 2100
Wire Wire Line
	6000 2200 6050 2200
Connection ~ 6000 2150
Wire Wire Line
	7100 2100 7150 2100
Wire Wire Line
	7150 2100 7150 2150
Wire Wire Line
	7150 2150 7150 2200
Wire Wire Line
	7150 2200 7100 2200
Wire Wire Line
	7150 2150 7250 2150
Connection ~ 7150 2150
Text GLabel 7200 1750 2    60   BiDi ~ 0
5v_display
Text GLabel 5900 1750 0    60   BiDi ~ 0
Vdd_12v
Wire Wire Line
	5900 1750 5950 1750
Wire Wire Line
	5950 1700 5950 1750
Wire Wire Line
	5950 1750 5950 1800
Wire Wire Line
	5950 1700 6050 1700
Wire Wire Line
	5950 1800 6050 1800
Connection ~ 5950 1750
Wire Wire Line
	7100 1700 7150 1700
Wire Wire Line
	7150 1700 7150 1750
Wire Wire Line
	7150 1750 7150 1800
Wire Wire Line
	7150 1750 7200 1750
Wire Wire Line
	7150 1800 7100 1800
Connection ~ 7150 1750
Wire Notes Line
	5350 1550 5350 2350
Wire Notes Line
	5350 2350 7600 2350
Wire Notes Line
	5350 1550 7850 1550
$Comp
L Conn_01x02_Male J?
U 1 1 5A561932
P 6250 3050
F 0 "J?" H 6250 3150 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6250 2850 50  0000 C CNN
F 2 "" H 6250 3050 50  0001 C CNN
F 3 "" H 6250 3050 50  0001 C CNN
	1    6250 3050
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x02_Male J?
U 1 1 5A561938
P 6250 2650
F 0 "J?" H 6250 2750 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6250 2450 50  0000 C CNN
F 2 "" H 6250 2650 50  0001 C CNN
F 3 "" H 6250 2650 50  0001 C CNN
	1    6250 2650
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x02_Male J?
U 1 1 5A56193E
P 6900 2550
F 0 "J?" H 6900 2650 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6900 2350 50  0000 C CNN
F 2 "" H 6900 2550 50  0001 C CNN
F 3 "" H 6900 2550 50  0001 C CNN
	1    6900 2550
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x02_Male J?
U 1 1 5A561944
P 6900 2950
F 0 "J?" H 6900 3050 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6900 2750 50  0000 C CNN
F 2 "" H 6900 2950 50  0001 C CNN
F 3 "" H 6900 2950 50  0001 C CNN
	1    6900 2950
	1    0    0    -1  
$EndComp
Text GLabel 5900 3000 0    60   BiDi ~ 0
Vss
Text GLabel 7250 3000 2    60   BiDi ~ 0
Vss
Wire Wire Line
	5900 3000 6000 3000
Wire Wire Line
	6000 2950 6000 3000
Wire Wire Line
	6000 3000 6000 3050
Wire Wire Line
	6000 2950 6050 2950
Wire Wire Line
	6000 3050 6050 3050
Connection ~ 6000 3000
Wire Wire Line
	7100 2950 7150 2950
Wire Wire Line
	7150 2950 7150 3000
Wire Wire Line
	7150 3000 7150 3050
Wire Wire Line
	7150 3050 7100 3050
Wire Wire Line
	7150 3000 7250 3000
Connection ~ 7150 3000
Text GLabel 7200 2600 2    60   BiDi ~ 0
5v_rpi
Text GLabel 5900 2600 0    60   BiDi ~ 0
Vdd_12v
Wire Wire Line
	5900 2600 5950 2600
Wire Wire Line
	5950 2550 5950 2600
Wire Wire Line
	5950 2600 5950 2650
Wire Wire Line
	5950 2550 6050 2550
Wire Wire Line
	5950 2650 6050 2650
Connection ~ 5950 2600
Wire Wire Line
	7100 2550 7150 2550
Wire Wire Line
	7150 2550 7150 2600
Wire Wire Line
	7150 2600 7150 2650
Wire Wire Line
	7150 2600 7200 2600
Wire Wire Line
	7150 2650 7100 2650
Connection ~ 7150 2600
Wire Notes Line
	5350 2400 5350 3200
Wire Notes Line
	5350 3200 7850 3200
Wire Notes Line
	7600 2400 5350 2400
Wire Notes Line
	7850 700  7850 1500
Wire Notes Line
	7850 1550 7850 2350
Wire Notes Line
	7850 2350 7550 2350
Wire Notes Line
	7650 2400 7850 2400
Wire Notes Line
	7850 2400 7850 3200
Text Notes 7800 2300 1    60   ~ 0
Buck Converter\nMP1584
Text Notes 7800 3150 1    60   ~ 0
Buck Converter\nMP1584
$Comp
L Screw_Terminal_01x02 J?
U 1 1 5A5624A3
P 3200 3150
F 0 "J?" H 3200 3250 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 3200 2950 50  0000 C CNN
F 2 "" H 3200 3150 50  0001 C CNN
F 3 "" H 3200 3150 50  0001 C CNN
	1    3200 3150
	1    0    0    -1  
$EndComp
Text GLabel 2850 3250 0    60   BiDi ~ 0
Vss
Text GLabel 2850 3150 0    60   BiDi ~ 0
5v_rpi
Wire Wire Line
	2850 3150 3000 3150
Wire Wire Line
	2850 3250 3000 3250
$Comp
L Screw_Terminal_01x02 J?
U 1 1 5A5627BE
P 3200 4050
F 0 "J?" H 3200 4150 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 3200 3850 50  0000 C CNN
F 2 "" H 3200 4050 50  0001 C CNN
F 3 "" H 3200 4050 50  0001 C CNN
	1    3200 4050
	1    0    0    -1  
$EndComp
Text GLabel 2850 4150 0    60   BiDi ~ 0
5v_hub
Text GLabel 2850 4050 0    60   BiDi ~ 0
Vss
Wire Wire Line
	2850 4050 3000 4050
Wire Wire Line
	2850 4150 3000 4150
$Comp
L Screw_Terminal_01x02 J?
U 1 1 5A562967
P 3200 3600
F 0 "J?" H 3200 3700 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 3200 3400 50  0000 C CNN
F 2 "" H 3200 3600 50  0001 C CNN
F 3 "" H 3200 3600 50  0001 C CNN
	1    3200 3600
	1    0    0    -1  
$EndComp
Text GLabel 2850 3750 0    60   BiDi ~ 0
5v_display
Text GLabel 2850 3600 0    60   BiDi ~ 0
Vss
Wire Wire Line
	2850 3600 3000 3600
Wire Wire Line
	2850 3750 3000 3750
$EndSCHEMATC
