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

/*
Resets the UART overflow flag and updates a counter that keeps track of such overflows.
*/
    .global __U1ErrInterrupt
__U1ErrInterrupt:
    BCLR U1STA,#OERR		; Clear the overflow flag
    BCLR IFS4, #U1ERIF		; Clear the error interrupt flag
    INC _scs_overflow_count	; Increment the instrumentation counter
    retfie


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
    CLR W1			; W1 is our tx_c (transmit count)
    CLR W2			; W2 is our time out counter
    CLR _last_write_error	; Clear the last write error.
    MOV _output_buffer_ptr,W3   ; Move the address of output buffer to W3

1:  ; Top of the loop
    MOV W1,W0			; Setup for comparison
    CP _ob_idx			; Compare _ob_idx with current counter
    BRA LE, 3f			; Branche if _ob_idx is less than or equal to loop counter
				; Else fall through and write stuff to port

    BTSC U1STA,#UTXBF		; Test to see if the UART is ready to receive us.
				; We use the TRMT flag because UTXBF is broken according to silicon errata.
				; Using UXBF is confirmed to break stuff
    GOTO 2f			; If it's not skip to label 2...

    MOV.B [W3++],W0		; Move read-in serial byte into W0 and advance the output buffer
    MOV W0,U1TXREG		; ... and then move W0 into the serial output register
    ADD #1,W1			; Increment the transmit count
    CLR _timeout_clicks		; Since we successfully wrote something to somewehere clear the timeout counter

    BRA 1b			; Loop

2:  ; Serial stream is not ready to receive
    ADD #1, W2			; Increment the timeout counter
    MOV W2,W0			; Move to W0 in preparation for comparison
    CP _timeout_clicks		; Compare with global variable
    BRA LT,1b			; If the timeout counter has not been exceeded continue with the loop
				; If the timeout counter has been exceeded drop through and return

    ; We're here because we've fallen through the comparison above
    MOV #SER_ERR_W_TIMEOUT,W0	; Shove timeout error into W0
    MOV W0,_last_write_error	; Set error to timeout

3:  ; Exit of the loop
    CLR _ob_idx;		; Reset output buffer index
    RETURN

/*
UART receive interrupt service routine
*/
    .global __U1RXInterrupt
__U1RXInterrupt:
    PUSH W0			; Save the two registers we modify
    PUSH W1			;

2:
    MOV #SER_IBS,W0		; Compare current input buffer index with it's allocated size
    CP _ib_idx			; If >= has been allocated size bounce to end of routine
    BRA GE,3f			; else read in data

    MOV _ib_idx,W1		; Setup the buffer pointer to be incremented
    MOV _input_buffer_ptr,W0	;
    ADD W0,W1,W0		; Increment the buffer pointer
				;
				; XXX - Can we get rid of the pointer increment here by incrementing the pointer on write below...
				; This would save us two cycles per byte read

    MOV U1RXREG,W1		; Read the serial port byte into a register
    MOV.B W1,[W0]		; Move register contents into address in W0

    INC _ib_idx			; Increment buffer index

    BTSC U1STA,#URXDA		; Test to see if there are more bytes that can be read from the serial port
    BRA 2b			; If there are, read them in

1:
    MOV _scs_buff_watermark,W0	; Setup for watermark compare
    MOV _ib_idx,W1		;
    CPSGT W0,W1			; Compare...
    MOV W1,_scs_buff_watermark	; If the current buffer index is greater than the previous watermark, update the watermark instrumentation
				; If the current buffer is not greater than the previous watermark, fall through

3:
    BCLR IFS0,#U1RXIF		; Clear the UART ISR flag

    POP W1			; Restore the modified registers
    POP W0			;

    RETFIE			; Return from service routine


/*
A linker fixup.  The linker will not see and use the ISRs if nothing else from the object file is referenced.
*/
    .global _ser_linker_fix
_ser_linker_fix:
    NOP
    RETURN

/*
Serial communications status - overflow count.  Incremented whenever the error ISR resets the OERR flag on the UART
*/
    .bss
    .global _scs_overflow_count
_scs_overflow_count:
    .space 2
    .align 2

/*
Serial communication status - buffer watermark.  The highest buffer offset that is generated.  Used to size the input buffer and just because we're curious.
*/
    .bss
    .global _scs_buff_watermark
_scs_buff_watermark:
    .space 2
    .align 2