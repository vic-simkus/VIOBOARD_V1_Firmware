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

#ifndef SETUP_H
#define	SETUP_H
#include "config.h"

/**
 * Sets up the system instruction clock.
 */
extern void setup_clock(void);

/**
 * Sets up the heart-beat LED and associated Timer 1
 */
extern void setup_heart_beat(void);

/**
 * Sets up the main drive PWM
 */
extern void setup_pwm(void);

/**
 * Sets up the superfluous HMI LEDs
 */
extern void setup_hmi_leds(void);

/**
 * Sets up the serial port and associated ISR
 */
extern void setup_serial(void);

/**
 * Sets up the ADC and associated Timer 5
 */
extern void setup_adc(void);

/**
 * Sets up the control loop Time 2
 */

extern void setup_control_timer(void);

extern void setup_control_pins(void);


#endif	/* SETUP_H */

