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


/**
 \file
 \author Vidas Simkus (vic.simkus@simkus.com)
 *
 * Various configuration stuffs.
 */

#ifndef CONFIG_H
#define	CONFIG_H

#define I2C_FO		// I2C Fancy Output

#ifdef __dsPIC30F4011
#define BUS_OUT_LED LATFbits.LATF0
#define BUS_IN_LED LATFbits.LATF1
#define AUX_LED LATFbits.LATF4
#define HEARTBEAT_LED LATEbits.LATE0
#else

#define BUS_OUT_LED LATDbits.LATD2
#define BUS_IN_LED LATDbits.LATD9
#define AUX_LED LATCbits.LATC14
#define HEARTBEAT_LED LATDbits.LATD8

#endif

/*
 * Define interrupt priorities.
 */
#define IP_T1	2		// Watchdog and status LED timer.
#define	IP_I2C	5		// I2C
#define IP_ADC	4		// ADC

#define CONFIRM_CLICKS 20


#endif	/* CONFIG_H */

