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

#include "I2C/i2c.h"
#include <xc.h>

__attribute__((interrupt, shadow, no_auto_psv)) void I2C_MASTER_INTERRUPT(void)
{
	// Reset the ISR flag.
	MASTER_IF = 0;
	return;
}

void i2c_master_isr_fixup(void)
{
	Nop();
	return;
}