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

/**
 * Main entry to the command processor.
 * The function is invoked whenever the termination and thus submission of the command buffer is detected by the main event loop.
 */
void process_command(void);
UCHAR process_binary_command();
void process_text_command(void);

void init_command_processor(void);

void reset_bin_context(void);
void reset_command_processor_state(void);

/**
 * Binary command callback function pointer
 */
typedef UCHAR (*binary_command_callback)(void) ;

#define BCC_OUTPUT_BUFFER_SIZE 38

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

	UINT output_buffer_idx;

	UCHAR output_buffer[BCC_OUTPUT_BUFFER_SIZE];

} binary_message_context;

#define RESP_MSG_ID 0
#define RESP_MSG_CALL_INDEX 1
#define RESP_MSG_RES_CODE 2
#define RESP_MSG_PLL_MSG 3
#define RESP_MSG_PLL_LSB 4
#define RESP_MSG_HEADER_LEN 5

extern binary_message_context bin_context;

#define BCC_BUFFER_ADD_BYTE(_b) \
		bin_context.output_buffer[bin_context.output_buffer_idx] = _b; \
		bin_context.output_buffer_idx += 1

#define BCC_BUFFER_ADD_WORD(_w) \
		bin_context.output_buffer[bin_context.output_buffer_idx] = (_w >> 8) & 0x00FF; \
		bin_context.output_buffer_idx += 1; \
		bin_context.output_buffer[bin_context.output_buffer_idx] = (_w & 0x00FF); \
		bin_context.output_buffer_idx += 1

/**
 * Number of implemented binary messages.  Used to size the callback pointer array.
 */
#define BINARY_COMMAND_COUNT 0x0D

#endif	/* COMMAND_PROCESSOR_H */

