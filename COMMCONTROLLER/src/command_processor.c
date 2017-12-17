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
#include "globals.h"
#include "iocontroller_interface.h"
#include "cp_bcc.h"
#include <xc.h>
#include <libpic30.h>

#include <string.h>

static binary_command_callback binary_command_callbacks[BINARY_COMMAND_COUNT];
binary_message_context bin_context;

void init_command_processor(void)
{
	UINT i = 0;
	for (i = 0; i < BINARY_COMMAND_COUNT; i++)
	{
		binary_command_callbacks[i] = 0;
	}

	mem_clear(&bin_context, sizeof (binary_message_context));

	binary_command_callbacks[0x1] = bcc_get_ai_status;
	binary_command_callbacks[0x2] = bcc_get_do_status;
	binary_command_callbacks[0x3] = bcc_get_pmic_status;
	binary_command_callbacks[0x4] = bcc_get_l1_cal_val;
	binary_command_callbacks[0x5] = bcc_get_l2_cal_val;
	binary_command_callbacks[0x6] = bcc_get_boot_count;
	binary_command_callbacks[0x7] = bcc_get_board_status;	// XXX - not implemented

	binary_command_callbacks[0x8] = bcc_set_do_status;
	binary_command_callbacks[0x9] = bcc_set_pmic_status;
	binary_command_callbacks[0xA] = bcc_set_l1_cal_val;
	binary_command_callbacks[0xB] = bcc_set_l2_cal_val;

	binary_command_callbacks[0xC] = bcc_confirm_output_state;
	binary_command_callbacks[0xD] = bcc_start_status_stream;

	binary_command_callbacks[0xE] = bcc_reset;

	return;

}

void process_binary_stream(void)
{
	UINT ci;

	for (ci = 0x01; ci <= 0x07; ci++)
	{
		bin_context.output_buffer_idx = 0;
		BCC_RESP_CLEAR_HEADER();

		BCC_RESP_SET_MARKER();
		BCC_RESP_SET_CI(ci);

		/*
		 * The command processing function will output the rest of the data into the buffer.
		 */
		if (binary_command_callbacks[ci]() != 1)
		{
			serial_protocol_errors.bin_cmd_cb_fail += 1;
			BCC_RESP_SET_RES(0xFF00 & bin_context.output_buffer_w[RESP_MSG_RESP_FIELD_IDX_W]);  // Really make sure that the result code is not 0
		}

		bin_context.output_buffer_w[REP_MSG_PAYLOAD_LEN_IDX_W] += 1;
		bin_context.output_buffer_w[REP_MSG_PAYLOAD_LEN_IDX_W] = bin_context.output_buffer_w[REP_MSG_PAYLOAD_LEN_IDX_W] * 2;
		bin_context.output_buffer_w[BCC_MAKE_W_OFFSET((bin_context.output_buffer_idx - 1))] = 0;	// Clear the checksum field otherwise we get a funny checksum
		UINT chksum = checksum(bin_context.output_buffer_w, bin_context.output_buffer_idx / 2);
		bin_context.output_buffer_w[BCC_MAKE_W_OFFSET((bin_context.output_buffer_idx - 1))] = chksum;

		ser_write_data(bin_context.output_buffer, bin_context.output_buffer_idx);
	}

	ser_flush_buffer();

	return;
}

UCHAR process_binary_command(void)
{
	UINT sys_fail = 0;

	if (bin_context.expected_length < 1)
	{
		serial_protocol_errors.bin_cmd_no_index += 1;
		sys_fail = 1;
		goto _end;
	}

	UCHAR call_index = mg_command_buffer[MG_BUFFER_OFFSET(bin_context.start_index)];

	if (call_index >= BINARY_COMMAND_COUNT || call_index < 0x01)
	{
		serial_protocol_errors.bin_cmd_range += 1;
		sys_fail = 2;
		goto _end;
	}

	BCC_RESP_SET_MARKER();
	BCC_RESP_SET_CI(call_index);

	/*
	 * The command processing function will output the rest of the data into the buffer.
	 */
	if (binary_command_callbacks[call_index]() != 1)
	{
		serial_protocol_errors.bin_cmd_cb_fail += 1;
		BCC_RESP_SET_RES(0xFF00 & bin_context.output_buffer_w[RESP_MSG_RESP_FIELD_IDX_W]);  // Really make sure that the result code is not 0
	}

_end:

	if (sys_fail)
	{
		/*
		 * We end up here if the message length is goofy (<1) or if the call index is out of range.
		 * Basically the only time either one of those would happen is if the message is garbled in transmission.
		 */
		BCC_RESP_CLEAR_HEADER();
		BCC_RESP_SET_MARKER();
		BCC_RESP_SET_CI(0xFF);
		BCC_RESP_SET_RES(0xFF00 | sys_fail);
		BCC_RESP_SET_PAYLOAD_LEN(0x01);
		BCC_RESP_SET_WORD(0, 0xFFFF);
	}

	bin_context.output_buffer_w[REP_MSG_PAYLOAD_LEN_IDX_W] += 1;
	bin_context.output_buffer_w[REP_MSG_PAYLOAD_LEN_IDX_W] = bin_context.output_buffer_w[REP_MSG_PAYLOAD_LEN_IDX_W] * 2;
	bin_context.output_buffer_w[BCC_MAKE_W_OFFSET((bin_context.output_buffer_idx - 1))] = 0;	// Clear the checksum field otherwise we get a funny checksum
	UINT chksum = checksum(bin_context.output_buffer_w, bin_context.output_buffer_idx / 2);
	bin_context.output_buffer_w[BCC_MAKE_W_OFFSET((bin_context.output_buffer_idx - 1))] = chksum;

	ser_write_data(bin_context.output_buffer, bin_context.output_buffer_idx);
	ser_flush_buffer();

	/*
	 * Reset the binary context state
	 */
	bin_context.count = 0;
	bin_context.expected_length = 0;
	bin_context.is_active = 0;
	bin_context.length_msb = 0;
	bin_context.start_index = 0;
	bin_context.output_buffer_idx = 0;

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

