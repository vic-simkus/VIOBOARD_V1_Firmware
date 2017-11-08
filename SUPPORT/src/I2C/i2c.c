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

#include <xc.h>

#include "I2C/i2c.h"
#include "I2C/i2c_callbacks.h"

#include  <libpic30.h>

#include <string.h>

#ifdef I2C_ERROR_STRINGS
const UCHAR * const  I2C_ERROR_MAP [] = {
	"NONE",
	"ROVFL",
	"RTO",
	"WNAC",
	"WNAD",
	"WTO",
	"COL"
};
#endif

volatile i2c_register_list i2c_registers;

static volatile UCHAR last_write_error;
static volatile UCHAR last_read_error;

//volatile UINT slave_isr_count;

void i2c_init(UCHAR _isr_priority, UCHAR _slave_address, UINT _brg)
{
	i2c_master_isr_fixup();
	i2c_slave_isr_fixup();

	I2CCONbits.STREN = 1; //	Enable clock stretching
	I2CCONbits.A10M = 0; //	Disable 10 bit addresses

	if (_slave_address > 0)
	{
		I2CADD = _slave_address;
	}

	if (_brg > 0)
	{
		I2CBRG = _brg;
	}


	SLAVE_IPC = _isr_priority;
	SLAVE_IEC = 1; // Enable slave interrupts

	MASTER_IPC = _isr_priority;
	MASTER_IEC = 1;

	SLAVE_IF = 0;
	MASTER_IF = 0;


	I2CCONbits.I2CEN = 1; //	Enable I2C module

	//I2CCONbits.PEN = 1;

	return;
}

SINT i2c_last_read_error(void)
{
	SINT t = last_read_error;
	last_read_error = I2C_ERR_NONE;
	return t;
}

SINT i2c_last_write_error(void)
{
	SINT t = last_write_error;
	last_write_error = I2C_ERR_NONE;
	return t;
}


UCHAR _I2C_REG_PING_DB[1] = {0};
UCHAR _I2C_REG_PING_EX_DB[4] = {'P', 'O', 'N', 'G'};
UCHAR _I2C_REG_VER_DB[1] = {0};

volatile UCHAR _I2C_REG_ECHO_DB[I2C_ECHO_REG_SIZE];

void i2c_setup_default_registers(void)
{
	i2c_registers.count = I2C_REG_COUNT__;

	/*
	i2c_registers.register_list[I2C_REG_NULL] = i2c_init_register();
	i2c_registers.register_list[I2C_REG_RESET] = i2c_init_register();
	i2c_registers.register_list[I2C_REG_PING] = i2c_init_register();
	i2c_registers.register_list[I2C_REG_PING_EX] = i2c_init_register();
	i2c_registers.register_list[I2C_REG_ECHO] = i2c_init_register();
	i2c_registers.register_list[I2C_REG_VERSION] = i2c_init_register();
	i2c_registers.register_list[I2C_REG_VERSION_STR] = i2c_init_register();
	 */

	i2c_registers.register_list[I2C_REG_NULL].data = 0;
	i2c_registers.register_list[I2C_REG_NULL].data_length = 0;
	i2c_registers.register_list[I2C_REG_NULL].cb_read = i2c_null_reg_read_cb;
	i2c_registers.register_list[I2C_REG_NULL].cb_write = i2c_null_reg_write_cb;
	i2c_registers.register_list[I2C_REG_NULL].ro = 0;

	i2c_registers.register_list[I2C_REG_RESET].data = 0;
	i2c_registers.register_list[I2C_REG_RESET].data_length = 0;
	i2c_registers.register_list[I2C_REG_RESET].cb_read = 0;
	i2c_registers.register_list[I2C_REG_RESET].cb_write = i2c_reset_reg_write_cb;
	i2c_registers.register_list[I2C_REG_RESET].ro = 0;

	i2c_registers.register_list[I2C_REG_PING].data = _I2C_REG_PING_DB;
	i2c_registers.register_list[I2C_REG_PING].ro = 1;
	i2c_registers.register_list[I2C_REG_PING].data[0] = 'R';
	i2c_registers.register_list[I2C_REG_PING].data_length = 1;

	i2c_registers.register_list[I2C_REG_PING_EX].data = _I2C_REG_PING_EX_DB;
	i2c_registers.register_list[I2C_REG_PING_EX].ro = 1;
	i2c_registers.register_list[I2C_REG_PING_EX].data_length = 4;

	i2c_registers.register_list[I2C_REG_ECHO].data = 0;
	i2c_registers.register_list[I2C_REG_ECHO].ro = 0;
	i2c_registers.register_list[I2C_REG_ECHO].data_length = 0;
	i2c_registers.register_list[I2C_REG_ECHO].cb_read = i2c_echo_reg_read_cb;
	i2c_registers.register_list[I2C_REG_ECHO].cb_write = i2c_echo_reg_write_cb;

	i2c_registers.register_list[I2C_REG_VERSION].data = _I2C_REG_VER_DB;
	i2c_registers.register_list[I2C_REG_VERSION].ro = 1;
	i2c_registers.register_list[I2C_REG_VERSION].data_length = 1;
	i2c_registers.register_list[I2C_REG_VERSION].data[0] = '1';

	i2c_registers.register_list[I2C_REG_VERSION_STR].data = (UCHAR*) "VI2C v1.0";
	i2c_registers.register_list[I2C_REG_VERSION_STR].ro = 1;
	i2c_registers.register_list[I2C_REG_VERSION_STR].data_length = strlen((const char *)i2c_registers.register_list[I2C_REG_VERSION_STR].data) + 1; // account for the null at the end.

	return;
}

/******************************************************************************
Level 2 functions
 ******************************************************************************/

UCHAR i2c_write_read(UCHAR _address, UCHAR * _write_data, UINT _write_count, UCHAR * _read_buffer, UINT _read_count, i2c_result * _res)
{
	UINT read_idx = 0;
	INIT_RES_STRUCT(res);

	last_write_error = I2C_ERR_NONE;
	last_read_error = I2C_ERR_NONE;

	/*
	 * The bus is started by i2c_write_control_byte
	 */
	if (!i2c_write_control_byte((_address << 1) | I2C_RW_W, 0))
	{
		goto __end_func;
	}

	res.bytes_written = i2c_write_buffer(_write_data, _write_count);

	if (res.bytes_written != _write_count)
	{
		/*
		 * Failed to write all of the bytes in the buffer.
		 */
		goto __end_func;
	}

	/*
	 * The bus is restarted by i2c_write_control_byte
	 */
	if (!i2c_write_control_byte((_address << 1) | I2C_RW_R, 1)) // Reverse direction of the bus
	{
		/*
		 * Failed to reverse the bus
		 */
		goto __end_func;
	}

	/*
	So at this point, the data to be sent has been sent.
	The bus direction has been reversed.  All without errors.  Magic, I tell ya.
	 */

	/*
	 * Read and clear overflow for shits and giggles
	 */
	_read_buffer[0] = I2CRCV;
	I2CSTATbits.I2COV = 0;

	read_idx = 0;

	while (1)
	{
		I2CCONbits.RCEN = 1; // Enable master receive

		while (I2CCONbits.RCEN == 1)
		{
			/*
			 * Allow master receive operation to complete.
			 */
			Nop();
		}

		_read_buffer[read_idx] = I2CRCV; // Read receive buffer
		read_idx = read_idx + 1; // Increment read index

		if (read_idx < _read_count) // If less then desired count...
		{
			I2CCONbits.ACKDT = 0; // Send ACK
		}
		else
		{
			I2CCONbits.ACKDT = 1; // Else send NACK
		}

		I2C_WAIT_I2CCON_L5();
		I2CCONbits.ACKEN = 1; // Start the ACK process

		while (I2CCONbits.ACKEN == 1)
		{
			/*
			 * Wait for the process to finish.
			 */
			Nop();
		}

		if (I2CCONbits.ACKDT == 1) // If nack was sent...
		{
			break; // Bounce
		}
	} // receive loop

	res.bytes_read = read_idx;

__end_func:

	i2c_bus_stop();

	res.write_error = last_write_error;
	res.read_error = last_read_error;

	if (_res != 0)
	{
		_res->bytes_read = res.bytes_read;
		_res->bytes_written = res.bytes_written;
		_res->write_error = res.write_error;
		_res->read_error = res.read_error;
	}

	return !(res.write_error & res.read_error);
}

/******************************************************************************

Level 1 functions

 *******************************************************************************/

UCHAR i2c_write_buffer_ex(UCHAR _address, UCHAR * _data, UINT _count, i2c_result * _res)
{
	last_write_error = I2C_ERR_NONE;
	last_read_error = I2C_ERR_NONE;
	INIT_RES_STRUCT(res);

	if (i2c_write_control_byte((_address << 1) | I2C_RW_W, 0) == 0)
	{
		/*
		 * Failed to send control byte.  Ooops.
		 */
		res.write_error = last_write_error;
		res.read_error = last_read_error;
	}
	else
	{
		res.bytes_written = i2c_write_buffer(_data, _count);

		if (res.bytes_written < _count)
		{
			/*
			 * Failed to send byte.
			 */
			res.write_error = last_write_error;
			res.read_error = last_read_error;
		}
	}

	i2c_bus_stop();

	res.write_error = last_write_error;
	res.read_error = last_read_error;

	if (_res != 0)
	{
		_res->bytes_read = res.bytes_read;
		_res->bytes_written = res.bytes_written;
		_res->write_error = res.write_error;

		_res->read_error = res.read_error;
	}

	return !(res.write_error & res.read_error);
}

UINT i2c_write_buffer(UCHAR * _data, UINT _count)
{
	int i = 0;

	for (i = 0; i < _count; i++)
	{
		if (!i2c_write_byte(_data[i]))
		{
			/*
			 * Byte write failed.  Error is in last_write_error.
			 */
			break;
		}
	}// dump bits onto the bus

	return i;
}

/******************************************************************************

Level 0 functions

 *******************************************************************************/

inline UCHAR i2c_bus_start(void)
{
	I2C_WAIT_IDLE_BUS();

	I2CCONbits.SEN = 1;

	while (I2CCONbits.SEN == 1)
	{
		Nop();
	}

	if (I2CSTATbits.BCL == 0)
	{
		return 1;
	}
	else
	{
		last_write_error = I2C_ERR_COLLISION;
		return 0;
	}
}

inline UCHAR i2c_bus_restart(void)
{
	I2C_WAIT_I2CCON_L5();

	I2CCONbits.RSEN = 1;

	while (I2CCONbits.RSEN == 1)
	{
		Nop();
	}

	if (I2CSTATbits.BCL == 0)
	{
		return 1;
	}
	else
	{
		last_write_error = I2C_ERR_COLLISION;
		return 0;
	}
}

inline UCHAR i2c_bus_stop(void)
{
	I2C_WAIT_I2CCON_L5();

	I2CCONbits.PEN = 1;

	while (I2CCONbits.PEN == 1)
	{
		Nop();
	}

	if (I2CSTATbits.BCL == 0)
	{
		return 1;
	}
	else
	{
		last_write_error = I2C_ERR_COLLISION;
		return 0;
	}
}

UCHAR i2c_write_control_byte(UCHAR _addr, UCHAR _restart)
{
	I2CSTATbits.IWCOL = 0; // clear the collision bit
	I2CSTATbits.BCL = 0; //clear master collision bit

	if (_restart == 1)
	{
		if (!i2c_bus_restart())
		{
			// Collision detected.
			return 0;
		}
	}
	else
	{
		if (!i2c_bus_start())
		{
			// Collision detected.
			return 0;
		}
	}

	if (!i2c_write_byte(_addr))
	{
		/*
		 * i2c_write_byte sets its own error code.
		 */

		if (last_write_error == I2C_ERR_W_NACK_DATA)
		{
			/*
			 * Change the NACK error because write_byte does not know if it's
			 * writing a data or control bit.
			 */
			last_write_error = I2C_ERR_W_NACK_CTRL;
		}

		return 0;
	}
	return 1;
}

UCHAR i2c_write_byte(UCHAR _data_out)
{
	turn_on_send_led();

	/*
	 * Wait for an idle bus from the perspective of the local chip
	 */
	I2C_WAIT_I2CCON_L5();

	/*
	 * Clear collision flags.
	 */
	I2CSTATbits.IWCOL = 0;
	I2CSTATbits.BCL = 0;

	/*
	 * Shove data into the output register.
	 */
	I2CTRN = _data_out;

	/*
	 * Wait for the chip to finish sending it AND process an ACK/NACK from the remote chip
	 */
	WAIT_FOR_TRSTAT();

	/*
	 * We either fucked out with the timing or something went goofy.  Report collision and bounce.
	 */
	if (I2CSTATbits.IWCOL == 1 || I2CSTATbits.BCL == 1)
	{
		last_write_error = I2C_ERR_COLLISION;
		turn_off_send_led();
		return 0;
	}

	/*
	 * Slave failed to acknowledge the receipt of the byte.  Its receive buffer must have overflowed.
	 * Slave ACK.  1 == NACK
	 */
	if (I2CSTATbits.ACKSTAT == 1)
	{
		last_write_error = I2C_ERR_W_NACK_DATA;
		turn_off_send_led();
		return 0;
	}

	turn_off_send_led();
	return 1;	// 1 byte written out.  Clever, eh
}

/*
EOF
 */