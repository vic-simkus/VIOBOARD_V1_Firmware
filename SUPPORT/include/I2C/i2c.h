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

#ifndef __I2C_H
#define __I2C_H

/**
 * \file
 * \author Vidas Simkus (vic.simkus@simkus.com)
 *
 * \brief Provides access to the on-chip I2C module.
 *
 * This library has been tested on dsPIC30F4011 and PIC24FV32KA302 uCs.
 *
 * The module consists functions that are designated mode 0 to mode 2. Is this level stuff really beneficial?  I have no idea.  There's no real clear delineation between the levels.
 * 'Level 0' functions are the lowest level functions that interface directly with the hardware (for what little interface there is actually to be done). Rarely should level 0 functions be used in 'user' code.
 * 'Level 1' functions are higher level functions that rely on level 0 functions. These also should be used with caution in user code.
 * 'Level 2' functions are the highest level functions intended to be used with wild abandon in user code.  All timing, bus states, etc are taken care of.
 */

#include "data_types.h"
#include "i2c_config.h"

#define I2C_ADDR_COMM_CTRL	0x10
#define I2C_ADDR_IO_CTRL	0x30

typedef struct
{
	UINT write_errors;
	UINT read_errors;
	UINT collisions;
} I2C_COMM_STATS;

extern volatile I2C_COMM_STATS i2c_comm_stats;

#ifdef __PIC24FV32KA302__

/*
This is a PIC24 device...
Change definition to match those of the dsPIC30F4011 to make porting easier
 */

#define I2CCON		I2C1CON
#define I2CSTAT		I2C1STAT
#define I2CCONbits	I2C1CONbits
#define I2CSTATbits	I2C1STATbits

#define I2CRCV		I2C1RCV
#define I2CTRN		I2C1TRN
#define I2CADD		I2C1ADD

#define SLAVE_IPC	_SI2C1P
#define	MASTER_IPC	_MI2C1P

#define	SLAVE_IEC	_SI2C1IE
#define MASTER_IEC	_MI2C1IE

#define	SLAVE_IF	_SI2C1IF
#define MASTER_IF	_MI2C1IF

#define I2CBRG 		I2C1BRG

#define I2CADD		I2C1ADD
#define I2CMSK		I2C1MSK

#define SLAVE_INTERRUPT		_SI2C1Interrupt
#define MASTER_INTERRUPT	_MI2C1Interrupt

#else
/*
This is a dsPIC30 device
 */

#define SLAVE_IPC	_SI2CIP
#define	MASTER_IPC	_MI2CIP

#define	SLAVE_IEC	_SI2CIE
#define MASTER_IEC	_MI2CIE

#define	SLAVE_IF	_SI2CIF
#define MASTER_IF	_MI2CIF

#define	I2CBRG 		I2CBRG

#define SLAVE_INTERRUPT			_SI2CInterrupt
#define MASTER_INTERRUPT		_MI2CInterrupt
#endif

#ifdef I2C_FO

/*
Functions below activate the various blinking lights and stuff.  Blinking lights and LCD displays are neat.
This is mostly for my development platform where I have all sorts of blinking lights and LCD display hooked up.
If you wish to utilize these routines:

define BUS_OUT_LED to something like LATBbits.LATB0 - or what ever
define BUS_IN_LED same general concept as BUS_OUT_LED
define I2C_FO (I2C fancy output)
 */

#define turn_on_send_led() BUS_OUT_LED = 1
#define turn_off_send_led() BUS_OUT_LED = 0

#define turn_on_receive_led() BUS_IN_LED = 1
#define turn_off_receive_led() BUS_IN_LED = 0

#else

#define turn_on_send_led() Nop()
#define turn_off_send_led() Nop()

#define turn_on_receive_led() Nop()
#define turn_off_receive_led() Nop()


#endif


/**
\defgroup BUS_RW Bus direction
These constants define the direction that the bus will be operating in within the context of the two operating nodes.
 */

/**
\ingroup BUS_RW
I2C Master will be writing to slave
 */
#define I2C_RW_W	0

/**
\ingroup BUS_RW
I2C Master will be reading from slave
 */
#define I2C_RW_R	1

#ifndef I2C_SLAVE_REGISTER_COUNT
#error "Must define I2C_SLAVE_REGISTER_COUNT"
#endif

#define I2C_BUS_RETRIES 10


/**
Various errors that the library may produce.  The last_read_error and last_write_error will be set to these values
 */
enum {
	I2C_ERR_NONE = 0,		///	0 -- No error
	I2C_ERR_R_OVR,			/// 1 -- Receive error; read overflow.
	I2C_ERR_R_TIMEOUT,		///	2 -- Read timeout
	I2C_ERR_W_NACK_CTRL,	///	3 -- Send error; slave did not acknowledge control byte
	I2C_ERR_W_NACK_DATA,	///	4 -- Send error, slave did not acknowledge data byte
	I2C_ERR_W_TIMEOUT,		/// 5 -- Write timeout
	I2C_ERR_COLLISION		/// 6 -- Generic collision.  May be either a write collision or a master collision.  Reason for a generic collision is because it doesn't ultimately matter what kind of collision it is.  Everything must be resent from the beginning.
} ;

#ifdef I2C_ERROR_STRINGS
/**
 * Provides human-readable error messages
 */
extern const UCHAR * const  I2C_ERROR_MAP[];
#endif

/**
These are the default registers that are implemented by the i2c_setup_default_registers function.
\see i2c_setup_default_registers
 */
enum {
	/**
	 * A null register.  Reading from it produces all zeros and writing to it sinks the input
	 */
	I2C_REG_NULL = 0,

	/**
	 * A write of 0xAA to this register will reset the chip
	 */
	I2C_REG_RESET = 1 ,

	/**
	 * A read from this register will result in byte 'R' (reply)
	 */
	I2C_REG_PING = 2,

	/**
	 * A read from this register will result in string 'PONG'.
	 */
	I2C_REG_PING_EX = 3,

	/**
	 * A read from this register will result in what ever was written to this register beforehand.  Limit of 8 bytes, please.
	 */
	I2C_REG_ECHO = 4,

	/**
	 * A read from this register will result in a user defined byte that defines the version of what ever this library is running on.  Defined in I2C_MODULE_VERSION
	 */
	I2C_REG_VERSION = 5,

	/**
	 * A read from this register will result in a string that identifies in a user friendly way what ever this library is and where it's running on.  Defines in I2C_MODULE_VERSION_STR
	 */
	I2C_REG_VERSION_STR = 6,

	/**
	 * Number of defined registers.  Keeps manual math to a minimum
	 */
	I2C_REG_COUNT__	= 7
} ;

/**
 * Function pointer to a register write callback.
 * \param _idx Current ISR index.
 */
typedef void (*i2c_cb_register_w)(UCHAR _byte, UINT _idx);

/**
Function pointer to a register-read callback function.
\param _idx Current ISR index.
\return Read byte
 */
typedef UCHAR(*i2c_cb_register_r)(UINT _idx);

/**
Result of the high-level read/write/read-write functions
 */
typedef struct __i2c_result {
	/**
	Number of bytes read.  Should be compared against the expected number of bytes to be read to check for error in transmission
	 */
	UINT bytes_read;

	/**
	Number of bytes written.  Should be compared against the expected number of bytes to be written to check for error in transmission
	 */
	UINT bytes_written;

	/**
	Last write error
	 */
	UINT write_error;

	/**
	Last read error
	 */
	UINT read_error;
} i2c_result;

/**
Definition of a register.
Every implemented register is defined by this struct.
 */
typedef struct {
	/**
	Read-only flag.
	If 1 then the register is read-only
	 */
	UCHAR ro;

	/**
	Data buffer for the register.
	 */
	UCHAR * data;

	/**
	Data buffer length.
	 */
	UINT data_length;

	/**
	Register write call-back.  The data buffer has priority.  Meaning that if data buffer is not zero the call-back will never be invoked.
	 */
	i2c_cb_register_w cb_write;

	/**
	Register read call-back.  The data buffer has priority.  Meaning that if data buffer is not zero the call-back will never be invoked.
	 */
	i2c_cb_register_r cb_read;
} i2c_register;

/**
 * All implemented registers that will be used by the library.
 */
typedef struct {
	/**
	Number of implemented registers
	 */
	UCHAR count;

	/**
	Array of pointers to instances of implemented registers.
	 */
	i2c_register register_list[I2C_SLAVE_REGISTER_COUNT];
} i2c_register_list;


/**
All *implemented* registers.
 */
extern volatile i2c_register_list i2c_registers;

/**
 * Initializes the I2C module including enabling the slave ISR.  All controllers using this library can be either slave or master.  In other words we create a multi master bus topology.
 * 	\note Make sure that the register structs are setup before calling this method.  This method will enable the I2C module and after that incoming request will begin to be serviced immediately.
 * 	\param _isr_priority Priority of the I2C slave ISR
 * 	\param _slave_address Unshifted, unmolested byte with upper seven bits as the slave address.  There are certain slave addresses that are considered "invalid".  Check the chip documentation for details.
 * 	\param _brg Baud rate generator value.  Consult chip document
 * 	\note Call 	i2c_setup_default_registers or equivalent BEFORE calling this function.
 */
extern void i2c_init(UCHAR _isr_priority, UCHAR _slave_address, UINT _brg);

/**
 * Returns the last encountered read error and clears the last read error.
 * \return Last read error
 */
inline extern SINT i2c_last_read_error(void);

/**
 * Returns the last encountered write error and clears the last write error.
 * \return Last write error
 */
inline extern SINT i2c_last_write_error(void);

/**
 * Starts the I2C bus.  Sets the I2CCON.SEN to 1 and waits for the bit to clear.
 * This is the fist function a master will call before proceeding with the control byte.
 * This function should only be called by the master.
 * This is a 'level 0' function and should not be used directly in user code.
 * \return Returns a 1 if operation was successful, 0 if a master bus collision (I2CSTAT.BCL) was detected.
 */
inline extern UCHAR i2c_bus_start(void);

/**
 * Restarts the I2C bus.  Sets the I2CCON.RSEN to 1 and waits for the bit to clear.
 * This function will be used by the master to reverse the bus direction after requesting data from the slave.
 * This function should only be called by the master.
 * \return Returns a 1 if operation was successful, 0 if a master bus collision (I2CSTAT.BCL) was detected.
 * This is a 'level 0' function and should not be used directly in user code.
 */
inline extern UCHAR i2c_bus_restart(void);

/**
 * Stops the I2C bus.  Sets the I2CCON.PEN to 1 and waits for the bit to clear.
 * This function will be used by the master to stop the bus after it is done sending or receiving all bytes.
 * This function should only be called by the master.
 * \return Returns a 1 if operation was successful, 0 if a master bus collision (I2CSTAT.BCL) was detected.
 * This is a 'level 0' function and should not be used directly in user code.
 */
inline extern UCHAR i2c_bus_stop(void);

/**
Writes a byte to the bus. This function will be called by the master as all slave writes are done in the slave ISR.
This is a 'level 0' function.
Waits for a clear bus before returning if no errors are encountered.
\return 1 = operation successful (1 byte written), 0 = a collision occurred.
\param _data_out byte to write
 */
//extern UCHAR i2c_write_byte(UCHAR _data_out);

/**
Writes the control byte (address + rw) on the bus. This function will be called by the master as all slave writes are done in the slave ISR.
Waits for clear bus at invocation and before returning if no errors encountered.
This is a 'level 1' function
\return 1 on successful completion of a write, 0 otherwise and last_write_error is set to the appropriate error code.
\param _addr 7 bit address in unmolested form.  The r/w bit will be accounted for by the function
\param _restart 1 if a 'restart' rather then a 'start' should be emitted
 */
inline extern UCHAR i2c_write_control_byte(UCHAR _addr, UCHAR _restart);

/**
Writes a buffer to the bus. This function will be called by the master as all slave writes are done in the slave ISR.
Waits for clear bus before returning if no errors are encountered.
The function will abort sending if a collision is detected or if the slave does not reply with an ACK.
This is a 'level 1' function.
\return Number of bytes written.
\param _data data to be written
\param _count number of bytes to write.
 */
extern UINT i2c_write_buffer(UCHAR * _data, UINT _count);

/**
 * writes a buffer to the bus. The difference between this and the write_buffer function is that this function will send the control byte as well as the data.
 * This function will be called by the master as all slave writes are done in the slave ISR.
 * This is a 'level 2' functions.
 * \param _address Slave address to write to.  Upper seven bits count.  Do not shift or otherwise molest.  The r/w bit is automatically accounted for.
 * \param _data Data to write
 * \param _count Number of bytes to write.
 * \return Number of bytes written.
 * \note This function will call i2c_bust_start and i2c_bus_stop.
 */
extern UCHAR i2c_write_buffer_ex(UCHAR _address, UCHAR * _data, UINT _count, i2c_result * _res);

/**
Writes a stream of bytes to a slave, turns the bus around and then reads a stream of bytes from the slave. This function will be called by the master as all slave writes are done in the slave ISR.
\param _address Unmollested slave address. The r/w bit is automatically accounted for.
\param _write_data Buffer to data to be written
\param _write_count Number of bytes to write before turning the bus around and reading from slave
\param _read_buffer Destination buffer for read data
\param _read_count Number of bytes to read from the slave
\param _res Result struct which, if not NULL, will be populated with details of the operation.
\return 1 on complete success, 0 otherwise
\note Because of the sequential nature of the operation the write and read buffers can be the same buffer assuming that overwriting of data is not an issue.
 */
extern UCHAR i2c_write_read(UCHAR _address, UCHAR * _write_data, UINT _write_count, UCHAR * _read_buffer, UINT _read_count, i2c_result* _res);

/**
Sets up the default registers for the library.
 */
extern void i2c_setup_default_registers(void);

/**
Pauses the I2C module.  All setting are saved.
 */
extern void i2c_pause_module(void);

/**
Resumes the I2C module.  All settings are restored.
 */
extern void i2c_resume_module(void);

/***************************************************************************
 *
 * Various defines start here.
 *
 ***************************************************************************/

/*
 * 0x4000 = 0b0100 0000 0000 0000 (TRSTAT)
 * 0x1F   = 0b0000 0000 0001 1111 (SEN,RSEN,PEN,RCEN,ACKEN)
 * SEN - Start Condition Enabled (master)
 * RSEN - Repeated Start Condition Enabled (master)
 * PEN - Stop Condition Enabled (master)
 * RCEN - Reive Enable (master)
 * ACKEN - Acknowledge Sequence Enable (master)
 */

#define I2C_WAIT_I2CCON_L5() while(I2CCON & 0x1F){Nop();}

/**
 * S and P are bits 3 and 4 in the I2CSTAT register
 * 0x18 = 0b0000 0000 0001 1000
 */
#define I2C_WAIT_IDLE_BUS() while(I2CSTATbits.P == 0 && I2CSTATbits.S == 1) {Nop();}


/// Wait for the read buffer to fill.  1 == receive complete, 0 == receive not complete.
#define WAIT_FOR_RBF() 	while(I2CSTATbits.RBF == 0){Nop();}


/// Wait for the transmit  buffer to become empty. 1 == transmit in progress, 0 == transmit complete
#define WAIT_FOR_TBF() 	while(I2CSTATbits.TBF == 1){Nop();}

#define WAIT_FOR_TRSTAT() while(I2CSTATbits.TRSTAT == 1){Nop();}

/// Initializes the i2c_result struct
#define INIT_RES_STRUCT(_res) i2c_result _res;  _res.bytes_written = 0; _res.bytes_read = 0;_res.read_error = I2C_ERR_NONE;_res.write_error = I2C_ERR_NONE


/**
 * Apparently the symbol for ISR handler is not sucked in by the linker if nothing else is referenced in the object file.  So here is a symbol we will explicitly reference in the init code so that the ISR routine gets linked in.
 */
extern void i2c_slave_isr_fixup(void);

/**
 * \see i2c_slave_isr_fixup
 */
extern void i2c_master_isr_fixup(void);

#endif
/*
EOF
 */
