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

    .title  "Support routines"
    .global _mem_clear
    .global _mem_copy
    .global _mem_copy_w
    .text


/**
Copies memory contents from one pointers to another one word at a time.
Parameters:
    W0 - Pointer to source memory
    W1 - Pointer to destination memory
    W2 - Count, in words, of memory to copy
Returns:
    Pointer to the same pointer as passed in W0
*/
_mem_copy_w:
    PUSH W0

    CLR W3		    ; Check to see if W1 is zero
    CPSNE W2,W3		    ; If it is bounce.
    GOTO 1f		    ; Otherwise DEC overflows it and we overwrite important stuff

    DEC W2,W2		    ; REPEAT repeats instruction N+1 times
    REPEAT W2		    ;
    MOV [W0++],[W1++]	    ; Move the word and increment
			    ; Since we copy by word we expect the pointers to be word-aligned

1:
    POP W0		    ; Return the same address we were passed.  Why?  Because
    RETURN


/**
Copies memory contents from one pointers to another one byte at a time.
Parameters:
    W0 - Pointer to source memory
    W1 - Pointer to destination memory
    W2 - Count, in bytes, of memory to copy
Returns:
    Pointer to the same pointer as passed in W0
*/
_mem_copy:
    PUSH W0

    CLR W3		    ; Check to see if W1 is zero
    CPSNE W2,W3		    ; If it is bounce.
    GOTO 1f		    ; Otherwise DEC overflows it and we overwrite important stuff

    DEC W2,W2		    ; REPEAT repeats instruction N+1 times
    REPEAT W2		    ;
    MOV.B [W0++],[W1++]    ; Move the byte and increment
			    ; We copy by byte because the pointer may not point to a word-aligned space

1:
    POP W0		    ; Return the same address we were passed.  Why?  Because
    RETURN


/**
Clears the memory at the supplied addres
Parameters:
    W0 - Pointer to memory
    W1 - Length of memory to clear
Returns:
    Pointer to the same pointer as passed in W0
*/
_mem_clear:
    PUSH W0	    ; Save W0 because we will be molesting it

    CLR W4	    ; Check to see if W1 is zero
    CPSNE W1,W4	    ; If it is bounce.
    GOTO 1f	    ; Otherwise DEC overflows it and we overwrite important stuff

    DEC W1, W3	    ; REPEAT repeats instruction N+1 times
    REPEAT W3	    ;
    CLR.B [W0++]    ; Clear the byte at address and increment
		    ; We clear by byte because the pointer may not point to a word-aligned space
1:
    POP W0	    ; Return the same address we were passed.  Why?  Because
    RETURN

/* EOF */