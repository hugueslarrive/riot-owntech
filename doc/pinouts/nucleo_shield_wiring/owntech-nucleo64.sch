EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Nucleo shield wiring"
Date "2020-10-22"
Rev ""
Comp "LAAS-CNRS"
Comment1 "OwnTech project"
Comment2 "Author: Hugues Larrive <hugues.larrive@laas.fr>"
Comment3 "Licence: CC BY-SA 4.0"
Comment4 ""
$EndDescr
$Comp
L MCU_Module:NUCLEO64-F334r8 U1
U 1 1 5F92D313
P 3050 4450
F 0 "U1" H 3050 6467 50  0000 C CNN
F 1 "NUCLEO64-F334r8" H 3050 6376 50  0000 C CNN
F 2 "owntech-nucleo64:ST_NUCLEO-F334" V 5000 2900 50  0001 L CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/data_brief/DM00105918.pdf" H 2600 4950 50  0001 C CNN
	1    3050 4450
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:NUCLEO64-F334r8 U1
U 2 1 5F93192F
P 8350 4450
F 0 "U1" H 8350 6467 50  0000 C CNN
F 1 "NUCLEO64-F334r8" H 8350 6376 50  0000 C CNN
F 2 "owntech-nucleo64:ST_NUCLEO-F334" V 10300 2900 50  0001 L CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/data_brief/DM00105918.pdf" H 7900 4950 50  0001 C CNN
	2    8350 4450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 5F94A623
P 5550 2850
F 0 "J2" H 5630 2796 50  0000 L CNN
F 1 "Conn_01x04" H 5630 2751 50  0001 L CNN
F 2 "" H 5550 2850 50  0001 C CNN
F 3 "~" H 5550 2850 50  0001 C CNN
	1    5550 2850
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 5F94AF44
P 5550 3200
F 0 "J3" H 5630 3146 50  0000 L CNN
F 1 "Conn_01x02" H 5630 3101 50  0001 L CNN
F 2 "" H 5550 3200 50  0001 C CNN
F 3 "~" H 5550 3200 50  0001 C CNN
	1    5550 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J1
U 1 1 5F992073
P 4650 2750
F 0 "J1" H 4730 2746 50  0000 L CNN
F 1 "Conn_01x05" H 4730 2701 50  0001 L CNN
F 2 "" H 4650 2750 50  0001 C CNN
F 3 "~" H 4650 2750 50  0001 C CNN
	1    4650 2750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J5
U 1 1 5F9AB1E2
P 6300 2850
F 0 "J5" H 6380 2796 50  0000 L CNN
F 1 "Conn_01x04" H 6380 2751 50  0001 L CNN
F 2 "" H 6300 2850 50  0001 C CNN
F 3 "~" H 6300 2850 50  0001 C CNN
	1    6300 2850
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J7
U 1 1 5F9ABF73
P 7050 2750
F 0 "J7" H 7130 2696 50  0000 L CNN
F 1 "Conn_01x02" H 7130 2651 50  0001 L CNN
F 2 "" H 7050 2750 50  0001 C CNN
F 3 "~" H 7050 2750 50  0001 C CNN
	1    7050 2750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J4
U 1 1 5F9ACD43
P 6300 2400
F 0 "J4" H 6380 2346 50  0000 L CNN
F 1 "Conn_01x04" H 6380 2301 50  0001 L CNN
F 2 "" H 6300 2400 50  0001 C CNN
F 3 "~" H 6300 2400 50  0001 C CNN
	1    6300 2400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 5F9AD75C
P 7050 2300
F 0 "J6" H 7130 2246 50  0000 L CNN
F 1 "Conn_01x02" H 7130 2201 50  0001 L CNN
F 2 "" H 7050 2300 50  0001 C CNN
F 3 "~" H 7050 2300 50  0001 C CNN
	1    7050 2300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J8
U 1 1 5F9B0539
P 7050 4200
F 0 "J8" H 7130 4146 50  0000 L CNN
F 1 "Conn_01x04" H 7130 4101 50  0001 L CNN
F 2 "" H 7050 4200 50  0001 C CNN
F 3 "~" H 7050 4200 50  0001 C CNN
	1    7050 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2550 3900 2550
Wire Wire Line
	3900 2550 3900 4150
Wire Wire Line
	3900 4150 3700 4150
Text Label 4200 2550 0    50   ~ 0
N_GND
Wire Wire Line
	4450 2650 4000 2650
Wire Wire Line
	4000 2650 4000 4850
Wire Wire Line
	4000 4850 7600 4850
Wire Wire Line
	7600 4850 7600 4450
Wire Wire Line
	7600 4450 7700 4450
Text Label 4200 2650 0    50   ~ 0
H2
Wire Wire Line
	4450 2750 4100 2750
Wire Wire Line
	4100 2750 4100 2000
Wire Wire Line
	4100 2000 9250 2000
Wire Wire Line
	9250 2000 9250 3450
Wire Wire Line
	9250 3450 9000 3450
Text Label 4200 2750 0    50   ~ 0
L2
NoConn ~ 4100 2850
Wire Wire Line
	4100 2850 4450 2850
Wire Wire Line
	4100 2950 4450 2950
Text Label 4200 2850 0    50   ~ 0
DGND
Text Label 4200 2950 0    50   ~ 0
+5VD
Text Label 5050 2850 0    50   ~ 0
+3.3VD
Text Label 5050 2950 0    50   ~ 0
DGND
Text Label 5050 3200 0    50   ~ 0
VHgh+
Text Label 5050 3300 0    50   ~ 0
VHgh-
Text Label 5850 2300 0    50   ~ 0
+5VD
Text Label 5850 2500 0    50   ~ 0
DGND
Text Label 6650 2300 0    50   ~ 0
V2+
Text Label 6650 2400 0    50   ~ 0
V2-
Text Label 6650 2750 0    50   ~ 0
V1+
Text Label 6650 2850 0    50   ~ 0
V1-
Text Label 6600 4100 0    50   ~ 0
+5VD
Text Label 6600 4400 0    50   ~ 0
DGND
Text Label 6600 4300 0    50   ~ 0
H1
Text Label 6600 4200 0    50   ~ 0
L1
Text Label 5850 2750 0    50   ~ 0
Tp
Text Label 5850 2850 0    50   ~ 0
IHgh
Text Label 5850 2950 0    50   ~ 0
I2
Text Label 6000 3050 0    50   ~ 0
I1
Wire Wire Line
	6850 4200 6400 4200
Wire Wire Line
	6400 4200 6400 3850
Wire Wire Line
	6400 3850 7700 3850
Wire Wire Line
	6850 4300 6300 4300
Wire Wire Line
	6300 4300 6300 3950
Wire Wire Line
	6300 3950 7700 3950
NoConn ~ 6500 4100
NoConn ~ 6500 4400
Wire Wire Line
	6500 4100 6850 4100
Wire Wire Line
	6500 4400 6850 4400
NoConn ~ 4950 2850
NoConn ~ 4950 2950
Wire Wire Line
	4950 2950 5350 2950
Wire Wire Line
	4950 2850 5350 2850
Wire Wire Line
	5350 2850 5350 2750
Connection ~ 5350 2850
Wire Wire Line
	5350 3050 5350 2950
Connection ~ 5350 2950
Connection ~ 7700 3850
Wire Wire Line
	7700 3850 7850 3850
Connection ~ 7700 3950
Wire Wire Line
	7700 3950 7850 3950
Wire Wire Line
	5350 3200 4850 3200
Wire Wire Line
	4850 3200 4850 2150
Wire Wire Line
	4850 2150 9100 2150
Wire Wire Line
	9100 2150 9100 3050
Wire Wire Line
	9100 3050 9000 3050
Wire Wire Line
	5350 3300 4800 3300
Wire Wire Line
	4800 3300 4800 2100
Wire Wire Line
	4800 2100 9150 2100
Wire Wire Line
	9150 2100 9150 3850
Wire Wire Line
	9150 3850 9000 3850
Wire Wire Line
	9100 3900 9100 3050
Connection ~ 9100 3050
NoConn ~ 5750 2300
NoConn ~ 5750 2500
Wire Wire Line
	5750 2300 6100 2300
Wire Wire Line
	6100 2400 6100 2300
Connection ~ 6100 2300
Wire Wire Line
	5750 2500 6100 2500
Wire Wire Line
	6100 2600 6100 2500
Connection ~ 6100 2500
Wire Wire Line
	6100 3050 5950 3050
Wire Wire Line
	5950 3050 5950 4650
Wire Wire Line
	5950 4650 3700 4650
Wire Wire Line
	6100 2950 5850 2950
Wire Wire Line
	5850 2950 5850 4550
Wire Wire Line
	5850 4550 3700 4550
Wire Wire Line
	5750 2850 5750 5050
Wire Wire Line
	5750 5050 2300 5050
Wire Wire Line
	2300 5050 2300 4550
Wire Wire Line
	2300 4550 2400 4550
Wire Wire Line
	5750 2850 6100 2850
Wire Wire Line
	5650 2750 5650 4950
Wire Wire Line
	5650 4950 2400 4950
Wire Wire Line
	2400 4950 2400 4650
Wire Wire Line
	5650 2750 6100 2750
Wire Wire Line
	6850 2850 6650 2850
Wire Wire Line
	6850 2400 6650 2400
Wire Wire Line
	6650 2850 6650 3350
Wire Wire Line
	6650 3350 7700 3350
Wire Wire Line
	6600 2750 6600 3400
Wire Wire Line
	6600 3600 4100 3600
Wire Wire Line
	4100 3600 4100 4350
Wire Wire Line
	4100 4350 3700 4350
Wire Wire Line
	6600 2750 6850 2750
NoConn ~ 7700 3400
NoConn ~ 9000 3900
Wire Wire Line
	9000 3900 9100 3900
Wire Wire Line
	7700 3400 6600 3400
Connection ~ 6600 3400
Wire Wire Line
	6600 3400 6600 3600
NoConn ~ 3700 4400
Wire Wire Line
	3700 4400 4150 4400
Wire Wire Line
	4150 4400 4150 3650
Wire Wire Line
	4150 3650 6650 3650
Wire Wire Line
	6650 3650 6650 3350
Connection ~ 6650 3350
Wire Wire Line
	6600 2300 6600 2550
Wire Wire Line
	6600 2550 7400 2550
Wire Wire Line
	7400 2550 7400 3450
Wire Wire Line
	7400 3450 7700 3450
Wire Wire Line
	6600 2300 6850 2300
Wire Wire Line
	6650 2400 6650 2600
Wire Wire Line
	6650 2600 7350 2600
Wire Wire Line
	7350 2600 7350 3500
Wire Wire Line
	7350 3500 7700 3500
Wire Wire Line
	7700 3500 7700 3550
Wire Wire Line
	4100 2950 4100 3050
Wire Wire Line
	4100 3050 3700 3050
NoConn ~ 4250 3950
Wire Wire Line
	4250 3950 3700 3950
Text Notes 4200 3900 0    50   ~ 0
You must cut the track
$EndSCHEMATC
