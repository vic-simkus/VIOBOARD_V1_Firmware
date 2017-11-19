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


#define __SERIAL_COMM_H_INT
#include "serial_comm.h"
#include "serial_comm_config.h"
#include "support.h"

#include <xc.h>
#include <stdlib.h>


#define SER_UTXI_1	0b00
#define SER_UTXI_4	0b11

volatile UCHAR input_buffer_1[SER_IBS];
volatile UCHAR input_buffer_2[SER_IBS];
volatile UCHAR * input_buffer_ptr;

volatile UCHAR output_buffer[SER_IBS];
volatile UCHAR * output_buffer_ptr = output_buffer;	// For the assembler stuff.  Because I couldn't figure out how to get the fire register address of a variable.

volatile UINT ib_idx;
volatile UINT ob_idx;

UINT timeout_clicks;

volatile UINT last_write_error;

inline static UINT write_char(UCHAR _c, UCHAR _do_buffer);

/*
 * Instrumentation for keeping track of our errors.
 */
ser_prot_errors serial_protocol_errors;

extern void ser_linker_fix(void);

void ser_init(UCHAR _brg, UCHAR _brgh, UCHAR _isr_priority, UINT _timeout_clicks)
{
	timeout_clicks = _timeout_clicks;
	//last_read_error = SER_ERR_NONE;
	//last_write_error = SER_ERR_NONE;

	mem_clear(&serial_protocol_errors,sizeof(ser_prot_errors));

	input_buffer_ptr = input_buffer_1;

	ib_idx = 0;
	ob_idx = 0;

	U1MODEbits.RTSMD = 0;		//	Flow control
	U1MODEbits.UEN = 0b10;		//	Use CTS/RTS
	U1MODEbits.BRGH = _brgh;
	U1BRG = _brg;

	// For a chip with FCY of 16Mhz
	// 34 - 115,000 baud
	// 416 - 9600 baud
	// 13332 - 300 baud

	U1STAbits.URXISEL = SER_UTXI_1;		//	Interrupt every character since the Microchip FIFOs are broken

	U1MODEbits.UARTEN = 1;				// Enable UART
	U1STAbits.UTXEN = 1;				// Enable transmission

	IFS0bits.U1RXIF = 0;				//	Clear pending interrupt flag
	IFS4bits.U1ERIF = 0;

	IPC2bits.U1RXIP = _isr_priority;	//	Set receive interrupt priority
	IPC16bits.U1ERIP = _isr_priority;

	IEC0bits.U1RXIE = 1;				//	Enable receive interrupts
	IEC4bits.U1ERIE = 1;				//	Enable UART1 error interrupts

	ser_linker_fix();
	return;
}

inline UINT ser_write_char(const UCHAR _c)
{
	return write_char(_c, 1);
}

inline UINT ser_write_16bit(const UINT _i)
{
	UCHAR written = write_char((UCHAR) (_i >> 8), 1);
	written += write_char((UCHAR) (_i), 1);
	return (written);
}

UINT ser_write_string(const UCHAR * _data)
{
	while (*_data != 0)
	{
		UCHAR c = *_data;
		if (write_char(c, 1) != 1)
		{
			//error
			return 0;
		}
		_data++;
	}

	return 1;
}

UINT ser_write_string_nl(const UCHAR * _data)
{
	if (ser_write_string(_data) != 1)
	{
		return 0;
	}

	return ser_write_char('\n');
}

UINT ser_write_data(const UCHAR * _data, UINT _len)
{
	int i;
	for (i = 0; i < _len; i++)
	{
		if (write_char(_data[i], 1) != 1)
		{
			return 0;
		}
	}
	return 1;
}

inline UCHAR ser_new_data(void)
{
	return (ib_idx > 0);
}

UINT ser_get_data(UCHAR ** _dest)
{
	UINT buffer_idx;

	/*
		Disable interrupts while the buffers are switched around.
		Because we're using hardware flow control we shouldn't
		lose any characters in the process.
	 */
	__builtin_disi(0x3FFF);

	*_dest = (UCHAR *) input_buffer_ptr;
	buffer_idx = ib_idx;

	if (input_buffer_ptr == input_buffer_1)
	{
		input_buffer_ptr = input_buffer_2;
	}
	else
	{
		input_buffer_ptr = input_buffer_1;
	}

	ib_idx = 0;

	/*
		Enable the interrupts
	 */
	__builtin_disi(0x0000);

	return buffer_idx;
}

int  __attribute__(( __section__(".libc"))) write(int handle, void *_buffer, unsigned int count)
{
	int i;
	for (i = 0; i < count; i++)
	{
		write_char(*((UCHAR *) _buffer + i), 1);
	}
	return i;
}


static inline UINT write_char(UCHAR _c, UCHAR _do_buffer)
{
	if (ob_idx == SER_OBS)
	{
		ser_flush_buffer();

		if (last_write_error != SER_ERR_NONE)
		{
			return 0;
		}
	}

	output_buffer[ob_idx] = _c;
	ob_idx = ob_idx + 1;

	if (_do_buffer == 0)
	{
		return ser_flush_buffer();
	}

	return 1;
}