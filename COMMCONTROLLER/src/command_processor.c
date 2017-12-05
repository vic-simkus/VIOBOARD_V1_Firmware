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
#include <xc.h>
#include <libpic30.h>

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

	binary_command_callbacks[0x0] = bcc_reset;
	binary_command_callbacks[0x1] = bcc_get_ai_status;
	binary_command_callbacks[0x2] = bcc_get_do_status;
	binary_command_callbacks[0x3] = bcc_set_do_status;
	binary_command_callbacks[0x4] = bcc_get_pmic_status;
	binary_command_callbacks[0x5] = bcc_set_pmic_status;
	binary_command_callbacks[0x6] = bcc_get_l1_cal_val;
	binary_command_callbacks[0x7] = bcc_get_l2_cal_val;
	binary_command_callbacks[0x8] = bcc_set_l1_cal_val;
	binary_command_callbacks[0x9] = bcc_set_l2_cal_val;
	binary_command_callbacks[0xA] = bcc_get_boot_count;
	binary_command_callbacks[0xB] = bcc_confirm_output_state; // XXX - this will be the get board stats stuff
	binary_command_callbacks[0xC] = bcc_confirm_output_state;

	return;

}

UCHAR process_binary_command(void)
{
	/*
	 * Disable the UART so that incomming data does not mess with our buffer.
	 */
//	U1MODEbits.UARTEN = 0;

	UCHAR rc = 1;
	if (bin_context.expected_length < 1)
	{
		serial_protocol_errors.bin_cmd_no_index += 1;
		rc = 0;
		goto _end;
	}

	UCHAR call_index = mg_command_buffer[bin_context.start_index];

	if (call_index >= BINARY_COMMAND_COUNT)
	{
		serial_protocol_errors.bin_cmd_range += 1;
		rc = 0;
		goto _end;
	}

	BCC_BUFFER_ADD_BYTE(0x10);			// begin binary message
	BCC_BUFFER_ADD_BYTE(call_index);		// the command this message is in response to.

	/*
	 * The command processing function will output the rest of the data into the stream.
	 */
	if (binary_command_callbacks[call_index]() != 1)
	{
		serial_protocol_errors.bin_cmd_cb_fail += 1;
		rc = 0;
	}

	if(!rc)
	{
		bin_context.output_buffer_idx = 0;
		BCC_BUFFER_ADD_BYTE(0x10);			// begin binary message
		BCC_BUFFER_ADD_BYTE(call_index);	// the command this message is in response to.
		BCC_BUFFER_ADD_WORD(0x00);			// Failure
		BCC_BUFFER_ADD_WORD(0x01);
		BCC_BUFFER_ADD_BYTE(0xFF);
	}

	if((bin_context.output_buffer_idx % 2)	!= 0)
	{
		/*
		 * Pad to even length
		 */
		BCC_BUFFER_ADD_BYTE(0x00);
		bin_context.output_buffer_w[2] += 1;
	}

	BCC_BUFFER_ADD_WORD(0x0);

	bin_context.output_buffer_w[2] += 2;		// Third byte is the buffer length.  We change it here in order to accommodate the alignment fix and checksum

	UINT chksum = checksum(bin_context.output_buffer_w,bin_context.output_buffer_idx/2);
	bin_context.output_buffer_w[BCC_MAKE_W_OFFSET((bin_context.output_buffer_idx-1))] = chksum;

	_end:

	ser_write_data(bin_context.output_buffer,bin_context.output_buffer_idx);
	ser_flush_buffer();

	return 1;
}

void process_text_command(void)
{
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
		ser_write_string("! IC: ");
		ser_write_string(mg_command_buffer);
		ser_write_char('\n');
	}
	else if (fail == 2)
	{
		ser_write_string("! CF\n");
	}

	ser_flush_buffer();

	return;
}

