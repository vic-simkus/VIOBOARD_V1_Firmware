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

#include "config.h"

#define __RTCC_H_INT
#include "rtcc.h"
#include "support.h"

#include <xc.h>

#include  <libpic30.h>

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
	/*
	 * The T1 ISR gets called twice a second.
	 */

	STAT_LED_PORT = ~	STAT_LED_PORT;
	ClrWdt();		// Need to rest the WDT every second.
	IFS0bits.T1IF = 0;
	return;
}

void rtcc_init(void)
{
	ClrWdt();

	STAT_LED_PORT = 1;
	ClrWdt();
	__delay_ms(64);

	STAT_LED_PORT = 0;
	ClrWdt();
	__delay_ms(64);

	STAT_LED_PORT = 1;
	ClrWdt();
	__delay_ms(64);

	STAT_LED_PORT = 0;
	ClrWdt();
	__delay_ms(64);

	ClrWdt();

	T1CONbits.TCKPS = 0b11;		//	Timer prescaler 256:1
	PR1 = 31250;				//	Timer period of 0.5 seconds
								//	If this is changed the WDT period must also be changed.

	IFS0bits.T1IF = 0;			// Clear timer interrupt flag.

	IPC0bits.T1IP = 6;			// Set the timer interrupt priority to 1

	IEC0bits.T1IE = 1;			// Enable timer interrupts
	T1CONbits.TON = 1;			// Enable timer

	return;
}