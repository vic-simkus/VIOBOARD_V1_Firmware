/*
Vic's IO Board V1; Firmware

Copyright (C) 2019 Vidas Simkus (vic.simkus@simkus.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


/**
 * \file
 * \author Vidas Simkus (vic.simkus@simkus.com)
 *
 * I/O configurations and definitions.
 */

#ifndef IOCONFIG_H
#define	IOCONFIG_H

#include "config.h"
#include <xc.h>

/*
 * DIGITAL OUTPUT:
 * DO1 -- RF0
 * D02 -- RF1
 * DO3 -- RF3
 * D04 -- RF4
 */

#define D01 LATFbits.LATF1						// Digital output 1
#define D02 LATFbits.LATF0						// Digital output 2
#define D04	LATFbits.LATF6						// Digital output 4
#define D03 LATFbits.LATF4						// Digital output 3

#define D01_CONFIG TRISFbits.TRISF1				// Digital output 1 configuration bit
#define D02_CONFIG TRISFbits.TRISF0				// Digital output 2 configuration bit
#define D04_CONFIG TRISFbits.TRISF6				// Digital output 4 configuration bit
#define D03_CONFIG TRISFbits.TRISF4				// Digital output 3 configuration bit

#define PWR_ENABLE_AI LATCbits.LATC13			// Enable output for analog power PMIC.
#define PWR_ENABLE_DO LATFbits.LATF5			// Enable output for digital output PMIC.

#define PWR_ENABLE_AI_CONFIG TRISCbits.TRISC13	// Configuration register for analog power enable pin.
#define PWR_ENABLE_DO_CONFIG TRISFbits.TRISF5	// Configuration register for digital output enable pin.

#define PWR_FAULT_AI PORTDbits.RD0				// Fault input for analog power PMIC
#define PWR_FAULT_DO PORTDbits.RD1				// Fault input for digital output PMIC

#define PWR_FAULT_AI_LED PORTAbits.RA11
#define PWR_FAULT_DO_LED PORTDbits.RD3

#define PWR_FAULT_AI_CONFIG TRISDbits.TRISD0	// Configuration register for analog power fault input pin
#define PWR_FAULT_DO_CONFIG TRISDbits.TRISD1	// Configuration register for digital output fault input pin

#define PWR_FAULT_AI_LED_CONFIG TRISAbits.TRISA11
#define PWR_FAULT_DO_LED_CONFIG TRISDbits.TRISD3

/*
 * Analog inputs


	0 -- TRISBbits.TRISB1 = 1;	//ICD3	-- T7
	1 -- TRISBbits.TRISB2 = 1;	//ICD4	-- T8
	2 -- TRISBbits.TRISB3 = 1;	//420_4	-- T4
	3 -- TRISBbits.TRISB4 = 1;	//420_3	-- T3
	4 -- TRISBbits.TRISB5 = 1;	//420_2	-- T2
	5 -- TRISBbits.TRISB8 = 1;	//420_1	-- T1
	6 -- TRISBbits.TRISB9 = 1;	//ICD1	-- T5
	7 -- TRISBbits.TRISB10 = 1;	//ICD2	-- T6

 */

#define AD_INPUT_NUM 8

/**
 * Masks for bit manipulations.
 * The DO statuses are sent/received as a single byte.
 * The lower four bits correspond to the outputs.  1 means output is on.  0 means it's off.
 * Layout:
 * 	7		6		5		4		3		2		1		0
 *	-		-		-		-		DO4		DO3		DO2		DO1
 */

#define DO_4_MASK 0x08
#define DO_3_MASK 0x04
#define DO_2_MASK 0x02
#define DO_1_MASK 0x01

/**
 * Masks for bit manipulation
 * The PMIC statuses are sent received as a single byte.
 * Layout:
 * 	7		6		5		4		3		2		1		0
 *	-		-		-		-		P2F		P1F		P2S		P1S
 *
 * P1E = PMIC 1 fault (1 = in fault, 0 = no error)
 * P2E = PMIC 2 fault (1 = in fault, 0 = no error)
 * P1S = PMIC 1 status (1 = enabled, 0 = disabled)
 * P2S = PMIC 2 status (1 = enabled, 0 = disabled)
 *
 * Error flags can only be reset by setting them to zero.  Setting an error flag to 1 has no affect.
 *
 * PMIC 1 == AI PMIC
 * PMIC 2 == DO PMIC
 *
 */

#define PMIC_AI_EN_MASK 0x01
#define PMIC_DO_EN_MASK 0x02
#define PMIC_AI_ERR_MASK 0x04
#define PMIC_DO_ERR_MASK 0x08

#endif	/* IOCONFIG_H */

