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

/**
 * \file
 * \author Vidas Simkus (vic.simkus@simkus.com)
 *
 * Declarations for setup functions.
 * XXX Need to document
 */

#ifndef SETUP_H
#define	SETUP_H

extern void setup_adc(void);
extern void setup_i2c_custom_registers(void);
extern void setup_i2c(void);
extern void setup_pmics(void);
extern void setup_status_led_pins();
extern void setup_do_pins(void);
extern void setup_T1();
extern void setup_T3();

#endif	/* INIT_H */

