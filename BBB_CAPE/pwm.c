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
    
    return;

}

void pwm_set_duty_cycle_percent( unsigned int _percent )
{
    pwm_set_duty_cycle( _pwm_percent_table[_percent] );
    
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
