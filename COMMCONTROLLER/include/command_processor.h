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
void process_binary_command();
void process_text_command(void);

void init_command_processor(void);

void reset_bin_context(void);
void reset_command_processor_state(void);

typedef UCHAR (*binary_command_callback)(void) ;

typedef struct {
	UCHAR is_active;
	UCHAR length_msb;
	UINT expected_length;
	UINT count;
	UINT start_index;
} binary_message_context;


extern binary_message_context bin_context;

#define BINARY_COMMAND_COUNT 0x10

#endif	/* COMMAND_PROCESSOR_H */

