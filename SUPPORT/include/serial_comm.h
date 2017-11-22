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

#ifndef __SERIAL_COMM_H
#define __SERIAL_COMM_H

/**
	\file
	Module for handling basic UART communications. The module relies on hardware (CTS/RTS) flow control.
	Reception of characters is buffered to internal buffers via interrupts, while sending is done directly.
	If internal buffers are filled no further reception of data will be done.
	Output is buffered.  Flushing of output buffers is done automatically, but mechanism is provided to manually flush the buffers.
	The module replaces the libc function 'write' and redirects all output to the UART in a buffered manner.

	The timeout mechanism in the library is very crude.  It spins waiting for U1STAbits.TRMT to be set while incrementing a counter.
	Once a counter reaches the value of timeout_clicks (set in ser_init) the write process is aborted.

	\see SER_IBS
	\see SER_OBS

 * \author Vidas Simkus (vic.simkus@simkus.com)
 */

#include "data_types.h"

/**
	Writes a character to the UART.  Output is buffered.
	\return 1 if data was written to UART.  0 if an error was encountered; last write error will be set.
 */
inline extern UINT ser_write_char(const UCHAR _c);

/**
 * Writes a 16 bit integers to the UART.  Output is buffered.
 * \param _i Integers to be written out.
 * \return 1 if data was written to UART.  0 if an error was encountered; last write error will be set.
 */
inline extern UINT ser_write_16bit(const UINT _i);

/**
	Writes a string to the UART.  The writes are buffered until end of string or as the buffer is filled up.
	\return 1 if data was written.  0 if error was encountered; last write error will be set.
	\note An alternative to this function is 'printf'.  This function is simpler and therefore faster.
 */
extern UINT ser_write_string(const UCHAR * _data);

/**
	Writes a string the UART and appends an NL character at the end.  Same behaviour as ser_write_string
	\see ser_write_string
 */
extern UINT ser_write_string_nl(const UCHAR * _data);

/**
	Initializes the serial module.  This *must* be called before any other module functions.  No data will be sent or received without calling this first.

	Common values for the _brg parameter using the high speed baud generator (_brgh = 1) for a chip with FCY of 16Mhz

	34 - 115K
	70 - 56K
	416 - 9600
	13332 - 300

	\param _brg Baud rate generator period
	\param _brgh 1 selected high speed baud rate.  0 selects low speed baud rate.  This affects the BRG period
	\param _isr_priority Priority of the RX ISR
	\param _timeout_clicks Number of instruction cycles to wait for something.  This is a very rough estimate as the loop overhead is not accounted for.
	\note Consult the PIC famility reference manual for correct BRGH and BRGH combinations
 */
extern void ser_init(UCHAR _brg, UCHAR _brgh, UCHAR _isr_priority, UINT _timeout_clicks);

/**
	Return 1 if new data is available.  Otherwise 0 is returned.
 */
inline extern UCHAR ser_new_data(void);

/**
	Flushes the output buffer to the UART.
	There is no guarantee how fast this function will return.
	This function will reset the last write error.
	\return Returns the number of bytes written.  If number of bytes written is less than what was expected a timeout occured.
 */
extern UINT ser_flush_buffer(void);

/**
	Gets the pointer to current input buffer and switches the active input buffer.
	\param _dest Address of the destination pointer.  This will be populated with current input buffer pointer.
	\return Number of bytes in the input buffer.  If there's not data 0 will be returned.
	\note Do not modify in any way the input buffer pointer.  I.e do not free it.  Once the pointer is received copy the data out of the buffer and forget about the pointer.  Any modification to the pointer *WILL* break stuff.
 */
extern UINT ser_get_data(UCHAR ** _dest);

/**
	Writes data to the UART.  The writes are buffered until end of data or as the buffer is filled up.
	\return 1 if data was written successfully. 0 otherwise; last write error will be set.
 */
extern UINT ser_write_data(const UCHAR * _data, UINT _size);

/**
 * \see serial_comm_asm.S
 */
extern volatile UINT scs_overflow_count;

/**
 * Statistics we keep around for the communication portions of the board.
 * \XXX This is misnamed.  Nothing directly to do with the serial stuff.
 */
typedef struct {
	/**
	 * Number of times an invalid call index was encountered.
	 */
	UINT bin_cmd_no_index;
	/**
	 * Number of times a call index was out of range.
	 */
	UINT bin_cmd_range;
	/**
	 * Number of times that a binary callback failed.
	 */
	UINT bin_cmd_cb_fail;
	/**
	 * Number of times the command processor buffer overflowed.
	 */
	UINT cmd_proc_overflow;
} ser_prot_errors;

/**
 * Communication system statistics instance.
 */
extern ser_prot_errors serial_protocol_errors;

#endif