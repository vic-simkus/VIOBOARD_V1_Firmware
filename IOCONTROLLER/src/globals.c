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

#define __GLOBALS_H_INT
#include "globals.h"

eeprom_data_struct eeprom_data;
eeprom_data_struct working_eeprom_data;

const UINT EEPROM_DATA_OFFSETS[EEPROM_SIZE_LINES/2] = {
	EEPROM_START_ADDR + (0x40 * 0x00),
	EEPROM_START_ADDR + (0x40 * 0x01),
	EEPROM_START_ADDR + (0x40 * 0x02),
	EEPROM_START_ADDR + (0x40 * 0x03),
	EEPROM_START_ADDR + (0x40 * 0x04),
	EEPROM_START_ADDR + (0x40 * 0x05),
	EEPROM_START_ADDR + (0x40 * 0x06),
	EEPROM_START_ADDR + (0x40 * 0x07),
	EEPROM_START_ADDR + (0x40 * 0x08),
	EEPROM_START_ADDR + (0x40 * 0x09),
	EEPROM_START_ADDR + (0x40 * 0x0A),
	EEPROM_START_ADDR + (0x40 * 0x0B),
	EEPROM_START_ADDR + (0x40 * 0x0C),
	EEPROM_START_ADDR + (0x40 * 0x0D),
	EEPROM_START_ADDR + (0x40 * 0x0E),
	EEPROM_START_ADDR + (0x40 * 0x0F)
};

void init_globals(void)
{
	UINT i=0;
	for(i=0;i<AD_INPUT_NUM;i++)
	{
		AD_BUFFER[i] = 0;
	}

	confirm_clicks_passed = 0;

	return;
}