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

#ifndef EEPROM_UTILS_H
#define	EEPROM_UTILS_H

#include "data_types.h"

/**
 * \file
 *
 * \author Vidas Simkus (vic.simkus@simkus.com)
 *
 * High-level EEPROM manipulation functions.
 *
 */

/**
 * Initializes a new instance of the persistent data and writes it out to the EEPROM.
 */
void eeprom_initialize_data(void);

/**
 * Writes out the current instance of the persistent data to EEPROM.  Wear leveling is accounted for.
 */
void eeprom_writeout_data(void);

/**
 * Reads in the latest instance of the persistent data from EEPROM.
 */
void eeprom_readin_data(void);

/**
 * Searches the EPPROM data and returns the offset where the next instance should be written to and that instances serial number.
 * \return Two 16 bit values packed into a 32 bit integer.  The upper 16 bits is the index into the offset array.  The lower 16 bits is the serial number.
 */
ULONG eeprom_get_next_serial_ex(void);

/**
 * Searches the EEPROM data and returns the offset and serial number of the most resent instance that is stored in EEPROM.
 * \return Two 16 bit values packed into a 32 bit integer.  The upper 16 bits is the index into the offset array.  The lower 16 bits is the serial number.
 */
ULONG eeprom_get_current_serial_ex(void);

/**
 * Extracts the index into the pre-calculated offset array from the return value of the eeprom_get_*_ex methods that return a ULONG.
 * \see EEPROM_DATA_OFFSETS
 */
#define EE_UNPACK_OFFSET(_val) ((_val >> 16) & 0x00FF)
/**
 * Extracts the serial number from the return value of the eeprom_get_*_ex methods that return a ULONG.
 */
#define EE_UNPACK_SERIAL(_val) ((_val & 0x00FF))

#endif	/* EEPROM_UTILS_H */

