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
\brief Misc support routines
 */

#ifndef __SUPPORT_H
#define __SUPPORT_H

#include "data_types.h"

#ifdef SUPPORT_INC_SERIAL_COMM
#include "serial_comm.h"
#endif

/**
Rotates a byte one bit to the right without using the cary flag
 */
#define ROTR_B(_byte) asm("RRNC.B %0,%0" : "+r"(_byte) : "r"(_byte))

/**
Rotates a byte one bit to the left without using the cary flag
 */
#define ROTL_B(_byte) asm("RLNC.B %0,%0" : "+r"(_byte) : "r"(_byte))

/**
Swaps the nibbles on a byte
 */
#define SWAP_B(_byte) asm("SWAP.B %0" : "+r"(_byte) : "r"(_byte))

/**
Rotates a byte a specified number of bits to the left
 */
extern inline unsigned char rotl_b(unsigned char _byte, unsigned char _num);

/**
Rotates a byte a specified number of bits to the right
 */
extern inline unsigned char rotr_b(unsigned char _byte, unsigned char _num);

/**
 * Clears a chunk of memory by setting each byte to zero.
 * This function works a byte at a time.
 * \param _ptr Pointer to a block of memory to be cleared.  Does not need to be word aligned as the method uses byte instructions.
 * \return Same pointer as pointer parameter.
 */
extern void * mem_clear(void * _ptr, UINT _size);

/**
 * Copies memory one byte at a time from one buffer to another.
 * \param _ptr_source Pointer to the source block.  Does not need to be word aligned.
 * \param _ptr_dest Pointer to the destination block.  Does not need to be word aligned.
 * \param _size Number of bytes to be copied.
 * \return Same pointer as pointer as source parameter.
 */
extern void * mem_copy(const void * _ptr_source, void * _ptr_dest, UINT _size);

/**
 *
 * \param _ptr_source Pointer to the source block.  The block does have to be word aligned.
 * \param _ptr_dest Pointer to the destination block.  The block does have to be word aligned.
 * \param _size Number of words to be copied.
 * \return Same pointer as pointer as source parameter.
 */
extern void * mem_copy_w(const void * _ptr_source, void * _ptr_dest, UINT _size);

extern void ftoa (char *buf, double fval, int cField, int cPlaces);

#define EEPROM_START_ADDR  0xFC00

/**
 * Erases the whole EEPROM.
 * Implemented in assembler.
 */
extern void eeprom_erase_all(void);

/**
 * Erases an EEPROM word.
 * Implemented in assembler.
 * \param _addr Word-aligned pointer into the EEPROM address space to be cleared.  Meaning that first word is 0x7FFC00, next word is 0x7FFC02, and so forth in that manner.
 */
extern void eeprom_erase_word(void * _addr);

/**
 * Erases an EEPROM line.
 * Implemented in assembler.
 * \param _addr A line-aligned pointer into the EEPROM address space to be cleared.  First line is at 0x7FFC00, second line is at 0x7FFC20, and so forth. Reason, of course, is that there are 32 bytes per line and 0x20 is 32 in decimal.
 */
extern void eeprom_erase_line(void * _addr);

/**
 * Writes a word into the EEPROM.  Does erase the word before writing.
 * \param A word-aligned pointer into the EEPROM address space to be overwritten.  Meaning that first word is 0x7FFC00, next word is 0x7FFC02, and so forth in that manner.
 * \param Word to be written
 */
extern void eeprom_write_word(void * _addr, unsigned int _value);

/**
 * Writes a line into the EEPROM.  Does erase the line before writing.
 * \param _target_addr A line-aligned pointer into the EEPROM address space to be written to.  First line is at 0x7FFC00, second line is at 0x7FFC20, and so forth.
 * \param _source_addr A pointer to the source of the data that contains 16 words to be read.  Alignment does not matter, but it must be in the data memory.  No PSV stuff is done by this function.
 */
extern void eeprom_write_line(void * _target_addr, void * _source_addr);

/**
 * Reads a line from the EEPROM.
 * \param _source_addr A line-aligned pointer into the EEPROM address space to read from.
 * \param _target_addr Pointer to the destination buffer to write to.
 */
extern void eeprom_read_line(void * _source_addr, void * _target_addr);

#endif
