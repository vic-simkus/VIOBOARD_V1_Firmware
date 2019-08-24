#include <xc.h>
#include "I2C.h"
#include "config.h"

#define I2C_M_WA_0	1	// Receipt of address byte 1
#define I2C_M_WA_1	2	// Receipt of address byte 2
#define I2C_M_W		3	// We're in write mode
#define I2C_M_R		4	// We're in read mode

volatile FAKE_EEPROM eeprom_data;
static volatile uint16_t current_counter = 0;
static volatile unsigned char current_mode = I2C_M_R;

__attribute__( ( interrupt, shadow, no_auto_psv ) ) void _SI2C1Interrupt( void )
{
    /*
     * Need to read the address out of the register in order to prevent an overflow.
     */
    unsigned char work_byte = I2CRCV;
    I2CSTATbits.I2COV = 0;

    if( I2CSTATbits.D_NOT_A == I2C_DA_A ) //data/address.  1 == last byte was data; 0 == last byte was address
    {
	PIN_HMI1 ^= 1;

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

	    /*
		    No further action is required if we're in write mode.  Next byte will be register address
		    In read mode a byte would have to be output in response which will be done below.
	     */

	    PIN_HMI2 ^= 1;

	    /*
	     In next write operation we're expecting the first address byte.
	     */
	    current_mode = I2C_M_WA_0;


	    goto __end_slave_isr;
	}

	/*
	 * So in write mode, the ISR is done after the first byte (address byte).  In read mode the slave is expected to start producing
	 * data after the receipt of the address and r/w byte.
	 *
	 * In write mode the conditional dumps us to the end of the ISR.  In read mode we end up here.
	 */

    } // current byte is the address

    if( I2CSTATbits.R_NOT_W == I2C_RW_W )
    {
	PIN_HMI2 ^= 1;
	
	if (current_mode == I2C_M_WA_0)
	{
	    /*
	     This is the first byte of the address write
	     */
	    
	    current_counter = work_byte;	    // The first byte sent is bits 8-15. So we write and shift 8 to the left.
	    current_counter = (current_counter << 8);
	    
	    current_mode = I2C_M_WA_1;
	    
	}
	else if(current_mode == I2C_M_WA_1)
	{
	    current_counter = current_counter | work_byte;
	    current_mode = I2C_M_W;
	}
	else
	{
	    /*
	     Error condition.
	     */
	    PIN_HMI1 = 1;
	    PIN_HMI2 = 1;
	    PIN_HMI3 = 1;
	    PIN_HMI4 = 1;
	}
    }
    else
    {
	/*
	 * We're in read mode
	 */
	
	PIN_HMI3 ^= 1;

	work_byte = ( ( unsigned char * ) &eeprom_data )[current_counter];
	current_counter += 1;

	//WAIT_FOR_TBF( ); // Wait for transmit buffer to become empty.
	I2CTRN = work_byte;
    } // slave read mode


__end_slave_isr:

    if ( current_counter >= sizeof (eeprom_data ) )
    {
	current_counter = 0;
    }

    SLAVE_IF = 0; // Clear the slave ISR flag
    I2CCONbits.SCLREL = 1; //	Release the clock.  Let stuff continue to happen.

    return;
}

void  __linker_fixup_i2c_slave_isr( void )
{
    Nop( );
    return;
}

/*
 * EOF
 */
