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

void init_globals(void)
{
	mg_sep_buffer_idx = 0;
	mg_cmd_buffer_idx = 0;

	UINT i = 0;

	ioc_step_value = (float) IOC_ADC_MAX_REF / (float) IOC_ADC_STEPS;

	mem_clear(mg_command_buffer, COMMAND_BUFFER_SIZE);
	mem_clear(mg_separator_buffer, COMMAND_SEPARATOR_BUFFER_SIZE);

	for (i = 0; i < IOC_AI_COUNT; i++)
	{
		IOC_AI_RAW_VALUES[i] = 0;
	}

	for (i = 0; i < IOC_AI_COUNT; i++)
	{
		IOC_AI_VOLT_VALUES[i] = 0;
	}
}