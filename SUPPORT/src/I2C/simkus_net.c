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

#include "I2C/simkus_net.h"
#include "I2C/i2c_logger_reg.h"
#include "I2C/i2c.h"
#include "I2C/logger.h"

#include "support.h"

#include <string.h>
#include <libpic30.h>

#include <xc.h>

#define INIT_RESULT(_res) sn_result _res;_res.error =0;_res.bytes_written = 0;_res.bytes_read=0

static UCHAR sn_data_buffer[SN_DBS];

static UINT sn_retry_count;
static UINT sn_retry_wait_us;
static i2c_result i2c_res;

#define clear_i2c_result() memset(&i2c_res,0,sizeof(struct __i2c_result))

UCHAR sn_reset_slave_ex(UCHAR _addr, UINT _max_wait )
{
	sn_reset_slave(_addr);

	return sn_ping_ex(_addr, _max_wait);
}

UCHAR sn_ping(UCHAR _addr)
{
	clear_i2c_result();

	sn_data_buffer[0] = I2C_REG_PING;
	sn_write_read(_addr, sn_data_buffer, 1, sn_data_buffer, 1);

	if (i2c_res.read_error == I2C_ERR_NONE && i2c_res.write_error == I2C_ERR_NONE)
	{
		if (sn_data_buffer[0] == 'R')
		{
			return 1;
		}
	}

	return 0;
}

UCHAR sn_ping_ex(UCHAR _addr, UINT _max_wait)
{
	UINT i;

	for (i = 0; i < _max_wait; i++)
	{
		if (sn_ping(_addr) == 1)
		{
			return 1;
		}
	}

	return 0;
}

void sn_reset_slave(UCHAR _addr)
{
	sn_data_buffer[0] = I2C_REG_RESET;
	sn_data_buffer[1] = 0xAA;

	i2c_write_buffer_ex(_addr, sn_data_buffer, 3, 0);

	return;
}

const i2c_result * sn_write(UCHAR _addr, UCHAR * _data, UINT _length)
{
	clear_i2c_result();
	i2c_write_buffer_ex(_addr, _data, _length, &i2c_res);
	return &i2c_res;
}

const i2c_result*  sn_write_read(UCHAR _addr, UCHAR* _data_out, UINT _out_length, UCHAR * _data_in, UINT _in_length)
{
	UINT i;
	clear_i2c_result();

	for (i = 0; i < sn_retry_count; i++)
	{
		if (i2c_write_read(_addr, _data_out, _out_length, _data_in, _in_length, &i2c_res))
		{
			break;
		}

		__delay_us(sn_retry_wait_us);
	}


	return &i2c_res;
}

void sn_ship_log(UCHAR _addr, UCHAR _level, UCHAR * _msg)
{
	UINT msg_len = strlen((const char *)_msg) + 1;	// Account for the terminating null
	sn_data_buffer[0] = SN_REG_LOGGER;
	sn_data_buffer[1] = _level;
	strncpy((char *)sn_data_buffer + 2,(const char *) _msg, SN_DBS);


	i2c_write_buffer_ex(_addr, sn_data_buffer, msg_len + 2, 0);

	return;
}

void sn_init(UINT _retry_count, UINT _retry_wait)
{
	sn_retry_count = _retry_count;
	sn_retry_wait_us = _retry_wait;

	mem_clear(&sn_data_buffer, SN_DBS);

	return;
}

/*
EOF
 */
