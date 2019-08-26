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

#include "globals.h"
#include "config.h"

volatile uint16_t _cmd_state = GF_STATE_NONE;
volatile uint16_t _cmd_buffer_idx = 0;

volatile unsigned char _cmd_buffer[CMD_BUFFER_SIZE];

uint16_t _pwm_percent_table[101];
float _pwm_duty_sqrt_table[101];

const uint16_t _pwm_multx10_100;
const uint16_t _pwm_multx05_100;

volatile uint16_t _g_cn_state_change = 0;
volatile uint16_t _g_cn_state = 0;
volatile uint16_t _g_enable_drive = 0;

const float _g_pwm_1_percent;
volatile float _g_adc_reading_v = 0;
volatile float _g_curr_draw = 0;
volatile float _g_curr_offset = 0;

// 0.000200 = 200uA/V from the TI INA138 current shunt monitor.
const float _g_i_calc_denom = SENSE_RESISTOR * 0.000200 * GAIN_RESISTOR;
const float _g_v_per_ads = ( 3.3006F / ( float ) 4095 );
volatile float _g_adc_reading_avg = 0;

float _g_pwm_percent_f = 0;
