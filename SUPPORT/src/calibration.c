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

#ifdef SUPPORT_INC_CALIBRATION

#include "config.h"

#ifndef FCY
	#error "FCY not defined."
#endif

#define __CALIBRATION_H_INT
#include "calibration.h"

unsigned int cal_ns_per_inst;
unsigned int cal_inst_per_us;
unsigned int cal_inst_per_ms;

void calibrate(void)
{
	cal_ns_per_inst = 0;
	cal_inst_per_us = 0;
	cal_inst_per_ms = 0;

	cal_ns_per_inst = (1/(double)FCY) * 1000000000;

	cal_inst_per_us = 1000 / cal_ns_per_inst;
	cal_inst_per_ms = 1000000 / cal_ns_per_inst;

	return;
}
unsigned long cal_us_from_inst(unsigned long _inst)
{
	unsigned long long ns = 0;		// nano seconds
	ns = ((unsigned long)_inst * (unsigned long)(cal_ns_per_inst)) / 1000;
	return (unsigned long)ns;
}
unsigned long cal_inst_from_us(unsigned long _u_sec)
{
	unsigned int c = _u_sec * 1000;
	c = c/cal_ns_per_inst;
	return c;
}

#endif