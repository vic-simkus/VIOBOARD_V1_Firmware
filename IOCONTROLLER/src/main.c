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

int main(void)
{
	SRbits.IPL = 0;

	isr_linker_fixup();

	init_globals();

	setup_status_led_pins();
	setup_T1();

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
