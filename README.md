# Vic's IO Board V1; Firmware

This is the firmware for Vic's IO board V1.  Detailed description of the board can be found here (http://www.simkus.com/home/projects/vioboard_v1).  Design and build diary can be found here (http://www.simkus.com/home/projects/the-home-automation-system).

This repository contains three MAPLAB-X projects:
1. SUPPORT - Stuff common to both the IO controller and the communications controller
1. IOCONTROLLER - IO controller source
1. COMMCONTROLLER - Communications controller source

## SUPPORT
The support project contains the serial, I2C, EEPROM, and other small support functions.

## IOCONTROLLER
The IO controller is a dsPIC30F4013 and handles all of the analog inputs, digital outputs, and PMIC management.  Communication with the communications controller is done via I2C.

## COMMCONTROLLER
The communications controller is a PIC24FV32KA302 and it handles the RS-232 communications with the host device