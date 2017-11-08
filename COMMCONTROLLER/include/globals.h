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

#ifndef GLOBALS_H
#define	GLOBALS_H

#include "support.h"
#include "config.h"
#include "iocontroller_interface.h"

#ifndef __GLOBALS_H_INT
#define G_EXP extern
#else
#define G_EXP
#endif


/**
 * Map between terminal strip inputs and actually ADC channels on the IOCONTROLLER
 * The values returned by the IOCONTROLLER are in port order AN0, AN1...
 * This array maps those to terminal strip positions.
 *
 * IC_IO_MAP[0] = 6 means that ADC input 0 (first ADC input) maps to the 7th input on the terminal strip.

 * To make things even more difficult, terminal strip positions are numbered 1 through 8
 * while this map is index starting at 0.  Neato!
 */
//G_EXP volatile UCHAR IOC_IO_MAP[IOC_AI_COUNT];
G_EXP volatile UINT IOC_AI_RAW_VALUES[IOC_AI_COUNT];
G_EXP volatile float IOC_AI_VOLT_VALUES[IOC_AI_COUNT];
G_EXP volatile UINT IOC_CAL_VALUES[IOC_AI_COUNT];

G_EXP float ioc_step_value;

/**
 * The full, unmolested command buffer at the time that command processing was triggered.
 */
G_EXP UCHAR __attribute__((aligned)) mg_command_buffer[COMMAND_BUFFER_SIZE];

/**
 * Containts a list of indexes to the separator character in the command buffer
 * For example.  If the command buffer contains (minus the quotes) "1 2 3"
 * The separator buffer will contain [1,3].
 */
G_EXP UCHAR __attribute__((aligned)) mg_separator_buffer[COMMAND_SEPARATOR_BUFFER_SIZE];

/**
 * Number of entries in the separator index buffer.
 */
G_EXP UINT mg_sep_buffer_idx;

/*
 * Number of characters in the command buffer.
 */
G_EXP UINT mg_cmd_buffer_idx;


G_EXP void init_globals(void);


G_EXP UCHAR APP_CONFIG_VERBOSE;

G_EXP UINT mg_sep_buffer_idx;
G_EXP UINT mg_cmd_buffer_idx;

#endif	/* GLOBALS_H */

