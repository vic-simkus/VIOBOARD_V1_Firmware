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

#ifndef CP_IC_H
#define	CP_IC_H

#include "support.h"

void print_help(void);

UCHAR cmd_command(void);
UCHAR cmd_command_do_verbose_version();
UCHAR cmd_command_do_echo(UCHAR* _data, UCHAR _length);
UCHAR cmd_command_do_version();

UCHAR cmd_print_status(void);
UCHAR cmd_print_status_bus(void);
UCHAR cmd_print_status_ai(void);
UCHAR cmd_print_status_do(void);
UCHAR cmd_print_status_pmic(void);

void cmd_ping(void);
void cmd_reset(void);

UCHAR cmd_set(void);
UCHAR cmd_set_do(void);
UCHAR cmd_set_pmic(void);


#endif	/* CP_IC_H */

