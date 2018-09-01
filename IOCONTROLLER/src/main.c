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

#include "chip_config.h"
#include "globals.h"

#include "config.h"
#include "setup.h"
#include "isrs.h"

#include "I2C/i2c.h"
#include "I2C/simkus_net.h"
#include "I2C/logger.h"

#include "ioconfig.h"

#include "support.h"
#include "eeprom_utils.h"

#include <xc.h>
#include <libpic30.h>

#include "eeprom_config.h"

#include <stddef.h>

void fancy_led_startup()
{
	UINT i = 0;

	for (i = 0; i < 3; i++)
	{
		BUS_OUT_LED = 1;
		BUS_IN_LED = 1;
		AUX_LED = 1;

		__delay_ms(200);

		BUS_OUT_LED = 0;
		BUS_IN_LED = 0;
		AUX_LED = 0;

		__delay_ms(200);

	}

	return;
}

//int _EEDATA(2) user_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84.85,86.87,88,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120 };

int main(void)
{
	SRbits.IPL = 0;

	isr_linker_fixup();

	globals_init();

	setup_status_led_pins();

	/*
	 * Heart-beat LED and watchdog timer reset.
	 */
	setup_T1();

	/*
	 * ADC conversion trigger
	 */
	setup_T3();

	setup_do_pins();
	setup_adc();
	setup_pmics();


	/*
	 * We do this here because during board programming there's usually a series
	 * of startups caused by the programming of the commcontroller.
	 *
	 * These resets tend to scramble the EEPROM.
	 *
	 * So we put our eeprom stuff here to introduce a bit of a delay.
	 */

	eeprom_readin_data();

	/*
	 * Initialized eeprom_init value is 0xDEAD.  Uninitialized eeprom value is 0xFFFF
	 */
	if (eeprom_data.eeprom_init == 0xFFFF || eeprom_data.eeprom_init != 0xDEAD)
	{

		eeprom_initialize_data();
	}
	else
	{
		eeprom_data.boot_count += 1;
		eeprom_writeout_data();
	}

	setup_i2c();

	/*
	 * Give the other chip a chance to come up.
	 */
	fancy_led_startup();

	AUX_LED = 1;
	BUS_OUT_LED = 0;
	BUS_IN_LED = 0;

	while (1)
	{
		if (sn_ping(I2C_ADDR_COMM_CTRL))
		{
			AUX_LED = 0;
			break;
		}

		__delay_ms(100);
		AUX_LED = 1;
		__delay_ms(100);
		AUX_LED = 0;
	}

	AUX_LED = 0;

	sn_ship_log(I2C_ADDR_COMM_CTRL, LOGGER_LEVEL_PROTOCOL, (UCHAR*) "F IC. IC UP. CC SU.");

	UCHAR pmic_do_fault_sensed = 0;
	UCHAR pmic_ai_fault_sensed = 0;

	for (; ; )
	{
		if (PWR_FAULT_AI && !pmic_ai_fault_sensed)
		{
			// New trip
			pmic_ai_fault_sensed = 1;
			PWR_FAULT_AI_LED = 1;

			sn_ship_log(I2C_ADDR_COMM_CTRL, LOGGER_LEVEL_PROTOCOL, (UCHAR *) "F IC. PFS AI.");
		}
		else if (pmic_ai_fault_sensed && !PWR_FAULT_AI)
		{
			// Sensed before, but has been cleared.
			pmic_ai_fault_sensed = 0;
			PWR_FAULT_AI_LED = 0;

			sn_ship_log(I2C_ADDR_COMM_CTRL, LOGGER_LEVEL_PROTOCOL, (UCHAR *) "F IC. PFR AI.");
		}

		if (PWR_FAULT_DO && !pmic_do_fault_sensed)
		{
			// New trip
			pmic_do_fault_sensed = 1;
			PWR_FAULT_DO_LED = 1;

			sn_ship_log(I2C_ADDR_COMM_CTRL, LOGGER_LEVEL_PROTOCOL, (UCHAR *) "F IC. PFS DO.");
		}
		else if (pmic_do_fault_sensed && !PWR_FAULT_DO)
		{
			// Sensed before, but has been cleared.
			pmic_do_fault_sensed = 0;
			PWR_FAULT_DO_LED = 0;

			sn_ship_log(I2C_ADDR_COMM_CTRL, LOGGER_LEVEL_PROTOCOL, (UCHAR *) "F IC. PFR DO.");
		}

		if (eeprom_data.is_dirty)
		{
			/*
			 * If calibration data has changed write it out.
			 */

			eeprom_writeout_data();
		}

	}

	return 0;
}
