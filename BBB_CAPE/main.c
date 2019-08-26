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

#include "chip_config.h"
#include "config.h"
#include "setup.h"
#include "isrs.h"
#include "pwm.h"
#include "globals.h"
#include "adc.h"

#include "I2C.h"

#include <stdio.h>	//printf
#include <stdlib.h>
#include <libpic30.h> //atoi

/**
 Changes the BBB RESET pin state based on user input.
 */
static void process_bbb_reset( void )
{
    char reset = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_RESET_MASK );

    PIN_HMI1 = 0;
    PIN_HMI2 = 0;
    PIN_HMI3 = 0;
    PIN_HMI4 = 0;

    if( reset == 0 )
    {
	// Reset line is low.  BBB is held in reset
	printf( "\nR - PU.\n" );
	PIN_BBB_RESET = 1;
    }
    else
    {
	// Reset line is high.  BBB is operational
	printf( "\nR - PD.\n" );
	PIN_BBB_RESET = 0;
    }

    return;

}

static void dump_status_info( void )
{
    printf( "***\n" );

    if( _g_enable_drive == 0 )
    {
	printf( "Drive is ON\n" );
    }
    else
    {
	printf( "Drive is OFF\n" );
    }

    printf( "ADC step: %f V/step\n\n", ( double ) _g_v_per_ads );
    printf( "Vadc: %f V\n", ( double ) _g_adc_reading_v );
    printf( "Is: %f A\n", ( double ) _g_curr_draw );

    printf( "***\n" );

    return;
}

int main( void )
{
    ANSA = 0;
    ANSB = 0;

    TRISBbits.TRISB13 = 0;

    INTCON1bits.NSTDIS = 0;

    __linker_fixup_isr( );

    setup_clock( );
    setup_pwm( );
    setup_control_pins( );
    setup_serial( );
    setup_control_timer( ); // Starts the output drive
    pwm_set_duty_cycle( 0 );

    setup_heart_beat( );
    setup_hmi_leds( );

    i2c_init( 5, 0x54 );

    adc_setup( );

    printf( "\nR\n" );

    char aux_1 = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_AUX_1_MASK );
    char aux_2 = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_AUX_2_MASK );
    char enable = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_ENABLE_MASK );
    char reset = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_RESET_MASK );
    char power = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_POWER_MASK );

    for( ; ; )
    {
	while( _cmd_state != GF_STATE_HAVE_COMMAND )
	{
	    if( _g_cn_state_change == G_CN_STATE_CHANGE )
	    {
		_g_cn_state_change = G_CN_STATE_NONE;

		aux_1 = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_AUX_1_MASK );
		aux_2 = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_AUX_2_MASK );
		enable = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_ENABLE_MASK );
		reset = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_RESET_MASK );
		power = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_POWER_MASK );

		printf( "CN: " );
		printf( " A1: %d ", aux_1 );
		printf( " A2: %d ", aux_2 );
		printf( " E: %d ", enable );
		printf( " R: %d ", reset );
		printf( " P: %d\n", power );
	    }

	    if( enable == 0 )
	    {
		_g_enable_drive = 0;
		PIN_RS232_ENABLE = 0;
	    }
	    else
	    {
		if( _g_enable_drive == 0 )
		{
		    _g_enable_drive = 1;
		    PIN_RS232_ENABLE = 1;
		}
	    }
	}

	if( _cmd_buffer_idx < 1 )
	{
	    /*
	     * We're expecting at least one character command
	     */
	}
	else if( _cmd_buffer_idx == 1 )
	{
	    /*
	     * Single character command
	     */

	    switch( _cmd_buffer[0] )
	    {
		case 'b':
		    process_bbb_reset( );
		    break;
		case 's':
		    /*
		     * Dump status
		     */
		    dump_status_info( );
		    break;
		case 'c':
		    _g_curr_offset = 0 - _g_curr_draw;
		    break;
		case 'r':
		    __asm__ volatile ("reset" );
		    break;
		case 'e':
		    _cmd_state = GF_STATE_HAVE_COMMAND;
		    enable = 1;
		    break;
		case 'd':
		    _cmd_state = GF_STATE_HAVE_COMMAND;
		    enable = 0;
		    break;
		case '?':
		    printf( "* ? *\n" );
		    printf( "b - Toggle reset line\n" );
		    printf( "s - Status\n" );
		    printf( "r - Reset\n" );
		    printf( "e - Enable \n" );
		    printf( "d - Disable ''\n" );
		    printf( "* ? *\n" );
		    break;
		default:
		    printf( "!\n" );
		    break;
	    }
	}
	else
	{
	    printf( "!!\n" );

	}

	_cmd_buffer_idx = 0;
	_cmd_state = GF_STATE_NONE;
    }

}