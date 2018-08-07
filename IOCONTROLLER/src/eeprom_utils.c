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

#include "eeprom_utils.h"
#include "support.h"
#include "globals.h"
#include <stddef.h>

void eeprom_initialize_data(void)
{
	NVMCONbits.WRERR = 0;

	eeprom_data.eeprom_init = 0xDEAD;
	eeprom_data.boot_count = 1;
	eeprom_data.serial = 0;
	eeprom_data.is_dirty = 0;

	UINT i = 0;

	for (i = 0; i < AD_INPUT_NUM; i++)
	{
		eeprom_data.l1_cal_data_arr[i] = (i << 8);
		eeprom_data.l2_cal_data_arr[i] = ((i * 2) << 8);
	}

	eeprom_data.pad[0] = 0x100;
	eeprom_data.pad[1] = 0x101;
	eeprom_data.pad[2] = 0x102;
	eeprom_data.pad[3] = 0x103;
	eeprom_data.pad[4] = 0x104;
	eeprom_data.pad[5] = 0x105;
	eeprom_data.pad[6] = 0x106;
	eeprom_data.pad[7] = 0x107;
	eeprom_data.pad[8] = 0x108;
	eeprom_data.pad[9] = 0x109;
	eeprom_data.pad[10] = 0x10A;
	eeprom_data.pad[11] = 0xBEEF;

	eeprom_writeout_data();
}

ULONG eeprom_get_max_serial_ex(void)
{
	size_t i;
	UINT sv = 0, max_sv = 0, offset = 0;

	for (i = 0; i < EEPROM_DATA_STRUCT_COUNT; i++)
	{
		sv = eeprom_read_word((void *) EEPROM_DATA_OFFSETS[i] + EEPROM_SERIAL_OFFSET);

		if (sv <= 0xFFFE && sv > max_sv)
		{
			max_sv = sv;
			offset = i;
		}
	}

	/*
	 * We pack two 16 bit values into a 32 bit long
	 */
	ULONG ret;

	ret = offset;
	// Move the offset into the upper 16 bits...
	ret = ret << 16;
	// Add the max_sv
	ret |= max_sv;

	return ret;
}

ULONG eeprom_get_next_serial_ex(void)
{
	ULONG val = eeprom_get_max_serial_ex();

	UINT offset = EE_UNPACK_OFFSET(val) + 1;
	UINT serial = EE_UNPACK_SERIAL(val) + 1;

	if (offset > 0x0F)
	{
		offset = 0;
	}

	ULONG ret = offset;
	ret = ret << 16;
	ret |= serial;

	return ret;
}

void eeprom_writeout_data(void)
{
	UCHAR  ipl;
	ipl = SRbits.IPL;
	SRbits.IPL = 0b111;

	ULONG ser_data = eeprom_get_next_serial_ex();
	UINT offset = EE_UNPACK_OFFSET(ser_data);
	UINT serial = EE_UNPACK_SERIAL(ser_data);

	eeprom_data.serial = serial;

	/*
	 * We must erase lines before we write to them.
	 */
	eeprom_erase_line((void *) EEPROM_DATA_OFFSETS[offset]);
	eeprom_erase_line((void *) EEPROM_DATA_OFFSETS[offset] + 0x20);

	/*
	 * The eeprom struct is 2 lines in size.  Hence the two writes.
	 */
	eeprom_write_line((void *) EEPROM_DATA_OFFSETS[offset], ((void *) &eeprom_data));
	eeprom_write_line((void *) EEPROM_DATA_OFFSETS[offset] + 0x20, ((void *) &eeprom_data) + 0x20);

	// Reset the dirty flag.
	eeprom_data.is_dirty = 0;

	SRbits.IPL = ipl;

	/*
	 * sizeof gives size in bytes.  We care about size in words.
	 */
	mem_copy_w((void *) &eeprom_data, (void *) &working_eeprom_data, sizeof (eeprom_data_struct) / 2);
	globals_recalc_cal_values();

	return;
}

void eeprom_readin_data(void)
{
	UCHAR  ipl;
	ipl = SRbits.IPL;
	SRbits.IPL = 0b111;

	ULONG serial_data = eeprom_get_max_serial_ex();
	UINT offset = EE_UNPACK_OFFSET(serial_data);

	eeprom_read_line((void *) EEPROM_DATA_OFFSETS[offset], &eeprom_data);
	eeprom_read_line((void *) EEPROM_DATA_OFFSETS[offset] + 0x20, ((void *) &eeprom_data) + 0x20);
	eeprom_data.is_dirty = 0;

	/*
	 * sizeof gives size in bytes.  We care about size in words.
	 */
	mem_copy_w((void *) &eeprom_data, (void *) &working_eeprom_data, sizeof (eeprom_data_struct) / 2);

	SRbits.IPL = ipl;

	return;
}
