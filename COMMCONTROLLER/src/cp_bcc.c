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

#include "cp_bcc.h"
#include "iocontroller_interface.h"
#include "serial_comm.h"
#include "globals.h"
#include "command_processor.h"

#include "I2C/simkus_net.h"

extern binary_message_context bin_context;

UCHAR bcc_reset(void)
{
	if (bin_context.count != 0x06)
	{
		return 0;
	}

	UINT idx = bin_context.start_index + 1;	// Skip past the call index.

	if (mg_command_buffer[MG_BUFFER_OFFSET(idx + 0)] != 0xFF)
	{
		return 0;
	}

	if (mg_command_buffer[MG_BUFFER_OFFSET(idx + 1)] != 0xEE)
	{
		return 0;
	}

	if (mg_command_buffer[MG_BUFFER_OFFSET(idx + 2)] != 0xDD)
	{
		return 0;
	}

	if (mg_command_buffer[MG_BUFFER_OFFSET(idx + 3)] != 0xEE)
	{
		return 0;
	}

	if (mg_command_buffer[MG_BUFFER_OFFSET(idx + 4)] != 0xFF)
	{
		return 0;
	}


	sn_reset_slave(I2C_ADDR_IO_CTRL);
	__asm__ volatile ("reset");

	// We're never gonna get here, but hey what the hell
	return 1;
}

UCHAR bcc_get_ai_status(void)
{
	UCHAR rc = update_analog_readings();
	UINT i = 0;

	BCC_RESP_SET_RES(RESP_MSG_SUCCESS_CODE);	//result
	BCC_RESP_SET_PAYLOAD_LEN((IOC_AI_COUNT ));

	for (i = 0; i < IOC_AI_COUNT; i++)
	{
		BCC_RESP_SET_WORD(i, (IOC_AI_RAW_VALUES[i]));
	}

	return rc;
}

UCHAR bcc_get_do_status(void)
{
	BCC_RESP_SET_RES(RESP_MSG_SUCCESS_CODE);	//result
	BCC_RESP_SET_PAYLOAD_LEN(0x01);				// length
	BCC_RESP_SET_WORD(0, get_digital_ouputs());

	return 1;	// return success
}

UCHAR bcc_set_do_status(void)
{
	BCC_RESP_SET_RES(RESP_MSG_SUCCESS_CODE);	//result
	BCC_RESP_SET_PAYLOAD_LEN(0x01);				// length of payload
	BCC_RESP_SET_WORD(0, 0xff);					// dummy payload

	set_digital_outputs(mg_command_buffer[MG_BUFFER_OFFSET(bin_context.start_index + 1)]);

	return 1;		// return success
}

UCHAR bcc_get_pmic_status(void)
{
	BCC_RESP_SET_RES(RESP_MSG_SUCCESS_CODE);	//result
	BCC_RESP_SET_PAYLOAD_LEN(0x01);				// length of payload
	BCC_RESP_SET_WORD(0, get_pmic_status());	// payload

	return 1;	// return success
}

UCHAR bcc_set_pmic_status(void)
{
	BCC_RESP_SET_RES(RESP_MSG_SUCCESS_CODE);	//result
	BCC_RESP_SET_PAYLOAD_LEN(0x01);				// length of payload
	BCC_RESP_SET_WORD(0, 0xff);					// dummy payload

	set_pmic_status(mg_command_buffer[MG_BUFFER_OFFSET(bin_context.start_index + 1)]);

	return 1;	// return success
}

static UCHAR bcc_get_cal_values(UCHAR _cmd)
{
	UCHAR rc = update_cal_values(_cmd);

	if (!rc)
	{
		BCC_RESP_SET_RES(RESP_MSG_FAIL_CODE);	//result
	}
	else
	{
		BCC_RESP_SET_RES(RESP_MSG_SUCCESS_CODE);	//result
	}

	BCC_RESP_SET_PAYLOAD_LEN(IOC_AI_COUNT);

	UCHAR i = 0;
	for (i = 0; i < IOC_AI_COUNT; i++)
	{
		BCC_RESP_SET_WORD(i, IOC_CAL_VALUES[i]);
	}

	return rc;
}

static UCHAR bcc_set_cal_values(UCHAR _cmd)
{
	UCHAR rc = RESP_MSG_SUCCESS_CODE;

	if ((bin_context.count - 1) != (IOC_AI_COUNT * 2))	// subtract one from length for the call index.  Everything else should be 16 bytes of cal data (one word for each analog input.)
	{
		rc = RESP_MSG_FAIL_CODE;
		goto _end;
	}

	UCHAR i = 0, idx_a = 0, idx_b  = 0;

	bin_context.start_index += 1;		// advance past the call i ndex.

	for (i = 0; i < IOC_AI_COUNT; i++)
	{
		idx_a = MG_BUFFER_OFFSET(bin_context.start_index + (i * 2));
		idx_b = MG_BUFFER_OFFSET(bin_context.start_index + (i * 2) + 1);

		IOC_CAL_VALUES[i] = (mg_command_buffer[idx_a] << 8 | mg_command_buffer[idx_b]);
	}

	rc = set_cal_values(_cmd);

_end:

	BCC_RESP_SET_RES(rc);
	BCC_RESP_SET_PAYLOAD_LEN(0x01);
	BCC_RESP_SET_WORD(0, 0xFFFF);

	return rc;
}

UCHAR bcc_confirm_output_state(void)
{
	BCC_RESP_SET_RES(RESP_MSG_SUCCESS_CODE);
	BCC_RESP_SET_PAYLOAD_LEN(0x01);
	BCC_RESP_SET_WORD(0, 0xFFFF);					// dummy payload

	return confirm_output_state();
}

UCHAR bcc_get_l1_cal_val(void)
{
	return bcc_get_cal_values(IOC_I2C_REG_GET_L1_CAL);
}

UCHAR bcc_get_l2_cal_val(void)
{
	return bcc_get_cal_values(IOC_I2C_REG_GET_L2_CAL);
}

UCHAR bcc_set_l1_cal_val(void)
{
	return bcc_set_cal_values(IOC_I2C_REG_SET_L1_CAL);
}

UCHAR bcc_set_l2_cal_val(void)
{
	return bcc_set_cal_values(IOC_I2C_REG_SET_L2_CAL);
}

UCHAR bcc_get_boot_count(void)
{
	BCC_RESP_SET_RES(RESP_MSG_SUCCESS_CODE);
	BCC_RESP_SET_PAYLOAD_LEN(0x01);
	BCC_RESP_SET_WORD(0, get_boot_count());

	return 1;
}

UCHAR bcc_start_status_stream(void)
{
	bin_context.is_stream_active = 1;

	BCC_RESP_SET_RES(RESP_MSG_SUCCESS_CODE);
	BCC_RESP_SET_PAYLOAD_LEN(0x01);
	BCC_RESP_SET_WORD(0, 0xFFFF);					// dummy payload

	return 1;
}

UCHAR bcc_get_board_status(void)
{
	return 0;
}