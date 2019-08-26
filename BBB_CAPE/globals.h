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

#ifndef GLOBALS_H
#define	GLOBALS_H

#include <stdint.h>

#define GF_STATE_NONE		0			// No state.  Nothing is happening
#define GF_STATE_HAVE_COMMAND	1			// Have received a CR.  Main loop will process
#define GF_STATE_I2C		3

#define G_CN_STATE_NONE		0
#define G_CN_STATE_CHANGE	1

#define G_CN_PIN_ENABLE_MASK	0b00000001
#define G_CN_PIN_AUX_1_MASK	0b00000010
#define G_CN_PIN_AUX_2_MASK	0b00000100
#define G_CN_PIN_RESET_MASK	0b00001000
#define G_CN_PIN_POWER_MASK	0b00010000

#define G_CN_PIN_ALL_MASK		0b00011111

#define CMD_BUFFER_SIZE	32

#define KEY_CR		0x0D

#define PWM_PERC_ENTRIES 101

#define OP_AMP_GAIN	((float)4.1290106952)

/**
 * Current state of the command processor.  Updated in the serial receive ISR.
 */
volatile extern uint16_t _cmd_state;

/**
 * Command input buffer.  Populated by the serial receive ISR.
 */
volatile extern unsigned char _cmd_buffer[CMD_BUFFER_SIZE];

/**
 * Index into _cmd_buffer
 */
volatile extern uint16_t _cmd_buffer_idx;

/**
 * Current PWM duty cycle percent.  Set by pwm_set_duty_cycle
 */
volatile extern uint16_t _g_pwm_percent;
extern float _g_pwm_percent_f;

/**
 * Volts per ADC step.  (2^12)/AVdd
 * Set in globals.c
 */
extern const float _g_v_per_ads;

/**
 * Precalculated PWM duty cycle values index on percentage. Populated by pwm_precalc_values.
 */
extern uint16_t _pwm_percent_table[PWM_PERC_ENTRIES];

/**
 * Precalculated square roots of PWM duty cycle/period ratios. Populated by pwm_precalc_values.
 */
extern float _pwm_duty_sqrt_table[PWM_PERC_ENTRIES];

/// We're fudging around the const-ness of the two variables.  We want them to be const for everyone except setup.c
#ifndef __FUDGE_CONST

/**
 * 100 percent calculated as duty cycle value of 10% * 10.  The idea is account for any rounding errors in table pre-calculation.
 */
extern const uint16_t _pwm_multx10_100;

/**
 * 100 percent calculated as duty cycle value of 5% * 20.  The idea is account for any rounding errors in table pre-calculation.
 */
extern const uint16_t _pwm_multx05_100;

extern const float _g_pwm_1_percent;
#endif

/**
 * Averaged reading of the ADC.  Defined and populated in asm_isrs.s
 */
volatile extern float _g_adc_reading_avg;

/**
 * Calculated RMS voltage of the square wave across the sense resistor.
 */
volatile extern float _g_adc_reading_v;

/**
 * Calculated amp draw in milliamps
 */
volatile extern float _g_curr_draw;

/**
 * The denominator for solving the INA138 transfer function.
 * Value set in globals.c
 */
const extern float _g_i_calc_denom;

/**
 * Current draw limit in milliamps.
 */
const extern uint16_t _g_amp_draw_limit;

volatile extern uint16_t _g_cn_state_change;

volatile extern uint16_t _g_cn_state;

volatile extern uint16_t _g_enable_drive;

volatile extern uint16_t _g_manual_drive;

extern volatile float _g_curr_offset;

#endif	/* GLOBALS_H */

