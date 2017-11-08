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

/**
 * \file
 *
 * \author Vidas Simkus (vic.simkus@simkus.com)
 *
 * High-level EEPROM manipulation functions.
 *
 */

void eeprom_initialize_data(void);
void eeprom_writeout_data(void);
void eeprom_readin_data(void);

#endif	/* EEPROM_UTILS_H */

