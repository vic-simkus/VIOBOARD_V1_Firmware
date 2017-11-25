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

/**
 * \file
 * \author Vidas Simkus (vic.simkus@simkus.com)
 *
 * Globals
 */
#include "support.h"
#include "ioconfig.h"
#include "support.h"

#ifndef GLOBALS_H
#define	GLOBALS_H
#ifndef __GLOBALS_H_INT
#define GI_EXT extern
#else
#define GI_EXT
#endif

/**
 * Buffer containing the analog input values.  Populated by the ADC ISR.
 */
GI_EXT volatile UINT AD_BUFFER[AD_INPUT_NUM];

GI_EXT void init_globals(void);

/**
 * AI calibration values.
 */
typedef struct {
	UINT cal_0;
	UINT cal_1;
	UINT cal_2;
	UINT cal_3;
	UINT cal_4;
	UINT cal_5;
	UINT cal_6;
	UINT cal_7;

} cal_data_struct;

/**
 * Data structure we keep in EEPROM.
 * The size of the structure must be EEPROM line-aligned since we do all writing and reading one line at a time.
 * Hence the pad array at the end of the struct.
 * In other words the size of the struct in words must be a multiple of 16.
 */
typedef struct _eeprom_data_struct {
	/**
	 * Has the struct been initialized.  An initialized struct has a value of 0xDEAD in this field.
	 */
	UINT eeprom_init;	// 1

	/**
	 * How many times has the IOCONTROLLER booted since the struct been initialized.
	 */
	UINT boot_count;	// 2

	/**
	 * The array is for fast indexing into the calibration data by the ISR.
	 * The individual entries are there for meat-bag consumption.
	 * Yes I know I could index into it without the union by doing basic arithmetic, but that's the kind of hard-to read smart shit I deliberately avoid in the code.
	 */
	union {
		cal_data_struct l1_cal_data;
		UINT l1_cal_data_arr[AD_INPUT_NUM]; // 8 (3-10)
	};

	/**
	 * The array is for fast indexing into the calibration data by the ISR.
	 * The individual entries are there for meat-bag consumption.
	 */
	union {
		cal_data_struct l2_cal_data;
		UINT l2_cal_data_arr[AD_INPUT_NUM]; // 8 (11-18)
	};

	UINT is_dirty;	//19

	/**
	 * Pad buffer.
	 * NOTE - make sure to decrement as fields are added
	 */
	UINT pad[13];
} eeprom_data_struct;

/**
 * Number of timer clicks since the last digital output status confirmation message.
 */

volatile GI_EXT UINT confirm_clicks_passed;

GI_EXT eeprom_data_struct __attribute__((aligned)) eeprom_data;
GI_EXT eeprom_data_struct __attribute__((aligned)) working_eeprom_data;

#endif	/* GLOBALS_H */

