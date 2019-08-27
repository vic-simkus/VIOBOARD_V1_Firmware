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

#include "I2C/i2c.h"
#include <xc.h>

/******************************************************************************
Slave ISR
 *******************************************************************************/


/******************************************************************************
Start slave ISR variables
 ******************************************************************************/

/*
The main struct holding the slave-mode accessible registers
 */
extern volatile i2c_register_list i2c_registers;

/*
Currently selected register
 */
static volatile i2c_register* current_register_ptr = 0;
static volatile UINT current_register_idx = 0;

/*
Current index/offset of what ever is happening.
A counter of how many bytes have been received/sent since register selection
 */
static volatile UINT byte_index = 0;

/******************************************************************************
End slave ISR variables
 ******************************************************************************/

/*
	Since each device can act as either slave or master the slave ISR stays
	in in both builds
 */
__attribute__( ( interrupt, shadow, no_auto_psv ) ) void I2C_SLAVE_INTERRUPT( void )
{
    turn_on_receive_led( );

    UCHAR error_condition = 0;

    /*
     * Need to read the address out of the register in order to prevent an overflow.
     */
    UCHAR work_byte = I2CRCV;
    I2CSTATbits.I2COV = 0;

    if( I2CSTATbits.D_NOT_A == I2C_DA_A ) //data/address.  1 == last byte was data; 0 == last byte was address
    {
	/*
	 * The ISR was invoked in response to an ADDRESS reception.  The address it self is not visible to the ISR.
	 * We get here on two conditions:
	 *	- Initial bus start and it's associated address byte
	 *	- Bus direction reversal
	 */

	if( I2CSTATbits.R_NOT_W == I2C_RW_W ) //read/write 1 == read - data is output from slave.  0 == write - data is input to slave
	{
	    /*
	     * The WRITE bit was set
	     */
	    current_register_ptr = &i2c_registers.register_list[0];
	    current_register_idx = 0;
	    byte_index = 0;

	    /*
		    No further action is required if we're in write mode.  Next byte will be register address
		    In read mode a byte would have to be output in response which will be done below.
	     */

	    goto __end_slave_isr;
	}

	/*
	 * So in write mode, the ISR is done after the first byte (address byte).  In read mode the slave is expected to start producing
	 * data after the receipt of the address and r/w byte.
	 *
	 * In write mode the conditional dumps us to the end of the ISR.  In read mode we end up here.
	 */

	/*
	 * According to the datasheet, SCLREL will automatically clear on receipt of slave read request.
	 * We're going to do it manually just in case because we don't really trust the silicone.
	 */

	//I2CCONbits.SCLREL = 0;
    } // current byte is the address

    if( I2CSTATbits.R_NOT_W == I2C_RW_W )
    {
	if( current_register_idx == 0 )
	{
	    /*
	     * For our purposes, based on our protocol design, reading from a slave without first selecting a register is technically an error condition.
	     * The expected protocol flow, is as follows:
	     *
	     * In order to write to slave:
	     * - Write to slave address byte with write bit
	     * - Write byte to slave which is the destination register for future writes
	     * - Write data to slave
	     *
	     * In order to read from slave:
	     * - Write to slave the address byte with write bit
	     * - Write byte to slave which is the source register for future reads
	     * - Turn around the bus with a an address byte with read bit
	     * - Read data from slave
	     */

	    /*
	     * We're in write mode, but register has not been selected yet.
	     * That means that the first byte of the message is the register address
	     */

	    if( work_byte >= i2c_registers.count )
	    {
		/*
		Register out of range.  Do nothing
		Can we handle these error conditions better somehow?  Say via an LED or something?
		 */
		error_condition = 1;
		goto __end_slave_isr;
	    }
	    else
	    {
		current_register_ptr = &i2c_registers.register_list[work_byte];
		current_register_idx = work_byte;
	    }
	} //	current_register_idx == 0
	else
	{
	    /*
	     * We're in write mode and a register has been selected by the previous byte in the message.
	     */

	    if( current_register_ptr->ro == 1 )
	    {
		/*
		Trying to write to a read only register
		Need to figure out how to handle errors better.
		See comment for input_byte >= register_count
		 */
		error_condition = 3;
		goto __end_slave_isr;
	    }

	    if( current_register_ptr->data != 0 )
	    {
		/*
		Data pointer has priority over callback
		 */
		if( byte_index < current_register_ptr->data_length )
		{
		    current_register_ptr->data[byte_index] = work_byte;
		    byte_index = byte_index + 1;
		}
		else
		{
		    /*
		     * If the data write exceeds the data available, do nothing.
		     */
		    goto __end_slave_isr;
		}
	    }
	    else if( current_register_ptr->cb_write != 0 )
	    {
		current_register_ptr->cb_write( work_byte, byte_index );
		byte_index = byte_index + 1;
	    }
	    else
	    {
		/*
		Error condition.  Neither a valid data pointer nor a valid callback function pointer are available
		 */
		error_condition = 2;
		goto __end_slave_isr;
	    }
	} //	current_register_idx != 0
    }
    else
    {
	/*
	 * We're in read mode
	 * Current register has been selected by the previous byte write.  Or if it
	 * has not, we're operating on register 0 which is a null register.
	 */

	turn_on_send_led( );

	if( current_register_ptr->data != 0 )
	{
	    /*
	    Data pointer has priority over callback
	     */
	    if( byte_index >= current_register_ptr->data_length )
	    {
		/*
		If the data read exceeds the data available, do nothing.
		 */

		work_byte = 0;
	    }
	    else
	    {
		work_byte = current_register_ptr->data[byte_index];
		byte_index = byte_index + 1;
	    }
	}
	else if( current_register_ptr->cb_read != 0 )
	{
	    work_byte = current_register_ptr->cb_read( byte_index );
	    byte_index = byte_index + 1;
	}
	else
	{
	    /*
	    Error condition.  Neither a valid data pointer nor a valid callback function pointer are available
	     */
	    error_condition = 2;
	    goto __end_slave_isr;
	}

	WAIT_FOR_TBF( ); // Wait for transmit buffer to become empty.
	I2CTRN = work_byte;
    } // slave read mode


__end_slave_isr:

    if( error_condition > 0 )
    {
#ifdef I2C_FO

	if( error_condition == 1 )
	{
	    /*
	     * Register is out of range.
	     */

	    BUS_IN_LED = 1;
	    BUS_OUT_LED = 1;
	}
	else if( error_condition == 2 )
	{
	    /*
	     * Misconfigured register.  Data and callback are both null.
	     */

	    BUS_IN_LED = 0;
	    BUS_OUT_LED = 1;
	}
	else if( error_condition == 3 )
	{
	    /*
	     * Tried to write to a RO register.
	     */

	    BUS_IN_LED = 1;
	    BUS_OUT_LED = 0;
	}
#endif
    }
    else
    {
	turn_off_receive_led( );
	turn_off_send_led( );
    }

    SLAVE_IF = 0; // Clear the slave ISR flag
    I2CCONbits.SCLREL = 1; //	Release the clock.  Let stuff continue to happen.

    return;
}

void i2c_slave_isr_fixup( void )
{
    Nop( );
    return;
}

/*
 * EOF
 */