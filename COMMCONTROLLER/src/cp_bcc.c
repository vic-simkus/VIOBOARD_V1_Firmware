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
	sn_reset_slave(I2C_ADDR_IO_CTRL);
	__asm__ volatile ("reset");

	// We're never gonna get here, but hey what the hell
	return 1;
}

UCHAR bcc_get_ai_status(void)
{
	UCHAR rc = update_analog_readings();
	UINT i = 0;

	ser_write_char(rc);						// result code
	ser_write_16bit((IOC_AI_COUNT * 2));	// length of payload

	for (i = 0; i < IOC_AI_COUNT; i++)
	{
		ser_write_16bit(IOC_AI_RAW_VALUES[i]);
	}


	return rc;
}

UCHAR bcc_get_do_status(void)
{
	ser_write_char(0x01);					// result code
	ser_write_16bit(0x01);					// length of payload
	ser_write_char(get_digital_ouputs());	// payload

	return 1;								// return success
}

UCHAR bcc_set_do_status(void)
{
	ser_write_char(0x01);					// result code
	ser_write_16bit(0x01);					// length of payload
	ser_write_char(0xff);					// dummy payload

	set_digital_outputs(mg_command_buffer[bin_context.start_index + 1]);

	return 1;		// return success
}

UCHAR bcc_get_pmic_status(void)
{
	ser_write_char(0x01);					// result code
	ser_write_16bit(0x01);					// length of payload
	ser_write_char(get_pmic_status());		// payload
	return 1;								// return success
}

UCHAR bcc_set_pmic_status(void)
{
	ser_write_char(0x01);					// result code
	ser_write_16bit(0x01);					// length of payload
	ser_write_char(0xff);					// dummy payload

	set_pmic_status(mg_command_buffer[bin_context.start_index + 1]);

	return 1;								// return success
}

UCHAR bcc_get_l1_cal_val(void)
{
	return 1;
}

UCHAR bcc_get_l2_cal_val(void)
{
	return 1;
}

UCHAR bcc_set_l1_cal_val(void)
{
	return 1;
}

UCHAR bcc_set_l2_cal_val(void)
{
	return 1;
}

UCHAR bcc_get_boot_count(void)
{
	return 1;
}
