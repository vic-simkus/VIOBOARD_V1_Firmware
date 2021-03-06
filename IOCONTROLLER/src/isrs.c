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

#include <xc.h>

#include "globals.h"
#include "ioconfig.h"

void __attribute__((interrupt, no_auto_psv))  	_ADCInterrupt(void)
{
	/*
	 * AD_BUFFER[x] is the analog input on the terminal strip.
	 */


	/*
	TRISBbits.TRISB1 = 1;	//ICD3 -- T7 -- ADBUF0
	TRISBbits.TRISB2 = 1;	//ICD4 -- T8 -- ADBUF1
	TRISBbits.TRISB3 = 1;	//420_4 -- T4 -- ADBUF2
	TRISBbits.TRISB4 = 1;	//420_3 -- T3 -- ADBUF3
	TRISBbits.TRISB5 = 1;	//420_2 -- T2 -- ADBUF4
	TRISBbits.TRISB8 = 1;	//420_1 -- T1 -- ADBUF5
	TRISBbits.TRISB9 = 1;	//ICD1 -- T5 -- ADBUF6
	TRISBbits.TRISB10 = 1;	//ICD2 -- T6 -- ADBUF7
	 */

	AD_BUFFER[0] = ADCBUF5 + AD_CAL_OFFSET_BUFFER[0];
	AD_BUFFER[1] = ADCBUF4 + AD_CAL_OFFSET_BUFFER[1];
	AD_BUFFER[2] = ADCBUF3 + AD_CAL_OFFSET_BUFFER[2];
	AD_BUFFER[3] = ADCBUF2 + AD_CAL_OFFSET_BUFFER[3];
	AD_BUFFER[4] = ADCBUF6 + AD_CAL_OFFSET_BUFFER[4];
	AD_BUFFER[5] = ADCBUF7 + AD_CAL_OFFSET_BUFFER[5];
	AD_BUFFER[6] = ADCBUF0 + AD_CAL_OFFSET_BUFFER[6];
	AD_BUFFER[7] = ADCBUF1 + AD_CAL_OFFSET_BUFFER[7];

	/*
	 * In case of a negative calibration offset we can underflow.
	 * Fix the underflow here.
	 *
	 * We also hand the zero goofiness.  For example if the offset is 5 with
	 * nothing connected to the analog input, the value read out is 5.  Which is weird
	 * since it should be at zero.
	 *
	 * This is probably not the best way to do it, but my issue is with reading being
	 * wrong, not not (double negative!) having a zero.
	 */

	UINT i = 0;
	for (i = 0; i < 8; i++)
	{
		if(AD_BUFFER[i] > 4096 || AD_BUFFER[i] == AD_CAL_OFFSET_BUFFER[i])
		{
			AD_BUFFER[i] = 0;
		}
	}

	IFS0bits.ADIF = 0;

	return;
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
	ClrWdt();

	/*
	 * Flip the blinky light
	 */
	HEARTBEAT_LED ^= 1;

	confirm_clicks_passed += 1;

	if (confirm_clicks_passed >= CONFIRM_CLICKS)
	{
		D01 = 0;
		D02 = 0;
		D03 = 0;
		D04 = 0;

		confirm_clicks_passed = 0;
	}

	/*
	 * Clear the T1 interrupt flag.
	 */
	IFS0bits.T1IF = 0;
	return;
}

void isr_linker_fixup(void)
{
	Nop();
	return;
}