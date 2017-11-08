#ifndef __SPI_H
#define __SPI_H

/**
\file
Module for handling basic SPI communication.  
The module is a thin wrapper over the built-in SPI functionality and supports only master mode.
The module does not use ISRs, rather it dumps the outgoing data directly onto the bus.
The "enhanced" mode is not used because when I attempted to use it all I got out of the FIFOs where zeros.
Either way, for such a basic, synchronous communications mechanism I don't think there's a need for FIFOs and ISRs.
*/

#include "data_types.h"

#ifndef SPI_SS
	/**
	Defines the latch to use for the SS pin.  The default is for a PIC24FV32KA302
	*/
	#define SPI_SS LATBbits.LATB15
#endif

#ifndef SPI_SS_TRIS
	/**
	Defines the TRIS to use in configuring the SS pin.
	\see SPI_SS
	*/
	#define SPI_SS_TRIS TRISBbits.TRISB15
#endif

/**
Initializes the SPI module.
*/
extern void spi_init(void);

/**
Sends and receives data via SPI.
Because of the way that SPI works, _data_out and _data_in can be the same chunk of memory.
The function does no bounds checking.  It's up to the caller to ensure that the memory is properly allocated and such.
\param _data_out Buffer containing the data to be written out to the slave.
\param _data_int Buffer where the data received from the slave will be written to.
\param _count Number of bytes to write/read from the slave.
*/
extern void spi_communicate(UCHAR * _data_out,UCHAR * _data_in,UINT _count);

#endif
