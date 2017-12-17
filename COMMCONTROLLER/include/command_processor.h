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

#ifndef COMMAND_PROCESSOR_H
#define	COMMAND_PROCESSOR_H

#define reset_command_processor_state()	\
	mg_sep_buffer_idx = 0; \
	mg_cmd_buffer_idx = 0

/**
 * Main entry to the command processor.
 * The function is invoked whenever the termination and thus submission of the command buffer is detected by the main event loop.
 */
void process_command(void);

/**
 * Processes a binary command.
 * @return
 */
UCHAR process_binary_command();

/**
 * Processes a text command.
 */
void process_text_command(void);

/**
 * Initializes the command processor.
 */
void init_command_processor(void);

/**
 * Dumps all of the board status to the serial port in binary message format.
 */
void process_binary_stream(void);

/**
 * Binary command callback function pointer
 */
typedef UCHAR (*binary_command_callback)(void) ;

/**
 * \note Keep this an even number.
 */
#define BCC_OUTPUT_BUFFER_SIZE 64

/**
 * Context for the binary message processing stage.
 */
typedef struct {
	/**
	 * Is the command processor in binary message processing mode.
	 * This is set to true (1) when the start of binary message ID byte (@) is received.
	 */
	UCHAR is_active;

	/**
	 * Most significant byte of the payload length.  We don't have an LSB field because when we get the LSB off the wire we roll it into the expected_length field.
	 */
	UCHAR length_msb;

	/**
	 * Expected payload length.
	 */
	UINT expected_length;

	/**
	 * Number of payload bytes received.  When this matches expected_length the binary message is processed.
	 */
	UINT count;

	/**
	 * Index of the binary message payload start within the command buffer.
	 */
	UINT start_index;

	/**
	 * Output buffer index
	 */
	UINT output_buffer_idx;

	/**
	 * 1 if the output stream mode is active.
	 */
	UINT is_stream_active;

	union {
		/**
		 * Output buffer in byte format.
		 */
		UCHAR output_buffer[BCC_OUTPUT_BUFFER_SIZE];

		/**
		 * Output buffer in word format.
		 */
		UINT output_buffer_w[BCC_OUTPUT_BUFFER_SIZE / 2];
	} ;

} binary_message_context;

/**
 * Binary message processor context.
 */
extern binary_message_context bin_context;

/**
 * Binary message response header size in bytes
 */
#define RESP_MSG_HEADER_LEN_B 6

/**
 * Binary message response header size in words
 */
#define RESP_MSG_HEADER_LEN_W 3

/**
 * Binary message response payload length index in the word format output buffer.
 */
#define REP_MSG_PAYLOAD_LEN_IDX_W	0x02

/**
 * Binary message response marker index in the byte format output buffer.
 */
#define RESP_MSG_MARKER_FIELD_IDX_B 0x00

/**
 * Binary message call index index in the byte format output buffer.
 */
#define RESP_MSG_CI_FIELD_IDX_B 0x01

/**
 * Binary message response code index in the word format output buffer.
 */
#define RESP_MSG_RESP_FIELD_IDX_W 0x01

/**
 * Sets the binary message marker in the response output buffer.
 */
#define BCC_RESP_SET_MARKER() \
	bin_context.output_buffer[0x00] = 0x10			// byte 1

/**
 * Sets the call index value in the response output buffer.
 */
#define BCC_RESP_SET_CI(_ci) \
	bin_context.output_buffer[0x01] = _ci			// byte 2

/**
 * Sets the response code value in the response output buffer.
 */
#define BCC_RESP_SET_RES(_res) \
	bin_context.output_buffer_w[0x01] = _res		// bytes 3 & 4

/**
 * Sets the payload length, in words, in the response output buffer.
 */
#define BCC_RESP_SET_PAYLOAD_LEN(_len) \
	bin_context.output_buffer_w[REP_MSG_PAYLOAD_LEN_IDX_W] = _len; \
	bin_context.output_buffer_idx = RESP_MSG_HEADER_LEN_B + (_len * 2) + 2

/**
 * Sets the payload word value at specified index.  Indexes start at 0.
 */
#define BCC_RESP_SET_WORD(_idx,_w)	\
	bin_context.output_buffer_w[(RESP_MSG_HEADER_LEN_W ) + _idx] = _w

/**
 * Clears the header in the response output buffer.
 */
#define BCC_RESP_CLEAR_HEADER() \
	bin_context.output_buffer_w[0x00] = 0x00; \
	bin_context.output_buffer_w[0x01] = 0x00; \
	bin_context.output_buffer_w[0x02] = 0x00; \
	bin_context.output_buffer_w[0x03] = 0x00; \
	bin_context.output_buffer_w[0x04] = 0x00

/**
 * Converts a byte format output buffer index into a word format output buffer index.
 */
#define BCC_MAKE_W_OFFSET(_v) \
	((_v & 0xFFFE) / 2)

/**
 * Successful execution result code constant.
 */
#define RESP_MSG_SUCCESS_CODE 0x00

/**
 * Unsuccessful execution result code constant.
 */
#define RESP_MSG_FAIL_CODE 0x01

/**
 * Number of implemented binary messages.  Used to size the callback pointer array.
 */
#define BINARY_COMMAND_COUNT 0x0F

/**
 * Modulus addressing of the command buffer.
 */
#define MG_BUFFER_OFFSET(_offset) ((_offset) % COMMAND_BUFFER_SIZE)

#endif	/* COMMAND_PROCESSOR_H */

