/*
Vic's IO Board V1; Firmware

Copyright (C) 2019 Vidas Simkus (vic.simkus@simkus.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


/**
 * \file
 * \author Vidas Simkus (vic.simkus@simkus.com)
 *
 * Custom I2C register callback implementation declarations.
 */

#ifndef I2C_CALLBACKS_H
#define	I2C_CALLBACKS_H

#include "support.h"

UCHAR i2c_cb_get_l1_cal(UINT _idx);
UCHAR i2c_cb_get_l2_cal(UINT _icx);

void i2c_cb_set_l1_cal(UCHAR _byte, UINT _idx);
void i2c_cb_set_l2_cal(UCHAR _byte, UINT _icx);

UCHAR i2c_cb_get_boot_count(UINT _idx);

UCHAR i2c_cb_get_ai_status(UINT _idx);
UCHAR i2c_cb_get_do_status(UINT _idx);

void i2c_cb_set_do_status(UCHAR _byte, UINT _idx);
void i2c_cb_set_pmic_status(UCHAR _byte, UINT _idx);
UCHAR i2c_cb_get_pmic_status(UINT _idx);

void i2c_cb_set_output_confirm(UCHAR _byte, UINT _idx);



#endif	/* I2C_CALLBACKS_H */

