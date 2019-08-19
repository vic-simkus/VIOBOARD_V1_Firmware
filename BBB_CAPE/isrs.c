#include "isrs.h"

#include "config.h"
#include "setup.h"
#include "globals.h"

#include "pwm.h"

void __linker_fixup_isr( void )
{
    return;
}

volatile uint16_t _htbt_direction = HTBT_DIRECTION_UP;
volatile uint16_t _htbt_pause = 0;

void __attribute__( ( interrupt( auto_psv ) ) ) _ADC1Interrupt( void )
{
    _g_adc_reading_avg = ADC1BUF0;
    _g_adc_reading_avg += ADC1BUF1;
    _g_adc_reading_avg += ADC1BUF2;
    _g_adc_reading_avg += ADC1BUF3;
    _g_adc_reading_avg += ADC1BUF4;
    _g_adc_reading_avg += ADC1BUF5;
    _g_adc_reading_avg += ADC1BUF6;
    _g_adc_reading_avg += ADC1BUF7;
    /*
	_g_adc_reading_avg += ADC1BUF8;
	_g_adc_reading_avg += ADC1BUF9;
	_g_adc_reading_avg += ADC1BUF10;
	_g_adc_reading_avg += ADC1BUF11;
	_g_adc_reading_avg += ADC1BUF12;
	_g_adc_reading_avg += ADC1BUF13;
	_g_adc_reading_avg += ADC1BUF14;
	_g_adc_reading_avg += ADC1BUF15;
     */

    _g_adc_reading_avg = _g_adc_reading_avg / 8.0F;

    IFS0bits.AD1IF = 0;
    return;
}

void __attribute__( ( interrupt( auto_psv ) ) ) _CNInterrupt( void )
{
    /*
     * Change notification ISR.  We end up here if any of the monitored pins goes from high to low or vice versa.
     * At this point we don't dont what changed and we don't really care.  That's for the logic loop to figure out.
     */

    // Pack all of the pin states into an integer.
    _g_cn_state = PIN_PACK_VALUES( );

    // Flag the state as changed.
    _g_cn_state_change = G_CN_STATE_CHANGE;

    // Clear change notification IFS flag
    IFS1bits.CNIF = 0;
}

void __attribute__( ( interrupt( auto_psv ) ) ) _T2Interrupt( void )
{
    /*
     * Control loop timer ISR.
     */
    //PORTBbits.RB13 = !PORTBbits.RB13;

    _g_adc_reading_v = _g_v_per_ads * ( float ) _g_adc_reading_avg;
    _g_curr_draw = _g_adc_reading_v / _g_i_calc_denom;

    float output = _g_pwm_percent_f;

    if( _g_manual_drive == 0 )
    {
	if( _g_enable_drive == 1 )
	{
	    if( output < 100 )
	    {
		output += 1;

		if( output > 100 )
		{
		    output = 100;
		}

		pwm_set_duty_cycle_percent_f( output );
	    }
	}
	else
	{
	    pwm_set_duty_cycle( 0 );
	    _g_pwm_percent_f = 0;
	    _g_pwm_percent = 0;
	}
    }

    IFS0bits.T2IF = 0; // Reset ISR flag
    return;
}

void __attribute__( ( interrupt( auto_psv ) ) ) _U1RXInterrupt( void )
{
    /*
     * We must read in the next character otherwise we overflow
     */
    uint8_t read_char = U1RXREG;

    if( _cmd_state != GF_STATE_NONE )
    {
	/*
	 * We haven't finished processing the last command.  Bail.
	 * Character is lost.
	 */

	goto __end_isr;

    }

    /*
     * We have finished processing the last command.
     */

    if( read_char == KEY_CR ) // Carriage return
    {
	_cmd_state = GF_STATE_HAVE_COMMAND;
	_cmd_buffer[_cmd_buffer_idx] = 0;
	goto __end_isr;
    }
    else
    {
	/*
	 * Modulo buffer addressing.  Wrap around and lose data.
	 */
	_cmd_buffer[_cmd_buffer_idx] = read_char;
	_cmd_buffer_idx = ( _cmd_buffer_idx + 1 ) % CMD_BUFFER_SIZE;
    }

__end_isr:
    IFS0bits.U1RXIF = 0; // Clear receive interrupt flag.
    return;
}

/**
 * Burns CPU cycles while lighting up LEDs in a visually pleasing fashion.
 */
void __attribute__( ( interrupt( auto_psv ) ) ) _T1Interrupt( void )
{
    /*
     * Do the smooth dimming heartbeat here.
     */

    if( R_HTBT_DUTY >= _pwm_multx05_100 && _htbt_direction == HTBT_DIRECTION_UP )
    {
	_htbt_pause += 1;

	if( _htbt_pause == HTBT_PAUSE_INTERVAL_UP )
	{
	    _htbt_direction = HTBT_DIRECTION_DOWN;
	    _htbt_pause = 0;
	}
    }
    else if( R_HTBT_DUTY == 0 && _htbt_direction == HTBT_DIRECTION_DOWN )
    {
	_htbt_pause += 1;

	if( _htbt_pause == HTBT_PAUSE_INTERVAL_DOWN )
	{
	    _htbt_direction = HTBT_DIRECTION_UP;
	    R_HTBT_DUTY = 0;
	    _htbt_pause = 0;
	}
    }

    if( _htbt_pause == 0 )
    {
	if( _htbt_direction == HTBT_DIRECTION_UP )
	{
	    R_HTBT_DUTY = R_HTBT_DUTY + _pwm_percent_table[5];
	}
	else
	{
	    R_HTBT_DUTY = R_HTBT_DUTY - _pwm_percent_table[5];
	}
    }

    /*
     * End heartbeat
     */

    IFS0bits.T1IF = 0; // Reset interrupt flag
    ClrWdt( ); // Clear watchdog timer

    return;
}
