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

#include "support.h"

#include "command_processor.h"

#include "I2C/logger.h"
#include "cp_ic.h"
#include "serial_comm.h"
#include "cp_externs.h"
#include "globals.h"
#include "iocontroller_interface.h"
#include "cp_bcc.h"

#include <string.h>

static binary_command_callback binary_command_callbacks[BINARY_COMMAND_COUNT];
binary_message_context bin_context;

void reset_command_processor_state(void)
{
	mem_clear(mg_command_buffer, mg_cmd_buffer_idx);
	mem_clear(mg_separator_buffer, mg_sep_buffer_idx);

	mg_sep_buffer_idx = 0;
	mg_cmd_buffer_idx = 0;

	reset_bin_context();

	return;
}

void reset_bin_context(void)
{
	mem_clear(&bin_context, sizeof (binary_message_context));
	return;
}

void init_command_processor(void)
{
	UINT i = 0;
	for (i = 0; i < BINARY_COMMAND_COUNT; i++)
	{
		binary_command_callbacks[i] = 0;
	}

	mem_clear(&bin_context, sizeof (binary_message_context));

	/*
	#define IOC_I2C_REG_GET_AI 0x07
	#define IOC_I2C_REG_GET_DO 0x08
	#define IOC_I2C_REG_SET_DO 0x09
	#define IOC_I2C_REG_GET_PMIC 0x0A
	#define IOC_I2C_REG_SET_PMIC 0x0B
	 *
	#define IOC_I2C_REG_GET_L1_CAL 0x0C
	#define IOC_I2C_REG_GET_L2_CAL 0x0D
	#define IOC_I2C_REG_SET_L1_CAL 0x0E
	#define IOC_I2C_REG_SET_L2_CAL 0x0F
	#define IOC_I2C_REG_GET_BOOT_COUNT 0x10
	 */

	binary_command_callbacks[0] = bcc_reset;
	binary_command_callbacks[1] = bcc_get_ai_status;
	binary_command_callbacks[2] = bcc_get_do_status;
	binary_command_callbacks[3] = bcc_set_do_status;
	binary_command_callbacks[4] = bcc_get_pmic_status;
	binary_command_callbacks[5] = bcc_set_pmic_status;
	binary_command_callbacks[6] = bcc_get_l1_cal_val;
	binary_command_callbacks[7] = bcc_get_l2_cal_val;
	binary_command_callbacks[8] = bcc_set_l1_cal_val;
	binary_command_callbacks[9] = bcc_set_l2_cal_val;
	binary_command_callbacks[10] = bcc_get_boot_count;



}

void process_binary_command(void)
{
	/*
	 * mg_command_buffer[0] == @
	 * mg_command_buffer[1] == index of the call.
	 */

	if (bin_context.expected_length < 1)
	{
		logger_error("Invalid binary command; no index.");
		return;
	}

	UCHAR call_index = mg_command_buffer[bin_context.start_index];

	if (call_index >= BINARY_COMMAND_COUNT)
	{
		logger_error("Invalid binary command; index out of range.");
		return;
	}

	ser_write_char(0x10);			// begin binary message
	ser_write_char(call_index);		// the command this message is in response to.

	/*
	 * The command processing function will output the rest of the data into the stream.
	 */
	if (binary_command_callbacks[call_index]() != 1)
	{
		logger_error("Failed to execute command.");
	}

	ser_flush_buffer();

	return;

}

void process_text_command(void)
{
	if (APP_CONFIG_VERBOSE)
	{
		ser_write_string("# processing command\n");
	}

	UCHAR command = 0;
	UINT i = 0;
	UINT i2 = 0;
	UCHAR fail = 0;

	for (; i < mg_cmd_buffer_idx; i++)
	{
		command = mg_command_buffer[i];

		if (command == '?')
		{
			print_help();
			break;
		}
		else if (command == 'S')
		{
			i2 = i + 1;

			if ( i2 < mg_cmd_buffer_idx)
			{
				if (mg_command_buffer[i2] == 'T')
				{
					cmd_print_status();
					break;

				}
				else if (mg_command_buffer[i2] == 'E')
				{
					cmd_set();
					break;
				}
				else
				{
					fail = 1;
					break;
				}
			}
			else
			{
				fail = 1;
				break;
			}
		}
		else if (command == 'C')
		{
			if (!cmd_command())
			{
				fail = 2;
			}
			break;
		}
		else if (command == 'R')
		{
			cmd_reset();
			break;
		}
		else if (command == 'P')
		{
			cmd_ping();
			break;
		}
		else
		{
			fail = 1;
			break;
		}
	}

	if (fail == 1)
	{
		ser_write_string("! Invalid command: ");
		ser_write_string(mg_command_buffer);
		ser_write_char('\n');
	}
	else if (fail == 2)
	{
		ser_write_string("! Command failed\n");
	}

	if (APP_CONFIG_VERBOSE)
	{
		ser_write_string("# end of command process.\n\n");
	}

	ser_flush_buffer();

	return;
}

