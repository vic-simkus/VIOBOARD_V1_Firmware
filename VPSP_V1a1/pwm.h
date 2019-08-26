/*
 * File:   pwm.h
 * Author: vic
 *
 * Created on September 9, 2018, 4:41 PM
 */

#ifndef PWM_H
#define	PWM_H

extern void pwm_set_duty_cycle_percent(unsigned int _percent );
extern void pwm_set_duty_cycle_percent_f(float _percent);
extern void pwm_set_duty_cycle(unsigned int _duty_cycle);


#endif	/* PWM_H */

