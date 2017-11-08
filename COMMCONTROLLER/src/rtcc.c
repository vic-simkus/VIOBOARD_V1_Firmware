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

volatile UINT timer_clicks;
volatile UINT rts_clicks; // Incremented everytime the T1 interrupt catches the RTS pin low.  Reset when the pin is high.

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
	/*
	 * The T1 ISR gets called twice a second.
	 */

	STAT_LED_PORT = ~	STAT_LED_PORT;

	ClrWdt();		// Need to rest the WDT every second.
	timer_clicks = timer_clicks + 1;
	IFS0bits.T1IF = 0;

	/*
	 * Under certain circumstances the board and the LOGIC_PROCESS get in a bind.
	 * The LOGIC_PROCESS will not lower its RTS line and at the same time the board
	 * will not raise its RTS line.
	 *
	 * So we catch that here.  If our RTS line stays low for more than 4 seconds we
	 * reboot the board.
	 */
	if (PORTBbits.RB9 == 1)	// RTS is low.
	{
		rts_clicks += 1;
	}
	else
	{
		rts_clicks = 0;
	}

	if (rts_clicks >= 8)
	{
		__asm__ volatile ("reset");
	}

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

	timer_clicks = 0;
	rts_clicks = 0;

	T1CONbits.TCKPS = 0b11;		//	Timer prescaler 256:1
	PR1 = 31250;				//	Timer period of 0.5 seconds
	//	If this is changed the WDT period must also be changed.

	IFS0bits.T1IF = 0;			// Clear timer interrupt flag.

	IPC0bits.T1IP = 1;			// Set the timer interrupt priority to 1
	IPC2bits.U1RXIP = 2;		// Set the serial interrupt priority to 2

	IEC0bits.T1IE = 1;			// Enable timer interrupts
	T1CONbits.TON = 1;			// Enable timer

	return;
}

inline UINT rtcc_get_clicks(void)
{
	return timer_clicks;
}

inline UINT rtcc_get_seconds(void)
{
	return timer_clicks / 2;
}
