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

.ifdef __30F4013
    .include "p30F4013.inc"
.endif

.ifdef __24FV32KA302
    .include "p24FV32KA302.inc"
.endif

    .title  "EEPROM management routines"

    .text

    .global _eeprom_erase_all
    .global _eeprom_erase_word
    .global _eeprom_erase_line
    .global _eeprom_write_word
    .global _eeprom_write_line
    .global _eeprom_read_line

    .equiv NVMKEY_A, 0x55
    .equiv NVMKEY_B, 0xAA

    .equiv NVMCON_EE_ERASE_WORD,    0x4044  ; NVMCON register value to erase one EEPROM word (16 bits)
    .equiv NVMCON_EE_ERASE_LINE,    0x4045  ; NVMCON register value to erase one EEPROM line (16 words, 256 bits)
    .equiv NVMCON_EE_ERASE_ALL,	    0x4046  ; NVMCON register value to erase whole EEPROM
    .equiv NVMCON_EE_PROG_WORD,	    0x4004  ; NVMCON register value to write one EEPROM word (16 bits)
    .equiv NVMCON_EE_PROG_LINE,	    0x4005  ; NVMCON register value to write one EEPROM line (16 words, 256 bits)
    .equiv EEPROM_ADD_MSB,	    0x007F  ; Most significant byte of a 24 bit address of the EEPROM address range
					    ; The lower 16 bits will be the pointer to the actual memory
    .equiv IPL_DIS_INT,	    0x00E0  ; 0b1110 used to set the SR.IPL to 111 thus disabling interrupts

/*
Erases the whole EEPROM.
Interrupts are disabled until the erase finishes.
Parameters:
    None
Returns:
    Nothing
*/
_eeprom_erase_all:

    MOV #NVMCON_EE_ERASE_ALL,W0
    MOV W0,NVMCON

    PUSH SR		; Save SR
    MOV #IPL_DIS_INT,W0	; Move 0b1110 0000 into W0
    IOR SR		; OR SR with W0 placing result in SR setting SR.IPL to 111 and effectively disabling the interrupts

    MOV #NVMKEY_A, W0
    MOV W0, NVMKEY	; Push out first key
    MOV #NVMKEY_B, W0
    MOV W0, NVMKEY	; Push out second key
    BSET NVMCON,#WR	; Set the WR bit of NVMCON
    NOP			; Do we need these two NOPs or is that only for erasing flash?
    NOP
    POP SR		; Restore SR

1:
    BTSC NVMCON, #WR	; Test to see if erase is complete (bit test skip if clear)
    GOTO 1b		; Not clear, go back to the previous line

    RETURN

/*
Erases a word in the EEPROM.
All addressing is taken care of.  The caller does not need to make any special considerations with exception of passing a properly aligned address.
Interrupts are disabled until the erase finishes.
Parameters:
    W0 - Word-aligned pointer into the EEPROM address space to be cleared.  Meaning that first word is 0x7FFC00, next word is 0x7FFC02, and so forth in that manner
Return:
    Nothing
*/
_eeprom_erase_word:
    MOV	W0, NVMADR	    ; Setup the lower 16 bits of the EEPROM address to be erased
    MOV	#EEPROM_ADD_MSB,W0  ; Setup the upper 8 bits of the EEPROM address to be erased
    MOV	W0, NVMADRU	    ;

    MOV #NVMCON_EE_ERASE_WORD,W0    ; Set the erase word command
    MOV W0,NVMCON

    PUSH SR		    ; Dsiable interrupts
    MOV #IPL_DIS_INT,W0
    IOR SR

    MOV #NVMKEY_A, W0
    MOV W0, NVMKEY	; Push out first key
    MOV #NVMKEY_B, W0
    MOV W0, NVMKEY	; Push out second key
    BSET NVMCON,#WR	; Set the WR bit of NVMCON

    POP SR

1:
    BTSC NVMCON, #WR	; Test to see if erase is complete (bit test skip if clear)
    GOTO 1b		; Not clear, go back to the previous line

    RETURN

/*
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
    MOV	W0, NVMADR	    ; Setup the lower 16 bits of the EEPROM address to be erased
    MOV	#EEPROM_ADD_MSB,W0  ; Setup the upper 8 bits of the EEPROM address to be erased
    MOV	W0, NVMADRU	    ;

    MOV #NVMCON_EE_ERASE_LINE,W0    ; Set the erase word command
    MOV W0,NVMCON

    PUSH SR		    ; Dsiable interrupts
    MOV #IPL_DIS_INT,W0
    IOR SR

    MOV #NVMKEY_A, W0
    MOV W0, NVMKEY	; Push out first key
    MOV #NVMKEY_B, W0
    MOV W0, NVMKEY	; Push out second key
    BSET NVMCON,#WR	; Set the WR bit of NVMCON

    POP SR

1:
    BTSC NVMCON, #WR	; Test to see if erase is complete (bit test skip if clear)
    GOTO 1b		; Not clear, go back to the previous line

    RETURN

/*
Writes a word into the EEPROM.  Does erase the word before writing.
Interrupts are disabled until the write finishes.
Parameters:
    W0 - A word-aligned pointer into the EEPROM address space to be overwritten.  Meaning that first word is 0x7FFC00, next word is 0x7FFC02, and so forth in that manner
    W1 - Word to be written
Return:
    Nothing.
*/
_eeprom_write_word:
    PUSH W0		    ; Save W0 because we're about to molest it

    RCALL _eeprom_erase_word

    MOV #EEPROM_ADD_MSB,W0  ; Move the EEPROM address MSB into TBLPAG
    MOV W0, TBLPAG	    ;

    MOV #NVMCON_EE_PROG_WORD, W0
    MOV W0, NVMCON

    POP W0		    ; Restore W0 which contains the EEPROM target address
    TBLWTL W1,[W0]	    ; Write into the EEPROM address (holding latch)

    PUSH SR		    ; Disable interrupts
    MOV #IPL_DIS_INT,W0
    IOR SR

    MOV #NVMKEY_A, W0
    MOV W0, NVMKEY	; Push out first key
    MOV #NVMKEY_B, W0
    MOV W0, NVMKEY	; Push out second key
    BSET NVMCON,#WR	; Set the WR bit of NVMCON

    POP SR

1:
    BTSC NVMCON, #WR	; Test to see if erase is complete (bit test skip if clear)
    GOTO 1b		; Not clear, go back to the previous line

    RETURN

/*
Writes a line into the EEPROM.  Does erase the line before writing.
Interrupts are disabled until the write finishes.
Parameters:
    W0 - A line-aligned pointer into the EEPROM address space to be written to.  First line is at 0x7FFC00, second line is at 0x7FFC20, and so forth.
    W1 - A pointer to the source of the data that contains 16 words to be read.  Alignment does not matter, but it must be in the data memory.  No PSV stuff is done by this function.
Return:
    Nothing
*/
_eeprom_write_line:
    PUSH W0

    RCALL _eeprom_erase_line

    MOV #EEPROM_ADD_MSB,W0  ; Move the EEPROM address MSB into TBLPAG
    MOV W0, TBLPAG	    ;

    MOV #NVMCON_EE_PROG_LINE, W0
    MOV W0, NVMCON

    POP W0

    REPEAT #15
    TBLWTL [W1++],[W0++]

    PUSH SR		    ; Disable interrupts
    MOV #IPL_DIS_INT,W0
    IOR SR

    MOV #NVMKEY_A, W0
    MOV W0, NVMKEY	; Push out first key
    MOV #NVMKEY_B, W0
    MOV W0, NVMKEY	; Push out second key
    BSET NVMCON,#WR	; Set the WR bit of NVMCON

    POP SR

1:
    BTSC NVMCON, #WR	; Test to see if erase is complete (bit test skip if clear)
    GOTO 1b		; Not clear, go back to the previous line

    RETURN

_eeprom_read_line:
    PUSH W0

    MOV #EEPROM_ADD_MSB,W0  ; Move the EEPROM address MSB into TBLPAG
    MOV W0, TBLPAG	    ;

    POP W0

    REPEAT #15
    TBLRDL [W0++],[W1++]

    RETURN

; EOF


