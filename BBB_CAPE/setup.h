/*
 * File:   setup.h
 * Author: vic
 *
 * Created on September 9, 2018, 4:34 PM
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

