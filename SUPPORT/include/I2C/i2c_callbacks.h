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
 * Declarations for the default I2C implementation register callbacks.
 */
#ifndef CALLBACKS_H
#define	CALLBACKS_H

#include "data_types.h"

/**
 * Echo register write callback.
 * \param _char Character to write into the register.
 * \param _idx Byte index.
 */
void i2c_echo_reg_write_cb(UCHAR _char, UINT _idx);

/**
 * Echo register read callback.
 * \param _idx Index of the byte to read from the register.
 * \return Byte read.
 */
UCHAR i2c_echo_reg_read_cb(UINT _idx);

/**
 * Reset register write callback.
 * \param _char Character to write into the register.
 * \param _idx Byte index.
 */
void i2c_reset_reg_write_cb(UCHAR _char, UINT _idx);

/**
 * Null register read callback.  Always reads out zero.
 * Null register is the default space-filler register.  Everything written to it is discarded and reading from it always retuns zero.
 * \param _idx Index of the byte to read from the register.
 * \return Always returns zero.
 */
UCHAR i2c_null_reg_read_cb(UINT _idx);

/**
 * Null register write callback.  Everything written to it is discarded.
 * \param _char Byte to discard into the register.
 * \param _idx Byte index.
 */
void i2c_null_reg_write_cb(UCHAR _char, UINT _idx);

/**
 * Work buffer for the ECHO register.  Defined in i2c.c
 */
extern volatile UCHAR _I2C_REG_ECHO_DB[];

#endif	/* CALLBACKS_H */

