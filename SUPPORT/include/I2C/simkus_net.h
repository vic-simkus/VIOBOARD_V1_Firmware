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

#ifndef __SIMKUS_NET_H
#define __SIMKUS_NET_H
#include "i2c.h"
#include "data_types.h"

/**
 * \file
 * 
 * Higher level interface library that sits on top of the I2C library.
 *
 * The major functionality that this library adds is that it does more error checking.  Rather than aborting on error it will retry a specified number of times to complete the operation.
 * The library is based on a very basic retry mechanism.  If an error is encountered it will retry a specified number of times with a delay between tries.
 * The number of retries and the delay between them is specified in the init function. All writes/reads by the library are done using this retry mechanism.
 * Some functions take a _max_wait parameter.  This is an additional level of retrying.  It is intended to provide extra leeway for functionality that may take extra time.  For example reseting a chip.
 * Function that take the _max_wait parameter will try the write/read operation the specified number of times.  So for a worst case scenario the function will fail after (retry wait uS * retry count * max wait).
 * Assuming that retry wait is 10 uS and retry count is 1000 and max wait is 100 the total time before admitting failure and giving up is 10 * 1000 * 100 or 1,000,000 uS or 1 second.
 *
 * \author Vidas Simkus (vic.simkus@simkus.com)
*/

/**
Default logger register
XXX - Why is this here
*/
#define SN_REG_LOGGER	7

#ifndef SN_DBS
/**
Default data buffer size
*/
#define SN_DBS	128
#endif

/**
Initializes the library.
\param _retry_count number of times to retry an operation
\param _retry_wait Number of uS to wait between retries.  A good strategy is a larger retry count with a lower retry wait.
*/
extern void sn_init(UINT _retry_count,UINT _retry_wait);

/**
Sends a reset command to the specified slave.
Returns immediately after the reset command is sent.
\param _addr Slave address
*/
extern void sn_reset_slave(UCHAR _addr);

/**
Sends a reset command to the specified slave and waits for the slave to come back up.
\param _addr Slave address
\param _max_wait Number of times to try to reach the slave.  See file documentation.
*/
extern UCHAR sn_reset_slave_ex(UCHAR _addr,UINT _max_wait );

/**
Sends a ping to the specified slave.
\param _addr Slave address
\return 1 if slave responded; 0 otherwise
*/
extern UCHAR sn_ping(UCHAR _addr);

/**
Sends a ping to the specified slave.  If no response re-tries a specified number of times.
\param _addr Slave address
\param _max_wait Number of times to try to reach the slave.  See file documentation
*/
extern UCHAR sn_ping_ex(UCHAR _addr,UINT _max_wait);

/**
Send a log entry to the specified slave.  The slave is expected to be of 'comm controller' type.
*/
extern void sn_ship_log(UCHAR _addr,UCHAR _level,UCHAR * _buffer);

/**
Writes a sequence of bytes to a slave.
\param _addr Slave address
\param _data Data to write
\param _length Number of bytes to write
\see i2c_write_buffer_ex
*/
extern const i2c_result * sn_write(UCHAR _addr,UCHAR * _data,UINT _length);

/**
Writes a sequence of bytes to the slave then reverses the bus and reads a sequence of bytes from the slave.
\param _addr Slave address
\param _data_out Data to write
\param _out_length Number of bytes to write
\param _data_in Buffer to store the read data
\param _in_length Number of bytes to read
*/
extern const i2c_result * sn_write_read(UCHAR _addr,UCHAR* _data_out,UINT _out_length,UCHAR * _data_in,UINT _in_length);
#endif
/*
EOF
*/
