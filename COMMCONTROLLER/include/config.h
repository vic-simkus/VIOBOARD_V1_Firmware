/*
Vic's IO Board V1.0 Copyright (C) 2017 Vidas Simkus

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef __CONFIG_H
#define __CONFIG_H

/**
Various configuration parameters.  This file is used by all modules
 */

/*
 * The uC has an 8MHz crystal with a 4x PLL enabled for an Fosc of 32MHz.
 * On the PIC24 the Fcy = Fosc / 2 thus the below
 */
//#define FCY 16000000

#define STAT_LED_TRIS	TRISBbits.TRISB14
#define STAT_LED_PORT	LATBbits.LATB14


/*
 * Make sure that the TERM_WIDTH is evenly divisible by 2.
 * The command buffer and separator buffer are expected to be word aligned
 * so that we can use word operations to manipulate the buffers.
 */
#define TERM_WIDTH	120

/**
 * Incomming command buffer size.
 */
#define COMMAND_BUFFER_SIZE 32

/**
 * Incomming command separator buffer size.
 */
#define COMMAND_SEPARATOR_BUFFER_SIZE COMMAND_BUFFER_SIZE / 4


#endif
