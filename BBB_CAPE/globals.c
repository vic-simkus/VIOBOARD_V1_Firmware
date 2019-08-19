#include "globals.h"
#include "config.h"

volatile uint16_t _cmd_state = GF_STATE_NONE;
volatile uint16_t _cmd_buffer_idx = 0;

volatile unsigned char _cmd_buffer[CMD_BUFFER_SIZE];
volatile uint16_t _g_pwm_percent = 0;

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

// 0.0002000 = 200uA/V from the TI INA138 current shunt monitor.
const float _g_i_calc_denom = SENSE_RESISTOR * 0.000200 * GAIN_RESISTOR;
const float _g_v_per_ads = ( 3.3006F / ( float ) 4095 );
volatile float _g_adc_reading_avg = 0;

float _g_pwm_percent_f = 0;

volatile uint16_t _g_manual_drive = 0;