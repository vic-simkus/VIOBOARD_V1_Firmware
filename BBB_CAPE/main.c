#include "chip_config.h"
#include "config.h"
#include "setup.h"
#include "isrs.h"
#include "pwm.h"
#include "globals.h"
#include "adc.h"

#include <stdio.h>	//printf
#include <stdlib.h>
#include <libpic30.h> //atoi

static void process_bbb_reset( void )
{
    char reset = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_RESET_MASK );

    if( reset == 0 )
    {
	// Reset line is low.  BBB is held in reset
	printf( "\nRESET is low.  Will raise.\n" );
	PIN_BBB_RESET = 1;
    }
    else
    {
	// Reset line is high.  BBB is operational
	printf( "\nRESET is high.  Will pull down.\n" );
	PIN_BBB_RESET = 0;
    }

    return;

}

static void dump_pwm_table( void )
{
    unsigned int i = 0;
    for(; i < PWM_PERC_ENTRIES; i++ )
    {
	printf( "%3d: 0x%04X\n", i, _pwm_percent_table[i] );
    }
}

static void dump_status_info( void )
{
    printf( "*** STATUS ***\n" );

    printf( "ADC step: %f V/step\n\n", ( double ) _g_v_per_ads );

    if( _g_enable_drive == 0 )
    {
	printf( "Drive is DISABLED.\n" );
    }
    else
    {

	printf( "Drive is ENABLED\n" );

	printf( "PWMd: %03u%%", _g_pwm_percent );
	printf( "   PWMf: %f%%\n", ( double ) _g_pwm_percent_f );
    }

    printf( "Vadc: %f V\n", ( double ) _g_adc_reading_v );
    printf( "I: %f A\n", ( double ) _g_curr_draw );

    char aux_1 = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_AUX_1_MASK );
    char aux_2 = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_AUX_2_MASK );
    char enable = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_ENABLE_MASK );
    char reset = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_RESET_MASK );
    char power = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_POWER_MASK );

    printf( "\nAUX_1: %d\n", aux_1 );
    printf( "AUX_2: %d\n", aux_2 );
    printf( "ENABLE: %d\n", enable );
    printf( "RESET: %d\n", reset );
    printf( "POWER: %d\n", power );

    putchar( '\n' );

    printf( "*** STATUS ***\n" );

    return;
}

static void process_cmd_set_duty( void )
{
    if( _cmd_buffer_idx >= 6 || _cmd_buffer_idx < 3 )
    {
	printf( "Input error.\n" );
	return;
    }

    unsigned int duty_percent = ( unsigned int ) atoi( ( const char * ) _cmd_buffer + 2 );

    if( duty_percent > 100 )
    {
	printf( "Input error.\n" );
	duty_percent = 0;
    }

    if( duty_percent == 0 )
    {
	_g_manual_drive = 0;
    }
    else
    {
	_g_manual_drive = 1;
    }

    pwm_set_duty_cycle_percent( duty_percent );

    printf( "Duty: %d%%\n", duty_percent );

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
    adc_setup( );
    setup_control_pins( );
    setup_serial( );
    setup_heart_beat( );
    setup_hmi_leds( );
    setup_control_timer( ); // Starts the output drive

    pwm_set_duty_cycle( 0 );

    printf( "\nReady.\n" );

    char aux_1 = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_AUX_1_MASK );
    char aux_2 = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_AUX_2_MASK );
    char enable = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_ENABLE_MASK );
    char reset = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_RESET_MASK );
    char power = PIN_CHECK_VALUE( _g_cn_state, G_CN_PIN_POWER_MASK );

    for(;; )
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
		printf( "  AUX_1: %d ", aux_1 );
		printf( "  AUX_2: %d ", aux_2 );
		printf( "  ENABLE: %d ", enable );
		printf( "  RESET: %d ", reset );
		printf( "  POWER: %d ", power );

		putchar( '\n' );
	    }

	    if( enable == 0 )
	    {
		_g_enable_drive = 0;
		PIN_RS232_ENABLE = 0;

		_g_adc_reading_avg = 0;
		AD1CON1bits.ASAM = 0;
		adc_setup_for_ss( );
	    }
	    else
	    {
		if( _g_enable_drive == 0 )
		{
		    _g_enable_drive = 1;
		    PIN_RS232_ENABLE = 1;
		    //adc_setup_for_cont( );
		    //AD1CON1bits.ASAM = 1;
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
		case 't':
		    /*
		     * Dump PWM table
		     */
		    dump_pwm_table( );
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
		    printf( "*** ? ***\n" );
		    printf( "b - Toggle BBB reset line\n" );
		    printf( "s - Status\n" );
		    printf( "t - PWM table\n" );
		    printf( "r - Reset VPSB\n" );
		    printf( "e - Enable output\n" );
		    printf( "d - Disable output\n" );
		    printf( "*** ? ***\n" );
		    break;
		default:
		    printf( "!\n" );
		    break;
	    }
	}
	else
	{
	    /*
	     * Multiple character command
	     */

	    switch( _cmd_buffer[0] )
	    {
		case 'd':
		    process_cmd_set_duty( );
		    break;
		default:
		    printf( "!!\n" );
		    break;

	    }
	}

	_cmd_buffer_idx = 0;
	_cmd_state = GF_STATE_NONE;
    }

}