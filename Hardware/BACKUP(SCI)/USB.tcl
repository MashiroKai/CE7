# Copyright (C) 2020  Intel Corporation. All rights reserved.
# Your use of Intel Corporation's design tools, logic functions 
# and other software and tools, and any partner logic 
# functions, and any output files from any of the foregoing 
# (including device programming or simulation files), and any 
# associated documentation or information are expressly subject 
# to the terms and conditions of the Intel Program License 
# Subscription Agreement, the Intel Quartus Prime License Agreement,
# the Intel FPGA IP License Agreement, or other applicable license
# agreement, including, without limitation, that your use is for
# the sole purpose of programming logic devices manufactured by
# Intel and sold by Intel or its authorized distributors.  Please
# refer to the applicable agreement for further details, at
# https://fpgasoftware.intel.com/eula.

# Quartus Prime Version 20.1.0 Build 711 06/05/2020 SJ Lite Edition
# File: E:\ky_quartus_prj\CMD_SCI\cmdst.tcl
# Generated on: Mon Dec 04 14:41:12 2023

package require ::quartus::project
# Ground Test Board USB
#D0-D7 PIN13 - P20
#C0-C9 PIN21 - PIN33 
#C0 RXF# 
#C1 TXE#
#C2 RD#
#C3 WR#
#C4 SIWU#
#C5 CLKOUT#--XX# 
#C6 OE#--XX#
#C7 PWRSAV#
#C8 XX
#C9 XX
set_location_assignment PIN_A9 -to D[0]
set_location_assignment PIN_A11 -to D[1]
set_location_assignment PIN_A13 -to D[2]
set_location_assignment PIN_B11 -to D[3]
set_location_assignment PIN_A14 -to D[4]
set_location_assignment PIN_B13 -to D[5]
set_location_assignment PIN_B14 -to D[6]
set_location_assignment PIN_B15 -to D[7]
set_location_assignment PIN_P15 -to RXF_N
set_location_assignment PIN_R14 -to TXE_N
set_location_assignment PIN_P12 -to RD_N
set_location_assignment PIN_R11 -to WR_N
set_location_assignment PIN_R9 -to SIWU_N
set_location_assignment PIN_P9 -to CLKOUT
set_location_assignment PIN_P8 -to OE_N
set_location_assignment PIN_J5 -to CLK



