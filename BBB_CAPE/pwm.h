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

#ifndef PWM_H
#define	PWM_H

extern void pwm_set_duty_cycle_percent(unsigned int _percent );
extern void pwm_set_duty_cycle_percent_f(float _percent);
extern void pwm_set_duty_cycle(unsigned int _duty_cycle);


#endif	/* PWM_H */

