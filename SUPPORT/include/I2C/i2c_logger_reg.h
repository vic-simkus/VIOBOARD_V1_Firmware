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

#ifndef __I2C_LOGGER_REG
#define __I2C_LOGGER_REG

#include "data_types.h"
/**
 * \file
 * I2C LOGGER register stuffs. The I2C logger allows shipping of logs via the I2C bus to someone who can do something with them.
 *
 * \author Vidas Simkus (vic.simkus@simkus.com)
 */

/**
 * Initializes the various behind the scenes stuffs.
 * Must be called after i2c_setup_default_registers, but before enabling the I2C stuffs.
 */
extern void i2c_logger_reg_init(void);

/**
 * Flushes the logger buffers out to the serial link.
 */
extern UCHAR  i2c_logger_flush(void);

#endif
/*
EOF
*/
