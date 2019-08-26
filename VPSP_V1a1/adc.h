/*
 * File:   adc.h
 * Author: vic
 *
 * Created on July 27, 2019, 9:41 AM
 */

#ifndef ADC_H
#define	ADC_H

extern void adc_setup( void );
extern double adc_s_get_internal_bandgap( void );
extern double adc_s_get_vdd( void );
extern double adc_s_get_av( void );
extern void adc_setup_for_ss( void );
extern void adc_setup_for_cont( void );

#endif	/* ADC_H */

