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


    .title "Serial routines for the PIC24"
    .include "p24FV32KA302.inc"
    #include "serial_comm_config.h"
    .text
    .extern _output_buffer

/*
    .global __AddressError
__AddressError:
    PUSH W0
    BCLR INTCON1,#ADDRERR
    POP W0
    RESET
    retfie

    .global __StackError
__StackError:
    NOP
    NOP
    BCLR INTCON1,#STKERR
    RESET
    retfie

    .global __MathError
__MathError:
    NOP
    BCLR INTCON1,#MATHERR
    RESET
    retfie
*/

.macro __read_serial
    PUSH W0			    ; Save the two registers we modify
    PUSH W1			    ;
    PUSH W2

    MOV _ser_input_buffer_a_idx,W0
    MOV _ser_input_buffer_a,W1
    ADD W0,W1,W2

    MOV #SER_IBS,W0		    ; Compare current input buffer index with it's allocated size
2:
    CP _ser_input_buffer_a_idx	    ; If >= has been allocated size bounce to end of routine
    BRA GE,3f			    ; else read in data

    MOV U1RXREG,W1		    ; Read the serial port byte into a register
    MOV.B W1,[W2++]		    ; Move register contents into address in W0

    INC _ser_input_buffer_a_idx	    ; Increment buffer index

    BTSC U1STA,#URXDA		    ; Test to see if there are more bytes that can be read from the serial port
    GOTO 2b			    ; If there are, read them in

3:
    BCLR IFS0,#U1RXIF		; Clear the UART ISR flag

    POP W2
    POP W1			; Restore the modified registers
    POP W0			;
.endm

    .global _ser_switch_input_buffers
_ser_switch_input_buffers:

    DISI #0x08			    ; Disable interrupts while we switch buffers around

    PUSH _ser_input_buffer_a	    ; Swap pointers to the input buffers
    PUSH _ser_input_buffer_b	    ;
    POP _ser_input_buffer_a	    ;
    POP _ser_input_buffer_b	    ;

    PUSH _ser_input_buffer_a_idx    ; Move buffer A index value into buffer B index value
    POP _ser_input_buffer_b_idx

    CLR _ser_input_buffer_a_idx    ; Clear the buffer A index value

    RETURN
/*
Resets the UART overflow flag and updates a counter that keeps track of such overflows.
*/
    .global __U1ErrInterrupt
__U1ErrInterrupt:

    __read_serial

    BCLR U1STA,#OERR		; Clear the overflow flag
    BCLR IFS4, #U1ERIF		; Clear the error interrupt flag
    INC _scs_overflow_count	; Increment the instrumentation counter
    retfie


    .global __ser_init_asm
__ser_init_asm:
    CLR _ser_input_buffer_a_idx
    CLR _ser_input_buffer_b_idx

    MOV #_ser_input_buffer_a_pool,W0
    MOV W0,_ser_input_buffer_a

    MOV #_ser_input_buffer_b_pool,W0
    MOV W0,_ser_input_buffer_b

    RETURN


/*
Dumps the contents of global _output_buffer into the serial stream
Returns value of _ob_idx (number of bytes written).  Sets _last_write_error on failure.
Returns:
    W0 - Number of bytes written
Parameters:
    None
*/
    .global _ser_flush_buffer
_ser_flush_buffer:
    CP0 _ob_idx
    BTSC SR,#Z
    RETURN

    MOV _ob_idx,W1	    ;W1 is our tx_c (transmit count)
    MOV #_output_buffer,W2  ; Move the address of output buffer to W3

1:  ; Top of the loop

    BTSC U1STA,#UTXBF		; Test to see if the UART is ready to receive us.
    GOTO 1b			; If it's not skip to label 2...

    MOV.B [W2++],W0		; Move read-in serial byte into W0 and advance the output buffer
    MOV W0,U1TXREG		; ... and then move W0 into the serial output register
    DEC W1,W1			; Increment the transmit count

    CP0 W1			; Compare _ob_idx with current counter
    BRA NZ, 1b			; Branche if _ob_idx is less than or equal to loop counter
				; Else fall through and write stuff to port

    CLR _ob_idx;		; Reset output buffer index
    RETURN

/*
UART receive interrupt service routine
*/
    .global __U1RXInterrupt
__U1RXInterrupt:
    __read_serial
    RETFIE			; Return from service routine


/*
A linker fixup.  The linker will not see and use the ISRs if nothing else from the object file is referenced.
*/
    .global _ser_linker_fix
_ser_linker_fix:
    NOP
    RETURN


    .bss
    .global _ser_input_buffer_a
_ser_input_buffer_a:
    .space 2
    .align 2

    .bss
    .global _ser_input_buffer_b
_ser_input_buffer_b:
    .space 2
    .align 2

    .bss
    .global _ser_input_buffer_a_pool
_ser_input_buffer_a_pool:
    .space #SER_IBS
    .align 2

    .bss
    .global _ser_input_buffer_b_pool
_ser_input_buffer_b_pool:
    .space #SER_IBS
    .align 2

    .bss
    .global _ser_input_buffer_a_idx
_ser_input_buffer_a_idx:
    .space 2
    .align 2

    .bss
    .global _ser_input_buffer_b_idx
_ser_input_buffer_b_idx:
    .space 2
    .align 2



/*
Serial communications status - overflow count.  Incremented whenever the error ISR resets the OERR flag on the UART
*/
    .bss
    .global _scs_overflow_count
_scs_overflow_count:
    .space 2
    .align 2

