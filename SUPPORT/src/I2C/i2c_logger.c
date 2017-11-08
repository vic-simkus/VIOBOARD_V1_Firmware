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

#define __LOGGER_H_INT
#include "I2C/logger.h"

#include "support.h"
#include "serial_comm.h"

static UCHAR logger_level;

void logger_init(UCHAR _level)
{
	logger_set_level(_level);

	return;
}

void logger_set_level(UCHAR _level)
{
	logger_level = _level;

	return;
}
inline void logger_log(const UCHAR * _data,UCHAR _level)
{
	UCHAR level;

	if(_level < logger_level)
	{
		return;
	}

	switch(_level)
	{
		case LOGGER_LEVEL_PROTOCOL:
			level = 'P';
			break;
		case LOGGER_LEVEL_ERROR:
			level = 'E';
			break;
		case LOGGER_LEVEL_WARN:
			level = 'W';
			break;
		case LOGGER_LEVEL_INFO:
			level = 'I';
			break;
		case LOGGER_LEVEL_DEBUG:
			level = 'D';
			break;
		default:
			level = '*';
			break;
	}

	ser_write_string((UCHAR *)"LOG|");
	ser_write_char(level);
	ser_write_char('|');
	ser_write_string(_data);
	ser_write_char('\n');

	return;
}
inline void logger_protocol(const UCHAR * _data)
{
	logger_log(_data,LOGGER_LEVEL_PROTOCOL);
	return;
}
inline void logger_info(const UCHAR * _data)
{
	logger_log(_data,LOGGER_LEVEL_INFO);
	return;
}
inline void logger_warn(const UCHAR * _data)
{
	logger_log(_data,LOGGER_LEVEL_WARN);
	return;
}
inline void logger_error(const UCHAR * _data)
{
	logger_log(_data,LOGGER_LEVEL_ERROR);
	return;
}
inline void logger_debug(const UCHAR * _data)
{
	logger_log(_data,LOGGER_LEVEL_DEBUG);
	return;
}
