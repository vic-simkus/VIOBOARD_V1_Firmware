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

#include <xc.h>

#include "globals.h"

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

	AD_BUFFER[0] = ADCBUF5;
	AD_BUFFER[1] = ADCBUF4;
	AD_BUFFER[2] = ADCBUF3;
	AD_BUFFER[3] = ADCBUF2;
	AD_BUFFER[4] = ADCBUF6;
	AD_BUFFER[5] = ADCBUF7;
	AD_BUFFER[6] = ADCBUF0;
	AD_BUFFER[7] = ADCBUF1;


	UCHAR i = 0;
	for (i = 0; i < AD_INPUT_NUM; i++)
	{
		AD_BUFFER[i] = AD_BUFFER[i] + ( (working_eeprom_data.l1_cal_data_arr[i] & 0xFF00) >> 8);
		AD_BUFFER[i] = AD_BUFFER[i] - ( working_eeprom_data.l1_cal_data_arr[i] & 0x00FF);
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