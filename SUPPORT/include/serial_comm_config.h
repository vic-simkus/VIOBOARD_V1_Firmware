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
 \file

 \author Vidas Simkus (vic.simkus@simkus.com)

 */

#ifndef SER_IBS
/**
Input buffer size.  Since the input buffer is double buffered the actual size requirement is twice the specified amount.
 */
#define SER_IBS 32
#endif

#ifndef SER_OBS
/**
Output buffer size.
 */
#define SER_OBS 32


/**
 * Various error conditions that the library may encounter.
 * We use defines because we reference these values in assembler code and it doesn't understand enums
 */

#define	SER_ERR_NONE		0		/// No error
#define SER_ERR_R_OVERFLOW	1		/// Overflow on read.  The read buffer is full.
#define SER_ERR_W_TIMEOUT	2		/// Time out on write.

#endif

