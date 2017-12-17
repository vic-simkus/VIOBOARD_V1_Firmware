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

#include "config.h"
#include "chip_config.h"

#include <xc.h>
#include <libpic30.h>
#include <string.h>		// memcpy


#include "serial_comm.h"
#include "I2C/i2c.h"
#include "I2C/i2c_logger_reg.h"
#include "I2C/logger.h"
#include "I2C/simkus_net.h"

#include "rtcc.h"
#include "support.h"

#include "command_processor.h"
#include "globals.h"

extern UINT ib_idx;

void main_event_loop(void)
{
	mem_clear(mg_command_buffer, COMMAND_BUFFER_SIZE);
	mem_clear(mg_separator_buffer, COMMAND_SEPARATOR_BUFFER_SIZE);

	UCHAR c = 0;
	UINT idx = 0;

	UINT loop_counter = 0;

	for (; ; )
	{
		ser_switch_input_buffers();

		for (idx = 0; idx < ser_input_buffer_b_idx; idx++)
		{
			c = ser_input_buffer_b[idx];

			/*
			 * Put character into buffer and advance index.
			 * Duh.
			 */

			mg_command_buffer[mg_cmd_buffer_idx] = c;
			mg_cmd_buffer_idx += 1;

			if (bin_context.is_active == 1)
			{
				/*
				 * In binary mode
				 */
				if (bin_context.count == 0 && bin_context.start_index == 0)
				{
					/*
					 * Length MSB
					 */
					bin_context.length_msb = c;
					bin_context.count += 1;
				}
				else if (bin_context.count == 1 && bin_context.start_index == 0)
				{
					/*
					 * Length LSB
					 */
					bin_context.expected_length = ((UINT) bin_context.length_msb) << 8;
					bin_context.expected_length |= c;

					bin_context.start_index = mg_cmd_buffer_idx;
					bin_context.count = 0;
				}
				else
				{
					/*
					 * At this point we have the expected binary length.
					 */

					/*
					 * We only advance the received binary data
					 */
					bin_context.count += 1;

					if (bin_context.count == bin_context.expected_length)
					{
						process_binary_command();
					}
				}
			}// in  binary mode
			else
			{
				if (c == '@')
				{
					bin_context.is_active = 1;

					/*
					 * Once binary mode is initiated we expect the next two bytes to be
					 * expected length in little endian format with MSB transmitted first.
					 */
				}
				else if (c == '\n' || c == '\r')
				{
					/*
					 * text mode
					 */

					if (mg_cmd_buffer_idx > 1)
					{
						mg_cmd_buffer_idx = mg_cmd_buffer_idx - 1;
						mg_command_buffer[mg_cmd_buffer_idx] = 0;
						process_text_command();

						reset_command_processor_state();
					}
					else
					{
						reset_command_processor_state();
					}

				}
				else if (c == ' ' || c == '\t')
				{
					mg_separator_buffer[mg_sep_buffer_idx] = mg_cmd_buffer_idx - 1;
					mg_sep_buffer_idx += 1;
				}

			} // not in binary mode.

			if (mg_sep_buffer_idx == COMMAND_SEPARATOR_BUFFER_SIZE || mg_cmd_buffer_idx == COMMAND_BUFFER_SIZE)
			{
				/*
				 * We're utilizing the command buffer in circular mode.  All indexes into it for reading are mod 32.
				 * Resetting of the separator index will reset the text mode stuff, but text mode shouldn't be sending that much data to begin with.  Fools.
				 */
				reset_command_processor_state();
			}//Buffer overflow
		}//Loop through the input buffer

		if (bin_context.is_stream_active)
		{
			/*
			 * XXX - Magic number.  60000 iterations is approximately four times a second.
			 */
			if (loop_counter == 60000)
			{
				process_binary_stream();
				loop_counter = 0;
			}

			loop_counter += 1;
		}

		i2c_logger_flush();
	} // Main loop
}

int main(void)
{
	//scs_overflow_errors = 0;
	RCON = 0x0;

	Nop();	//A place the debugger can call home

	init_globals();
	init_command_processor();

	/*Configure oscilator*/

	REFOCONbits.ROSSLP = 1;
	REFOCONbits.ROSEL = 1;
	REFOCONbits.RODIV = 0;

	REFOCONbits.ROEN = 1;

	SRbits.IPL = 0;				// set CPU interrupt priority to zero (all interrupts will fire)
	INTCON1bits.NSTDIS = 1;		// disable nested interrupts.

	ANSA = 0;
	ANSB = 0;					// Claim all pins from greedy AD module

	STAT_LED_TRIS = 0;			// Status LED on pin 25

	logger_init(LOGGER_LEVEL_DEBUG);

	rtcc_init();

	//ser_init(34, 1, 7, 0xFFFF);
	ser_init(207, 1, 5, 0xFFFF);

	ser_write_char(0x00);
	ser_write_char(0x00);
	ser_write_char(0x00);
	ser_write_char(0x00);
	ser_write_char(0x00);
	ser_write_char(0x00);
	ser_flush_buffer();

	ser_write_char('\n');
	logger_debug("RS232 OK.");

	i2c_setup_default_registers();
	i2c_logger_reg_init();

	/*
	 * 0x9D I2C1BRG value is based on a 16MHz Fcy and desired bus speed of 100kHz
	 * 0x25 I2C1BRG value is based on a 16MHz Fcy and desired bys speed of 400KHz
	 */
	i2c_init(4, I2C_ADDR_COMM_CTRL, 0x25);

	logger_debug("I2C OK.");

	sn_init(1000, 10);

	ser_write_char('#');
	while (1)
	{
		ser_write_char('*');
		if (sn_ping(I2C_ADDR_IO_CTRL))
		{
			break;
		}
		__delay_ms(1000);
	}

	ser_write_char('\n');
	logger_protocol("F CC. CC UP. IC SU.");

	main_event_loop();

	return 0;
} // Main function
