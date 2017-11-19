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
    .global __U1ErrInterrupt
__U1ErrInterrupt:
    BCLR U1STA,#OERR
    BCLR IFS4, #U1ERIF
    INC _scs_overflow_count
    retfie

;
; Dumps the contents of global _output_buffer into the serial stream
; Resets _ob_idx on success.
; Returns:
;   W0 - Number of bytes written

    .global _ser_flush_buffer
_ser_flush_buffer:
    CLR W1			; W1 is our tx_c (transmit count)
    CLR W2			; W2 is our time out counter
    CLR _last_write_error	; Clear the last write error.  We don't set it, but

    MOV _output_buffer_ptr,W3   ; Move the address of output buffer to W3


1:  ; Top of the loop

    MOV W1,W0			; Setup for comparison
    CP _ob_idx			; Compare _ob_idx with current counter
    BRA LE, 3f			; Brance if _ob_idx is less than or equal to loop counter
				; Else fall through and write stuff to port

    BTSS U1STA,#TRMT		; Test to see if the UART is ready to receive us.
				; We use the TRMT flag because UTXBF is broken according to silicon errata.
				; Using UXBF is confirmed to break stuff
    GOTO 2f			; If it's not skip to label 2...

    MOV.B [W3++],W0		; Move read-in serial byte into W0 and advance the output buffer
    MOV W0,U1TXREG		; ... and then move W0 into the serial output register
    ADD #1,W1			; Increment the transmit count
    CLR _timeout_clicks		; Since we successfully wrote something to somewehere clear the timeout counter

    BRA 1b


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

    CLR _ob_idx;		; Clear output buffer index
    RETURN

    .global __U1RXInterrupt
__U1RXInterrupt:

    PUSH W0
    PUSH W1

2:
    MOV #SER_IBS,W0	    ; Compare current input buffer index with it's allocated size
    CP _ib_idx		    ; If >= has been allocated size bounce to end of routine
    BRA GE,1f		    ; else read in data

    MOV _ib_idx,W1
    MOV _input_buffer_ptr,W0
    ADD W0,W1,W0

    MOV U1RXREG,W1
    MOV.B W1,[W0]

    INC _ib_idx		    ; Increment buffer index

    BTSC U1STA,#URXDA
    BRA 2b

1:
    MOV _scs_buff_watermark,W0
    MOV _ib_idx,W1

    CPSGT W0,W1
    MOV W1,_scs_buff_watermark

    BCLR IFS0,#U1RXIF

    POP W1
    POP W0

    retfie


    .global _ser_linker_fix
_ser_linker_fix:
    nop
    return

    .bss
    .global _scs_overflow_count
_scs_overflow_count:
    .space 2
    .align 2

    .bss
    .global _scs_buff_watermark
_scs_buff_watermark:
    .space 2
    .align 2