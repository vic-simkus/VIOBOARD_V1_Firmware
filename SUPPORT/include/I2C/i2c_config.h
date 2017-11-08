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
 * Various defines to configure the I2C stuffs.
 */
#ifndef I2C_CONFIG_H
#define	I2C_CONFIG_H

/**
 * The size of the I2C logger register input buffer.  Any data that comes in after
 * the buffer is full will be silently discarded.
 */
#define I2C_LOGGER_IBS 80

/**
 * Echo register buffer size.
 */
#define I2C_ECHO_REG_SIZE 16


#endif	/* I2C_CONFIG_H */

