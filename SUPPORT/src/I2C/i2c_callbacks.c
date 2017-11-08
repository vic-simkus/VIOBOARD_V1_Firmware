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

#include "I2C/i2c_callbacks.h"
#include "I2C/i2c.h"

void i2c_echo_reg_write_cb(UCHAR _char, UINT _idx)
{
	if(_idx >= I2C_ECHO_REG_SIZE)
	{
		return;
	}

	_I2C_REG_ECHO_DB[_idx] = _char;

	return;
}

UCHAR i2c_echo_reg_read_cb(UINT _idx)
{
	if(_idx >= I2C_ECHO_REG_SIZE)
	{
		return 0;
	}

	return _I2C_REG_ECHO_DB[_idx];
}

void i2c_reset_reg_write_cb(UCHAR _char, UINT _idx)
{
	if (_char == 0xAA)
	{
		{
			__asm__ volatile ("reset");
		}
	}

	return;
}

UCHAR i2c_null_reg_read_cb(UINT _notused)
{
	return 0;
}

void i2c_null_reg_write_cb(UCHAR _notused, UINT _notused2)
{
	return;
}
