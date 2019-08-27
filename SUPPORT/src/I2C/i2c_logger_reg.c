/*
Vic's IO Board V1; Firmware

Copyright (C) 2019 Vidas Simkus (vic.simkus@simkus.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * Implements the I2C logger register functionality.
 */

#include <xc.h>
//#include <string.h>
#include <stdlib.h>


#include "I2C/i2c.h"
#include "I2C/i2c_logger_reg.h"
#include "I2C/i2c_config.h"

#include "I2C/logger.h"

/*
 * Two buffers that are flipped by i2c_logger_flush in order to minimize
 * the time that interrupts are disabled.
 */
volatile static UCHAR * i2c_logger_reg_1;
volatile static UCHAR * i2c_logger_reg_2;

/*
 * Active buffer.  Points to either i2c_logger_reg_1 or i2c_logger_reg_2
 */
volatile static UCHAR * i2c_logger_reg_ib;

/*
 * End of message flag.  Set by i2c_logger_reg_w_cb when it encounters an end of
 * message or overflow condition.
 */
volatile static UCHAR i2c_logger_msg_end;

/*
 * We keep track of number of bytes written to the register between buffer switches
 * so that we're not wasting cycles clearing the whole buffer.  We clear only
 * the number of bytes written to it.
 */
volatile static UINT i2c_logger_last_write_size;

/*
 * Work buffer for i2c_logger_flush.  Declared here so that we're not wasting time
 * when the function is called.
 */
static UCHAR * i2c_logger_work_buffer;

/*
 *  A copy of the bytes written so that i2c_logger_last_write_size can be cleared
 * and interrupts re-enabled quicker.
 */
static UINT i2c_logger_prev_write_size;

/*
 * Callback invoked by the ISR
 */
void i2c_logger_reg_w_cb(UCHAR _byte, UINT _idx)
{
	/*
	 * At some point during the previous write an end of message or a buffer
	 * full condition was encountered.  Silently bail.
	 */
	if (i2c_logger_msg_end == 1)
	{
		return;
	}

	/*
	 * Buffer is full.  Silently bail
	 */
	if (_idx == I2C_LOGGER_IBS)
	{
		i2c_logger_msg_end = 1;
		return;
	}

	i2c_logger_reg_ib[_idx] = _byte;
	i2c_logger_last_write_size = _idx;

	/*
	 * Found the end of the message.  Since the log entries are expected to be
	 * text strings, a null byte indicates the end.
	 */
	if (_byte == 0)
	{
		i2c_logger_msg_end = 1;
	}

	return;
}

UCHAR _I2C_LOGGER_REG_1_DB[I2C_LOGGER_IBS];
UCHAR _I2C_LOGGER_REG_2_DB[I2C_LOGGER_IBS];

void i2c_logger_reg_init()
{
	i2c_logger_msg_end = 0;

	i2c_logger_reg_1 = _I2C_LOGGER_REG_1_DB;
	i2c_logger_reg_2 = _I2C_LOGGER_REG_2_DB;

	i2c_logger_last_write_size = 0;
	i2c_logger_prev_write_size = 0;

	i2c_logger_reg_ib = i2c_logger_reg_1;

	//i2c_registers.register_list[i2c_registers.count] = i2c_init_register();
	i2c_registers.register_list[i2c_registers.count].ro = 0;
	i2c_registers.register_list[i2c_registers.count].cb_write = i2c_logger_reg_w_cb;

	i2c_registers.count += 1;

	return;
}

UCHAR i2c_logger_flush(void)
{
	if (I2CSTATbits.S == 1)
	{
		return 0;
	}

	if (i2c_logger_msg_end == 0)
	{
		return 0;
	}

	/*
	 * Disable interrupts.
	 */
	__builtin_disi(0x3FFF);
	{

		/*
		 * Save a pointer to the current buffer, save write count, switch
		 * the buffer pointers, and re-enable interrupts.
		 */
		i2c_logger_work_buffer = (UCHAR *) i2c_logger_reg_ib;
		i2c_logger_prev_write_size = i2c_logger_last_write_size;
		i2c_logger_last_write_size = 0;

		if (i2c_logger_reg_ib == i2c_logger_reg_1)
		{
			i2c_logger_reg_ib = i2c_logger_reg_2;
		}
		else
		{
			i2c_logger_reg_ib = i2c_logger_reg_1;
		}

		i2c_logger_msg_end = 0;
	}
	__builtin_disi(0x0000);

	/*
	 * Dump the log entry to the serial port.
	 */
	logger_log(i2c_logger_work_buffer + 1, i2c_logger_work_buffer[0]);

	/*
	 * Clear the previous buffer.
	 */
	UINT i = 0;
	for (i = 0; i < i2c_logger_prev_write_size; i++)
	{
		i2c_logger_work_buffer[i] = 0;
	}

	return 1;
}
/*
EOF
 */
