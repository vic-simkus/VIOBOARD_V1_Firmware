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


#ifndef __LOGGER_H
#define __LOGGER_H

#include "data_types.h"

/**
 * \file
 *
 * \author Vidas Simkus (vic.simkus@simkus.com)
 */

/**
 * \defgroup LOG_LEVEL Logging level
 * Defines for the various logging levels
 */

/**
 * \ingroup LOG_LEVEL
 * Logging level that identifies the message as a 'PROTOCOL' message.
 */
#define LOGGER_LEVEL_PROTOCOL	0x39

/**
 * \ingroup LOG_LEVEL
 * Error logging level.
 */
#define LOGGER_LEVEL_ERROR		0x38

/**
 * \ingroup LOG_LEVEL
 * Warning logging level.
 */
#define LOGGER_LEVEL_WARN		0x37

/**
 * \ingroup LOG_LEVEL
 * Information logging level.
 */
#define LOGGER_LEVEL_INFO		0x36

/**
 * \ingroup LOG_LEVEL
 * Debug logging level
 */
#define LOGGER_LEVEL_DEBUG		0x30

/**
 * Sets the logging level.  Any message that comes in that's less than the currently configured logging level will be discarded.
 * \param _level Logging level.
 */
void logger_set_level(UCHAR _level);

/**
 * Initializes the logger with the supplied logging level.
 * \param _level Logging level.
 */
void logger_init(UCHAR _level);

/**
 * Puts a message into the logger at the specified logging level.
 * \param _data Null terminated text log message.
 * \param _level Message log level.
 */
void logger_log(const UCHAR * _data,UCHAR _level);

/**
 * Wrapper for the logger_log function.
 * \see logger_log
 * \param _data Null terminated text log message.
 */
void logger_protocol(const UCHAR * _data);

/**
 * Wrapper for the logger_log function.
 * \see logger_log
 * \param _data Null terminated text log message.
 */
void logger_info(const UCHAR * _data);

/**
 * Wrapper for the logger_log function.
 * \see logger_log
 * \param _data Null terminated text log message.
 */
void logger_warn(const UCHAR * _data);

/**
 * Wrapper for the logger_log function.
 * \see logger_log
 * \param _data Null terminated text log message.
 */
void logger_error(const UCHAR * _data);

/**
 * Wrapper for the logger_log function.
 * \see logger_log
 * \param _data Null terminated text log message.
 */
void logger_debug(const UCHAR * _data);

#endif
