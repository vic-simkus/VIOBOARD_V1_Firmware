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
#include <stdlib.h>
#include <string.h>

#include "I2C/simkus_net.h"

#include "serial_comm.h"

#include "cp_ic.h"
#include "iocontroller_interface.h"
#include "globals.h"

#define PRINT_BUFFER_SIZE 10

static UCHAR __attribute__((aligned)) print_buffer[PRINT_BUFFER_SIZE];

/*
 * Displays digital output status's
 */
UCHAR cmd_print_status_do()
{
	ser_write_string("# DO outputs:\n");

	UCHAR do_bits = get_digital_ouputs();

	ser_write_string("#DO 1: ");
	ser_write_string((do_bits & DO_1_MASK) ? "ON" : "OFF");
	ser_write_char('\n');

	ser_write_string("#DO 2: ");
	ser_write_string((do_bits & DO_2_MASK) ? "ON" : "OFF");
	ser_write_char('\n');

	ser_write_string("#DO 3: ");
	ser_write_string((do_bits & DO_3_MASK) ? "ON" : "OFF");
	ser_write_char('\n');

	ser_write_string("#DO 4: ");
	ser_write_string((do_bits & DO_4_MASK) ? "ON" : "OFF");
	ser_write_char('\n');

	return 1;
}

UCHAR cmd_print_status_pmic(void)
{
	ser_write_string("# PMIC status:\n");

	UCHAR stat = get_pmic_status();

	ser_write_string("#DO Status:     ");
	ser_write_string((stat & PMIC_DO_EN_MASK) ? "ON" : "OFF");
	ser_write_char('\n');

	ser_write_string("#DO fault flag: ");
	ser_write_string((stat & PMIC_DO_ERR_MASK)  ? "ON" : "OFF");
	ser_write_char('\n');

	ser_write_string("#AI status:     ");
	ser_write_string((stat & PMIC_AI_EN_MASK)  ? "ON" : "OFF");
	ser_write_char('\n');

	ser_write_string("#AI fault flag: ");
	ser_write_string((stat & PMIC_AI_ERR_MASK) ? "ON" : "OFF");
	ser_write_char('\n');

	return 1;
}

/**
 * Displays the IOCONTROLLER analog input values.
 */
UCHAR cmd_print_status_ai(void)
{
	ser_write_string("# AI inputs:\n");

	UCHAR ret = 1;
	UCHAR i = 0;

	update_analog_readings();

	mem_clear((UCHAR *) & print_buffer, PRINT_BUFFER_SIZE);

	for (i = 0; i < IOC_AI_COUNT; i++)
	{

		utoa((char *) &print_buffer, i + 1, 10);
		ser_write_string("AI ");
		ser_write_string((const UCHAR *) &print_buffer);
		ser_write_string(": R=");

		mem_clear((UCHAR *) & print_buffer, PRINT_BUFFER_SIZE);

		utoa((char *) &print_buffer, IOC_AI_RAW_VALUES[i], 10);
		ser_write_string((const UCHAR *) &print_buffer);

		ser_write_string(", V=");

		ftoa((char *) &print_buffer, IOC_AI_VOLT_VALUES[i], 7, 5);
		ser_write_string((const UCHAR *) &print_buffer);

		ser_write_char('\n');
	}

	ser_write_char('\n');
	return ret;
}

UCHAR cmd_print_status_boot_count()
{
	ser_write_string("#Boot count: ");
	UINT boot_count = get_boot_count();

	mem_clear((UCHAR *) & print_buffer, PRINT_BUFFER_SIZE);
	utoa((char *) &print_buffer, boot_count, 10);
	ser_write_string((const UCHAR *) &print_buffer);
	ser_write_char('\n');

	return 1;
}

/**
 * Displays I2C bus stats as seen by the COMMCONTROLLER
 */
UCHAR cmd_print_status_bus(void)
{
	ser_write_string("#I2C bus stats:\n");

	ser_write_string("#I2CSTAT:\n");
	ser_write_string("# .TBF: ");
	ser_write_string((I2CSTATbits.TBF == 1 ? "1" : "0"));
	ser_write_string("\n");

	ser_write_string("# .RBF: ");
	ser_write_string((I2CSTATbits.RBF == 1 ? "1" : "0"));
	ser_write_string("\n");

	ser_write_string("# .R_W: ");
	ser_write_string((I2CSTATbits.R_NOT_W == 1 ? "1" : "0"));
	ser_write_string("\n");

	ser_write_string("# .S: ");
	ser_write_string((I2CSTATbits.S == 1 ? "1" : "0"));
	ser_write_string("\n");

	ser_write_string("# .P: ");
	ser_write_string((I2CSTATbits.P == 1 ? "1" : "0"));
	ser_write_string("\n");

	ser_write_string("# .D_A: ");
	ser_write_string((I2CSTATbits.D_NOT_A == 1 ? "1" : "0"));
	ser_write_string("\n");

	ser_write_string("# .I2COV: ");
	ser_write_string((I2CSTATbits.I2COV == 1 ? "1" : "0"));
	ser_write_string("\n");

	ser_write_string("# .IWCOL: ");
	ser_write_string((I2CSTATbits.IWCOL == 1 ? "1" : "0"));
	ser_write_string("\n");

	ser_write_string("# .BCL: ");
	ser_write_string((I2CSTATbits.BCL == 1 ? "1" : "0"));
	ser_write_string("\n");

	ser_write_string("\n");

	return 1;
}

/**
 * Pings the IOCONTROLLER chip and displays the results.
 */
void cmd_ping()
{
	ser_write_string("\n# Trying to ping I/O chip:\n");

	if (sn_ping(I2C_ADDR_IO_CTRL) == 1)
	{
		ser_write_string("# The IO chip is alive.\n");
	}
	else
	{
		ser_write_string("# The IO chip did not respond.\n");
	}

	ser_write_string("# End of ping\n");
	return;
}

static inline UCHAR set_bit(UCHAR byte, UCHAR mask, UCHAR val)
{
	if (val == 1)
	{
		return byte | mask;
	}
	else
	{
		return byte & (~mask);
	}
}

UCHAR cmd_set_pmic(void)
{
	ser_write_string("PMIC\n");
	UCHAR ret = 1;

	if (mg_sep_buffer_idx != 4)
	{
		ser_write_string("\n!Invalid parameter count.  Expecting 4.\n");
		ret = 0;
		goto _end;
	}

	UCHAR pmic_id = mg_command_buffer[mg_separator_buffer[1] + 1];
	UCHAR pmic_flag = mg_command_buffer[mg_separator_buffer[2] + 1];
	UCHAR value = mg_command_buffer[mg_separator_buffer[3] + 1];

	if (pmic_id != 'A' && pmic_id != 'D')
	{
		ser_write_string("!Invalid PMIC ID [");
		ser_write_char(pmic_id);
		ser_write_string("].  Expecting AI or DO\n");

		ret = 0;
		goto _end;
	}

	if (pmic_flag != 'E' && pmic_id != 'F')
	{
		ser_write_string("!Invalid PMIC flag [");
		ser_write_char(pmic_flag);
		ser_write_string("].  Expecting E or F \n");

		ret = 0;
		goto _end;
	}

	if (value != '1' && value != '0')
	{
		ser_write_string("!Invalid PMIC flag.  Expecting 1 or 0 \n");
		ret = 0;
		goto _end;
	}

	if (value == '1')
	{
		value = 1;
	}
	else
	{
		value = 0;
	}

	UCHAR curr_val = get_pmic_status();

	if (pmic_id == 'A')
	{
		// Analog output

		if (pmic_flag == 'E')
		{
			curr_val = set_bit(curr_val, PMIC_AI_EN_MASK, value);
		}
		else
		{
			curr_val = set_bit(curr_val, PMIC_AI_ERR_MASK, value);
		}
	}
	else
	{
		// Digital output

		if (pmic_flag == 'E')
		{
			curr_val = set_bit(curr_val, PMIC_DO_EN_MASK, value);
		}
		else
		{
			curr_val = set_bit(curr_val, PMIC_DO_ERR_MASK, value);
		}
	}

	set_pmic_status(curr_val);

_end:
	return ret;
}

UCHAR cmd_set_do(void)
{
	UCHAR ret = 1;

	ser_write_string("Digital outputs.\n");

	if (mg_sep_buffer_idx != 3)
	{
		ret = 0;
		ser_write_string("\n!Invalid parameter count.  Expecting 3.\n");
		goto _end;
	}

	UCHAR do_idx = mg_command_buffer[mg_separator_buffer[1] + 1];
	UCHAR do_val = mg_command_buffer[mg_separator_buffer[2] + 1];

	ser_write_string("# DO: ");
	ser_write_char(do_idx);
	ser_write_string(" -> ");
	ser_write_char(do_val);
	ser_write_char('\n');

	if (do_val == '0')
	{
		do_val = 0;

	}
	else if (do_val == '1')
	{
		do_val = 1;
	}
	else
	{
		ser_write_string("! Invalid DO value.  Expecting 1 or 0.\n");
		ret = 0;
		goto _end;
	}

	UCHAR current_outputs = get_digital_ouputs();

	if (do_idx == '1')
	{
		do_idx = DO_1_MASK;
	}
	else if (do_idx == '2')
	{
		do_idx = DO_2_MASK;
	}
	else if (do_idx == '3')
	{
		do_idx = DO_3_MASK;
	}
	else if (do_idx == '4')
	{
		do_idx = DO_4_MASK;
	}
	else
	{
		ser_write_string("! Invalid DO index.  Expecting 1-4\n");
		ret = 0;
		goto _end;
	}

	if (do_val == 1)
	{
		current_outputs |= do_idx;
	}
	else
	{
		current_outputs &= ~do_idx;
	}

	set_digital_outputs(current_outputs);

_end:
	return ret;
}

UCHAR cmd_print_cal_values()
{
	UCHAR ret = 1;

	/*
	 * To get to this point we expect the command line to be something along the lines of:
	 * ST C x
	 * STATUS CAL <cal level>
	 *
	 * So we're expecting the separator index to be 2.
	 */


	if (mg_sep_buffer_idx != 2)
	{
		ret = 1;
		ser_write_string("!Invalid number of parameters.  Expecting 1.\n");
		goto _end;
	}

	UCHAR cal_lev = mg_command_buffer[mg_separator_buffer[1] + 1];


	ser_write_string("\n#Cal vals level  ");
	ser_write_char(cal_lev);
	ser_write_char(':');
	ser_write_char('\n');

	if (cal_lev == '1')
	{
		update_l1_cal_values();
	}
	else if (cal_lev == '2')
	{
		update_l2_cal_values();
	}
	else
	{
		ret = 1;
		ser_write_string("!Invalid level parameter.\n");
		goto _end;
	}

	mem_clear((UCHAR *) & print_buffer, PRINT_BUFFER_SIZE);

	UCHAR i = 0;
	for (i = 0; i < IOC_AI_COUNT; i++)
	{
		utoa((char *) &print_buffer, ((IOC_CAL_VALUES[i] >> 8) & 0x00FF), 10);
		ser_write_char('+');
		ser_write_string(print_buffer);
		ser_write_char(' ');
		utoa((char *) &print_buffer, (IOC_CAL_VALUES[i] & 0x00FF), 10);
		ser_write_char('-');
		ser_write_string(print_buffer);
		ser_write_char('\n');
	}

	mem_clear((UCHAR *) & print_buffer, PRINT_BUFFER_SIZE);

_end:
	return ret;
}

UCHAR cmd_set_cal(void)
{
	/*
	 * What we're expecting here is:
	 * SE C N L +|-XXXX
	 * SET CALIBRATION iNput Level +|-XXXX
	 * Where:
	 *   N is the analog input
	 *   L is the level - either 1 or 2
	 *   +|- is either a plus or a minus adjustment of the analog input value
	 *   XXXX The value, in decimal, of the offset.  Maximum is 255.
	 */

	ser_write_string(" analog calibration offsets\n");

	UCHAR ret = 1;

	if (mg_sep_buffer_idx != 4)
	{
		ret = 1;
		ser_write_string("!Invalid number of parameters.  Expecting 5.\n");
		goto _end;
	}

	UCHAR input_num = mg_command_buffer[mg_separator_buffer[1] + 1];

	switch (input_num)
	{
		case '1':
			input_num = 1;
			break;
		case '2':
			input_num = 2;
			break;
		case '3':
			input_num = 3;
			break;
		case '4':
			input_num = 4;
			break;
		case '5':
			input_num = 5;
			break;
		case '6':
			input_num = 6;
			break;
		case '7':
			input_num = 7;
			break;
		case '8':
			input_num = 8;
			break;
		default:
			input_num = 0;
			break;
	}

	if (input_num < 1 || input_num > 8)
	{
		ser_write_string("! Analog input out of range.  Expecting 1<=input<=8\n");
		ret = 0;
		goto _end;
	}

	input_num -= 1;

	UCHAR level = mg_command_buffer[mg_separator_buffer[2] + 1];

	if (level == '1')
	{
		level = 1;
	}
	else if (level == '2')
	{
		level = 2;
	}
	else
	{
		level = 0;
	}

	if (level < 1 || level > 2)
	{
		ser_write_string("! Calibration level out of range.  Expecting 1<=input<=2\n");
		ret = 0;
		goto _end;
	}

	UCHAR direction = mg_command_buffer[mg_separator_buffer[3] + 1];

	if (direction != '-' && direction != '+')
	{
		ser_write_string("! Calibration direction is weird.  Expecting + or -.  Got: ");
		ser_write_char(direction);
		ser_write_char('\n');

		ret = 0;
		goto _end;
	}

	UINT offset = atoi(mg_command_buffer + mg_separator_buffer[3] + 2);

	if (offset > 255)
	{
		ser_write_string("! Offset out of range.  Expecting 0<=offset<=255.\n");
		ret = 0;
		goto _end;
	}

	if (level == 1)
	{
		update_l1_cal_values();
	}
	else
	{
		update_l2_cal_values();
	}

	IOC_CAL_VALUES[input_num] = 0;

	IOC_CAL_VALUES[input_num] = offset;

	if (direction == '+')
	{
		IOC_CAL_VALUES[input_num] = (IOC_CAL_VALUES[input_num] << 8);
	}


	if (level == 1)
	{
		set_l1_cal_values();
	}
	else
	{
		set_l2_cal_values();
	}

_end:
	return ret;
}

UCHAR cmd_set(void)
{
	UCHAR ret = 1;

	ser_write_string("\n#Setting: ");

	if (mg_sep_buffer_idx > 0)
	{
		if (mg_command_buffer[mg_separator_buffer[0] + 1] == 'D')
		{
			ret = cmd_set_do();
		}
		else if (mg_command_buffer[mg_separator_buffer[0] + 1] == 'P')
		{
			ret = cmd_set_pmic();
		}
		else if (mg_command_buffer[mg_separator_buffer[0] + 1] == 'C')
		{
			ret = cmd_set_cal();
		}
		else
		{
			ser_write_string("\n! Invalid set command.\n");
			ret = 0;
		}
	}
	else
	{
		ser_write_string("\n! Missing set identifier.\n");
		ret = 0;
	}

	return ret;
}

/**
 * Main entry point into the 'print status' command tree
 */
UCHAR cmd_print_status(void)
{

	if (mg_sep_buffer_idx < 1)
	{
		ser_write_string("! No parameter.\n");
		return 0;
	}

	if (mg_separator_buffer[0] + 1 >= mg_cmd_buffer_idx)
	{
		ser_write_string("! Terminating separator?\n");
		return 0;
	}

	UCHAR sep = mg_separator_buffer[0];
	UCHAR parm = mg_command_buffer[sep + 1];

	if ( parm == 'I')
	{
		/*
		 * I2C bus stats
		 */
		return cmd_print_status_bus();
	}
	else if (parm == 'A')
	{
		/*
		 * Analog input stats.
		 */

		return cmd_print_status_ai();
	}
	else if (parm == 'D')
	{
		/*
		 * Digital output stats
		 */
		return cmd_print_status_do();
	}
	else if (parm == 'P')
	{
		/*
		 * Digital output stats
		 */
		return cmd_print_status_pmic();
	}
	else if (parm == 'B')
	{
		return cmd_print_status_boot_count();
	}
	else if (parm == 'C')
	{
		return cmd_print_cal_values();
	}
	else
	{
		ser_write_string("! Invalid parameter: [");
		ser_write_data(mg_command_buffer + sep + 1, (mg_cmd_buffer_idx - sep) - 1 )  ;
		ser_write_string("]\n");

		return 0;
	}
	return 0;
}

/**
 * Resets botht the COMMCONTROLLER and IOCONTROLLER chips
 */
void cmd_reset(void)
{
	sn_reset_slave(I2C_ADDR_IO_CTRL);
	__asm__ volatile ("reset");

	return;
}

/**
 * Part of the 'COMMAND' command tree.  Displays the version of the IOCONTROLLER firmware.
 * @return 0 on failure, 1 on success
 */
UCHAR cmd_command_do_version()
{

	UCHAR buffer[1];
	buffer[0] = I2C_REG_VERSION;

	if (i2c_write_read(I2C_ADDR_IO_CTRL, (UCHAR *) & buffer, 1, (UCHAR *) & buffer, 1, 0))
	{
		ser_write_string("# version: ");
		ser_write_data((UCHAR *) & buffer, 1);
		ser_write_char('\n');
	}
	else
	{
		ser_write_string("! Failed to get version.\n");
		return 0;
	}

	return 1;
}

/**
 * Part of the 'COMMAND' command tree.  Writes data to the ECHO register on the IOCONTROLLER chip and reads it back and prints the result
 * @param _data Data to send
 * @param _length Length of the data to send.
 * @return 0 on failure, 1 on success
 */
UCHAR cmd_command_do_echo(UCHAR* _data, UCHAR _length)
{
#define BUFF_SIZE 32

	UCHAR ret = 1;
	UCHAR length = _length <= BUFF_SIZE - 1 ? _length : BUFF_SIZE - 1;
	UCHAR buffer[BUFF_SIZE];

	memcpy(buffer + 1, _data, length);
	buffer[0] = I2C_REG_ECHO;

	ser_write_string("# Sending for echo: [");
	ser_write_data(_data, length);
	ser_write_string("]\n");

	if (!i2c_write_buffer_ex(I2C_ADDR_IO_CTRL, buffer, length + 1, 0))
	{
		ser_write_string("! Failed to write to remote.");
		ret = 0;
		goto __end_func;
	}

	mem_clear(buffer, BUFF_SIZE);

	buffer[0] = I2C_REG_ECHO;

	if (!i2c_write_read(I2C_ADDR_IO_CTRL, buffer, 1, buffer, length, 0))
	{
		ser_write_string("! Failed to read from remote.");
		ret = 0;
		goto __end_func;
	}

	ser_write_string("# Remote response: [");
	ser_write_data(buffer, length);
	ser_write_string("]\n");

__end_func:
	return ret;
}

/**
 * Part of the 'COMMAND' command tree.  Prints the verbose version of the IOCONTROLLER chip firmware.
 * @return 0 on failure, 1 on success
 */
UCHAR cmd_command_do_verbose_version()
{
	UCHAR buffer[16];
	mem_clear((UCHAR *) & buffer, 16);

	buffer[0] = I2C_REG_VERSION_STR;

	if (i2c_write_read(I2C_ADDR_IO_CTRL, (UCHAR *) & buffer, 1, (UCHAR *) & buffer, 16, 0))
	{
		ser_write_string("# version string: ");
		ser_write_string((UCHAR *) & buffer);
		ser_write_char('\n');
	}
	else
	{
		ser_write_string("! Failed to get version.\n");
		return 0;
	}

	return 1;
}

/**
 * Main entry point to the 'COMMAND' command tree.
 * @return 0 in failure, 1 on success
 */
UCHAR cmd_command(void)
{
	if (mg_sep_buffer_idx < 1)
	{
		ser_write_string("! No parameter.\n");
		return 0;
	}

	if (mg_separator_buffer[0] + 1 >= mg_cmd_buffer_idx)
	{
		ser_write_string("! Terminating separator?\n");
		return 0;
	}

	UCHAR sep = mg_separator_buffer[0];
	UCHAR parm = mg_command_buffer[sep + 1];

	if ( parm == 'V')
	{
		if (sep + 2 >= mg_cmd_buffer_idx)
		{
			// Nothing past V
			return cmd_command_do_version();
		}
		else
		{
			if (mg_command_buffer[sep + 2] == 'S')
			{
				return cmd_command_do_verbose_version();
			}
			else
			{
				ser_write_string("! Invalid version parameter: [");
				ser_write_data(mg_command_buffer + sep + 1, (mg_cmd_buffer_idx - sep) - 1 );
				ser_write_string("]\n");
			}
		}
	}
	else if (parm == 'E')
	{
		if (mg_sep_buffer_idx > 1)
		{
			return cmd_command_do_echo(mg_command_buffer + mg_separator_buffer[1] + 1, mg_cmd_buffer_idx -  mg_separator_buffer[1]);
		}
		else
		{
			return cmd_command_do_echo("OHCE", 4);
		}
	}
	else
	{
		ser_write_string("! Invalid parameter: [");
		ser_write_data(mg_command_buffer + sep + 1, (mg_cmd_buffer_idx - sep) - 1 )  ;
		ser_write_string("]\n");

		return 0;
	}

	return 1;
}

/**
 * Main entry to the 'HELP' command tree
 */
void print_help(void)
{
	ser_write_string("\n# Help:\n");
	ser_write_string("# STATUS {I2C|AI|DO|PMIC|B|C}\n");
	ser_write_string("# SET { DO {1-4}  |  PMIC { AO|DO } { E|F } } VALUE\n");
	ser_write_string("# COMMAND {V|VS|E} [VALUE]\n");
	ser_write_string("# PING\n");
	ser_write_string("# RESET\n");
	ser_write_string("# ? -- Help(this)\n");
	ser_write_string("# END\n");
	ser_flush_buffer();

	return;
}
