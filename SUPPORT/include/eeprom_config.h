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
\author Vidas Simkus vic.simkus@simkus.com
\brief EEPROM-related constants.  Shared between C and ASM code.
*/

#ifndef EEPROM_CONFIG_H
#define	EEPROM_CONFIG_H

#define EEPROM_START 0xFC00				/// Start address of our EEPROM data.  Is there a more "portable" way of doing this?  Probably.

#define NVMKEY_A 0x55					/// Magic key 'A' to unlock EEPROM.
#define NVMKEY_B 0xAA					/// Magic key 'B' to unlock EEPROM.

#define NVMCON_EE_ERASE_WORD	0x4044  /// NVMCON register value to erase one EEPROM word (16 bits)
#define NVMCON_EE_ERASE_LINE	0x4045  /// NVMCON register value to erase one EEPROM line (16 words, 256 bits)
#define NVMCON_EE_ERASE_ALL		0x4046  /// NVMCON register value to erase whole EEPROM
#define NVMCON_EE_PROG_WORD		0x4004  /// NVMCON register value to write one EEPROM word (16 bits)
#define NVMCON_EE_PROG_LINE		0x4005  /// NVMCON register value to write one EEPROM line (16 words, 256 bits)
#define EEPROM_ADD_MSB			0x007F  /// Most significant byte of a 24 bit address of the EEPROM address range.  The lower 16 bits will be the pointer to the actual memory
#define EEPROM_PAGE	EEPROM_ADD_MSB		/// More consistant wording  Same as EEPROM_ADD_MSB
#define IPL_DIS_INT			    0x00E0	/// 0b1110 used to set the SR.IPL to 111 thus disabling interrupts

/*
 * the dsPIC4013 has 1024 bytes of EEPROM.
 * - 512 words; two bytes per 16bit word (1024 / 2)
 * - 32 lines; each line is 16 words (512 / 16)
 * - each line is 32 bytes (16 * 2)
 *
 * EEPROM starts at 0xFC00 and ends at 0xFFF0
 *
 */

#define EEPROM_SIZE_BYTES	1024						/// EEPROM size in bytes
#define EEPROM_SIZE_WORDS	(EEPROM_SIZE_BYTES / 2)		/// EEPROM size in words: (1024 bytes / 2 bytes per word) = 512 words
#define EEPROM_SIZE_LINES	(EEPROM_SIZE_WORDS / 16)	/// EEPROM size in lines: (512 words / 16 words per line) = 32 lines

#endif	/* EEPROM_CONFIG_H */

