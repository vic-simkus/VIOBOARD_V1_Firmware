/*
Vic's IO Board V1; Firmware

Copyright (C) 2019 Vidas Simkus (vic.simkus@simkus.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

.ifdef __30F4013
    .include "p30F4013.inc"
.endif

.ifdef __24FV32KA302
    .include "p24FV32KA302.inc"
.endif

    #include "eeprom_config.h"

    .title  "EEPROM management routines"

    .text

    .global _eeprom_erase_all
    .global _eeprom_erase_word
    .global _eeprom_erase_line
    .global _eeprom_write_word
    .global _eeprom_write_line
    .global _eeprom_read_line
    .global _eeprom_read_word

/**
Disables the interrupts for a certain number of instructions, performs the EEPROM
ulock dance, and initiates the write.
*/
.macro __unlock_eeprom_reg
    DISI #5
    MOV #NVMKEY_A, W0
    MOV W0, NVMKEY	; Push out first key
    MOV #NVMKEY_B, W0
    MOV W0, NVMKEY	; Push out second key
    BSET NVMCON,#WR	; Set the WR bit of NVMCON
.endm

/**
Waits for the last EEPROM operation to finish.
*/
.macro __wait_for_eeprom
1:
    BTSC NVMCON, #WR	; Test to see if erase is complete (bit test skip if clear)
    GOTO 1b		; Not clear, go back to the previous line
.endm

/**
Erases the whole EEPROM.
Interrupts are disabled until the erase finishes.
Parameters:
    None
Returns:
    Nothing
*/
_eeprom_erase_all:
    __wait_for_eeprom

    MOV #NVMCON_EE_ERASE_ALL,W0
    MOV W0,NVMCON

    __unlock_eeprom_reg

    NOP			; Do we need these two NOPs or is that only for erasing flash?
    NOP

    RETURN

/**
Erases a word in the EEPROM.
All addressing is taken care of.  The caller does not need to make any special considerations with exception of passing a properly aligned address.
Interrupts are disabled until the erase finishes.
Parameters:
    W0 - Word-aligned pointer into the EEPROM address space to be cleared.  Meaning that first word is 0x7FFC00, next word is 0x7FFC02, and so forth in that manner
Return:
    Nothing
*/
_eeprom_erase_word:
    __wait_for_eeprom

    MOV	W0, NVMADR	    ; Setup the lower 16 bits of the EEPROM address to be erased
    MOV	#EEPROM_ADD_MSB,W0  ; Setup the upper 8 bits of the EEPROM address to be erased
    MOV	W0, NVMADRU	    ;

    MOV #NVMCON_EE_ERASE_WORD,W0    ; Set the erase word command
    MOV W0,NVMCON

    __unlock_eeprom_reg

    RETURN

/**
Erases a line in the EEPROM.  A line is 16 words (32 bytes).
All addressing is taken care of.  The caller does not need to make any special considerations with exception of passing a properly aligned address.
Interrupts are disabled until the erase finishes.
Parameters:
    W0 - A line-aligned pointer into the EEPROM address space to be cleared.  First line is at 0x7FFC00, second line is at 0x7FFC20, and so forth.
	Reason, of course, is that there are 32 bytes per line and 0x20 is 32 in decimal.
Return:
    Nothing
*/

_eeprom_erase_line:
    __wait_for_eeprom

    MOV	W0, NVMADR	    ; Setup the lower 16 bits of the EEPROM address to be erased
    MOV	#EEPROM_ADD_MSB,W0  ; Setup the upper 8 bits ofNVMCON the EEPROM address to be erased
    MOV	W0, NVMADRU	    ;

    MOV #NVMCON_EE_ERASE_LINE,W0    ; Set the erase word command
    MOV W0,NVMCON

    __unlock_eeprom_reg

    RETURN

/**
Writes a word into the EEPROM.  Does not erase the word before writing.
Interrupts are disabled until the write finishes.
Parameters:
    W0 - A word-aligned pointer into the EEPROM address space to be overwritten.  Meaning that first word is 0x7FFC00, next word is 0x7FFC02, and so forth in that manner
    W1 - Word to be written
Return:
    Nothing.
*/
_eeprom_write_word:
    __wait_for_eeprom

    PUSH W0		    ; Save W0 because we're about to molest it

    MOV #EEPROM_ADD_MSB,W0  ; Move the EEPROM address MSB into TBLPAG
    MOV W0, TBLPAG	    ;

    MOV #NVMCON_EE_PROG_WORD, W0
    MOV W0, NVMCON

    POP W0		    ; Restore W0 which contains the EEPROM target address
    TBLWTL W1,[W0]	    ; Write into the EEPROM address (holding latch)

   __unlock_eeprom_reg

    RETURN

/**
Writes a line into the EEPROM.  Does not erase the line before writing.
Interrupts are disabled until the write finishes.
Parameters:
    W0 - A line-aligned pointer into the EEPROM address space to be written to.  First line is at 0x7FFC00, second line is at 0x7FFC20, and so forth.
    W1 - A pointer to the source of the data that contains 16 words to be read.  Alignment does not matter, but it must be in the data memory.  No PSV stuff is done by this function.
Return:
    Nothing
*/
_eeprom_write_line:
    __wait_for_eeprom

    PUSH W0		    ; Save W0 because we're about to molest it

    MOV #EEPROM_ADD_MSB,W0  ; Move the EEPROM address MSB into TBLPAG
    MOV W0, TBLPAG	    ;

    MOV #NVMCON_EE_PROG_LINE, W0
    MOV W0, NVMCON

    POP W0		    ; Restore W0 which contains the EEPROM target address
    REPEAT #15
    TBLWTL [W1++],[W0++]	    ; Write into the EEPROM address (holding latch)

    __unlock_eeprom_reg

    RETURN

/**
Reads a line from the EEPROM.
Parameters:
    W0 - A line aligned pointer into the EEPROM address space.  First line is at 0x7FFC00, second line is at 0x7FFC20 and so forth.
    W1 - A pointer to the destination of the write.  It should be aligned on a word boundry.  No PSV stuff is done by this function.
*/
_eeprom_read_line:
    __wait_for_eeprom

    PUSH W0

    MOV #EEPROM_ADD_MSB,W0  ; Move the EEPROM address MSB into TBLPAG
    MOV W0, TBLPAG	    ;

    POP W0

    REPEAT #15
    TBLRDL [W0++],[W1++]

    RETURN

/**
Reads a word from the EEPROM.
Parameters:
    W0 - A word aligned pointer into the EEPROM data space.
Return
    The read word in W0
*/
_eeprom_read_word:
    __wait_for_eeprom

    PUSH W0
    MOV #EEPROM_ADD_MSB,W0
    MOV W0, TBLPAG
    POP W0

    TBLRDL [W0],W0

    RETURN

; EOF


