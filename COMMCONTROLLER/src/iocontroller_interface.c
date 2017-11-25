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

#include "iocontroller_interface.h"
#include "globals.h"
#include "support.h"

#include "I2C/logger.h"
#include "I2C/i2c.h"
#include "I2C/simkus_net.h"

#include <xc.h>
#include <libpic30.h>

/*
 * Used for reading statuses from the IOCONTROLLER.
 * Analog values are the bulkiest as each input is two bytes.  There are 8 analog inputs for a total of 16 bytes.
 * Then add 1 for the register. And then align it to 16 words, bitches!
 */
#define WORK_BUFFER_SIZE 32

static UCHAR work_buffer[WORK_BUFFER_SIZE];

static UINT make_int(UCHAR _msb, UCHAR _lsb);

UCHAR set_cal_values(UCHAR _cmd)
{
	mem_clear(work_buffer,WORK_BUFFER_SIZE);
	UCHAR ret = 1;

	work_buffer[0] = _cmd;

	UCHAR i = 0;
	for (i = 0; i < IOC_AI_COUNT; i++)
	{
		work_buffer[1 + (i * 2)] = IOC_CAL_VALUES[i];
		work_buffer[1 + (i * 2) + 1]  = (IOC_CAL_VALUES[i] >> 8);
	}

	const i2c_result * i2c_res = sn_write(I2C_ADDR_IO_CTRL, work_buffer, (IOC_AI_COUNT * 2) + 1);
	if (i2c_res->read_error != I2C_ERR_NONE || i2c_res->write_error != I2C_ERR_NONE)
	{
		Nop();
		ret = 0;
		goto _end;
	}


_end:
	return ret;
}

UCHAR set_l1_cal_values(void)
{
	return set_cal_values(IOC_I2C_REG_SET_L1_CAL);
}

UCHAR set_l2_cal_values(void)
{
	return set_cal_values(IOC_I2C_REG_SET_L2_CAL);
}

UCHAR update_cal_values(UCHAR _cmd)
{
	UCHAR ret = 1;

	work_buffer[0] = _cmd;

	/*
	 * We're trying to get L1 cal values for the analog inputs.  Each cal value is a 16 bit word.
	 * We're reading bytes out of the IO controller so total bytes read is IOC_AI_COUNT * 2.
	 */
	const i2c_result * i2c_res = sn_write_read(I2C_ADDR_IO_CTRL, work_buffer, 1, work_buffer, (IOC_AI_COUNT * 2));

	if (i2c_res->read_error != I2C_ERR_NONE || i2c_res->write_error != I2C_ERR_NONE)
	{
		ret = 0;
		goto _end;
	}

	UCHAR i = 0;
	for (i = 0; i < IOC_AI_COUNT; i++)
	{
		UINT val = make_int(work_buffer[(i * 2) + 1], work_buffer[(i * 2)]);
		IOC_CAL_VALUES[i] = val;
	}

_end:
	return ret;
}

UCHAR update_l1_cal_values(void)
{
	return update_cal_values(IOC_I2C_REG_GET_L1_CAL);
}

UCHAR update_l2_cal_values(void)
{
	return update_cal_values(IOC_I2C_REG_GET_L2_CAL);
}

UINT get_boot_count(void)
{
	UINT ret = 1;

	mem_clear((UCHAR *) & work_buffer, WORK_BUFFER_SIZE);

	work_buffer[0] = IOC_I2C_REG_GET_BOOT_COUNT;

	const i2c_result * i2c_res = sn_write_read(I2C_ADDR_IO_CTRL, work_buffer, 1, work_buffer, 2);

	if (i2c_res->read_error != I2C_ERR_NONE || i2c_res->write_error != I2C_ERR_NONE)
	{
		ret = 0;
		goto _end;
	}

	ret = (work_buffer[1] << 8) | work_buffer[0];

_end:
	return ret;
}

UCHAR set_digital_outputs(UCHAR _out)
{
	UCHAR ret = 1;
	mem_clear((UCHAR *) & work_buffer, WORK_BUFFER_SIZE);
	work_buffer[0] = IOC_I2C_REG_SET_DO;
	work_buffer[1] = _out;

	const i2c_result * i2c_res = sn_write(I2C_ADDR_IO_CTRL, work_buffer, 2);

	if (i2c_res->read_error != I2C_ERR_NONE || i2c_res->write_error != I2C_ERR_NONE)
	{
		ret = 0;
	}

	return ret;
}

UCHAR set_pmic_status(UCHAR _out)
{
	UCHAR ret = 1;
	mem_clear((UCHAR *) & work_buffer, WORK_BUFFER_SIZE);
	work_buffer[0] = IOC_I2C_REG_SET_PMIC;
	work_buffer[1] = _out;

	const i2c_result * i2c_res = sn_write(I2C_ADDR_IO_CTRL, work_buffer, 2);

	if (i2c_res->read_error != I2C_ERR_NONE || i2c_res->write_error != I2C_ERR_NONE)
	{
		ret = 0;
	}

	return ret;
}

UCHAR get_digital_ouputs(void)
{
	mem_clear((UCHAR *) & work_buffer, WORK_BUFFER_SIZE);
	work_buffer[0] = IOC_I2C_REG_GET_DO;

	const i2c_result * i2c_res = sn_write_read(I2C_ADDR_IO_CTRL, work_buffer, 1, work_buffer, 1);

	if (i2c_res->read_error != I2C_ERR_NONE || i2c_res->write_error != I2C_ERR_NONE)
	{
		Nop();
	}

	return work_buffer[0];
}

UCHAR get_pmic_status(void)
{
	mem_clear((UCHAR *) & work_buffer, WORK_BUFFER_SIZE);
	work_buffer[0] = IOC_I2C_REG_GET_PMIC;

	const i2c_result * i2c_res = sn_write_read(I2C_ADDR_IO_CTRL, work_buffer, 1, work_buffer, 1);

	if (i2c_res->read_error != I2C_ERR_NONE || i2c_res->write_error != I2C_ERR_NONE)
	{
		Nop();
	}

	return work_buffer[0];
}

UCHAR update_analog_readings(void)
{
	UCHAR ret = 1;
	UCHAR i = 0;

	mem_clear((UCHAR *) & work_buffer, WORK_BUFFER_SIZE);
	work_buffer[0] = IOC_I2C_REG_GET_AI;

	const i2c_result * i2c_res = sn_write_read(I2C_ADDR_IO_CTRL, work_buffer, 1, work_buffer, (IOC_AI_COUNT * 2));

	if (i2c_res->read_error != I2C_ERR_NONE || i2c_res->write_error != I2C_ERR_NONE)
	{
		ret = 0;
		goto __end_func;
	}

	for (i = 0; i < IOC_AI_COUNT; i ++)
	{
		UINT val = make_int(work_buffer[(i * 2) + 1], work_buffer[(i * 2)]);
		//IOC_AI_RAW_VALUES[IOC_IO_MAP[i]] = val;
		//IOC_AI_VOLT_VALUES[IOC_IO_MAP[i]] = (float) val * (float) ioc_step_value;

		IOC_AI_RAW_VALUES[i] = val;
		IOC_AI_VOLT_VALUES[i] = (float) val * (float) ioc_step_value;
	}

__end_func:

	return ret;
}

UCHAR confirm_output_state(void)
{
	UCHAR ret = 1;

	mem_clear((UCHAR *) & work_buffer, WORK_BUFFER_SIZE);
	work_buffer[0] = IOC_I2C_REG_CONFIRM_OUTPUT;
	work_buffer[1] = 0xFF;

	const i2c_result * i2c_res = sn_write(I2C_ADDR_IO_CTRL, work_buffer, 2);

	if (i2c_res->read_error != I2C_ERR_NONE || i2c_res->write_error != I2C_ERR_NONE)
	{
		ret = 0;
	}

	return ret;
}

static UINT make_int(UCHAR _msb, UCHAR _lsb)
{
	UINT ret = _msb;
	ret = ret << 8;
	ret = ret | _lsb;
	return ret;
}
