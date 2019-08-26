#include "pwm.h"

#include "config.h"
#include "setup.h"
#include "globals.h"

#include <math.h>

void pwm_set_duty_cycle_percent_f( float _percent )
{
    float multiplier = 0;
    float fl = floorf( _percent );

    multiplier = _percent - fl;

    if( multiplier < 0.001 ) // Account for floating point imperfections.
    {
	multiplier = 0;
    }

    uint16_t duty_i = 0; // Integer duty cycle value for the closest lower whole percent duty cycle

    /*
     * If percent is <1, we multiple the duty cycle value for 1%
     * by the fractional duty cycle requested
     */
    if( _percent < 1 )
    {
	duty_i = _pwm_percent_table[1];
    }
    else
    {
	duty_i = _pwm_percent_table[( uint16_t ) fl];
    }

    uint16_t duty_calc = 0; // Calculated integer duty cycle value for the requested duty cycle percent

    if( fl < 1 )
    {
	/*
	 * If floor of supplied percent is <1 then the request is for duty cycle less than 1
	 */
	duty_calc = ( ( float ) duty_i ) * ( multiplier );
    }
    else
    {
	/*
	 * Otherwise add to the integer duty cycle value.
	 */
	duty_calc = duty_i + ( _g_pwm_1_percent * multiplier );
    }

    pwm_set_duty_cycle( duty_calc );
    _g_pwm_percent_f = _percent;
    _g_pwm_percent = _percent;

    return;

}

void pwm_set_duty_cycle_percent( unsigned int _percent )
{
    pwm_set_duty_cycle( _pwm_percent_table[_percent] );
    _g_pwm_percent = _percent;

    return;
}

void pwm_set_duty_cycle( unsigned int _duty )
{
    if( _duty == 0 )
    {
	R_PWM_DUTY = 0;

	// VIC IEC1bits.OC3IE = 0;
	// VIC T5CONbits.TON = 0;
    }
    else
    {
	R_PWM_DUTY = _duty;
	// VIC IEC1bits.OC3IE = 1;
    }

    return;
}
