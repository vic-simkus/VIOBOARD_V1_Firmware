/*
 * File:   config.h
 * Author: vic
 *
 * Created on September 9, 2018, 9:11 AM
 */

#ifndef CONFIG_H
#define	CONFIG_H

/*
 * Define the FCY for various delay functions and what not.
 *
 * We get FCY=16Mhz by: (8Mhz FRC * 4x PLL)/2
 */
#define FCY 16000000
#define FCY_PERIOD_nS	63		// 1 second/16000000
#define FCY_uSEC		16		// 1000/63 = 15.873.  16 is 1008 nanoseconds.


/**
 * Current control PWM duty cycle register
 */
#define R_PWM_DUTY		OC3R

/**
 * Current control PWM period register
 */
#define R_PWM_PERIOD	OC3RS

/**
 * Heartbeat PWM duty cycle register
 */
#define R_HTBT_DUTY		OC2R

/**
 * Heartbeat PWM period register
 */
#define R_HTBT_PERIOD	OC2RS

/**
 * Current control PWM output tristate register bits
 */
#define TR_PWM_BITS	TRISBbits.TRISB10
/**
 * Heartbeat output tristate register bits
 */
#define TR_HTBT_BITS	TRISBbits.TRISB11

/**
 * Current control PWM output pin
 */
#define P_PWM_PIN		PORTBbits.RB10
/**
 * Heartbeat output pin
 */
#define P_HTBT_PIN		PORTBbits.RB11

/**
 * Current control PWNM OC (output compare) module configuration registers
 */
#define PWM_OCCON1	OC3CON1
#define	PWM_OCCON2	OC3CON2

/**
 * Heartbeat OC (output compare) module configuration registers
 */
#define HTBT_OCCON1	OC2CON1
#define	HTBT_OCCON2	OC2CON2

/**
 * Current control PWNM OC (output compare) module configuration register bits
 */
#define PWM_OCCON1bits OC3CON1bits
#define PWM_OCCON2bits OC3CON2bits

/**
 * Heartbeat OC (output compare) module configuration register bits
 */
#define HTBT_OCCON1bits OC2CON1bits
#define HTBT_OCCON2bits OC2CON2bits

/**
 * Direction of the heartbeat LED brightness.
 */
/**
 * Brightness is going up (getting brighter)
 */
#define HTBT_DIRECTION_UP	1
/**
 * Brightness is going down (getting dimmer)
 */
#define HTBT_DIRECTION_DOWN 0

/**
 * How many timer click to pause at the top and bottom of the heartbeat LED brightness
 */
#define HTBT_PAUSE_INTERVAL_UP 14
#define HTBT_PAUSE_INTERVAL_DOWN 7


// 0xFFFF -- With peripheral clock post-scaling disabled, and Fcy=16Mhz, the period is 244Hz (accoding to datasheet)

/*
 *  0xFFFF == 244Hz	16 bits resolution
 *  0x7FFF == 488Hz	15 bits resolution
 *  0x0FFF == 3.9KHz	12 bits resolution
 *  0x03FF == 15.6KHz	10 bits resolution
 *  0x007F == 125KHz	07 bits resolution
 *  0x001F == 500KHz	05 bits resolution
 */

//#define PWM_PERIOD		0xFFFF
#define PWM_PERIOD		0x7FFF


#define SENSE_RESISTOR	0.010
#define GAIN_RESISTOR	240000

#define PIN_AUX1		PORTBbits.RB3      //PIN 07 -- RB3 -- CN7 (CNPD1)
#define PIN_AUX2		PORTAbits.RA2      //PIN 09 -- RA2 -- CN30 (CNPD2)
#define PIN_ENABLE		PORTAbits.RA4      //PIN 12 -- RA4 -- CN0 (CNPD1)	-- P8_40

#define PIN_BBB_RESET	PORTBbits.RB4
#define PIN_BBB_POWER	PORTAbits.RA7

#define PIN_BBB_RESET_P	PORTB
#define PIN_BBB_POWER_P	PORTA

#define PIN_PACK_VALUES()   (0x0 | ( (PIN_BBB_POWER << 4) | (PIN_BBB_RESET << 3) | (PIN_AUX2 << 2) | (PIN_AUX1 << 1) | PIN_ENABLE) )

#define PIN_CHECK_VALUE(_source,_mask)  ((_source & _mask) ? 1 : 0)

#define PIN_RS232_ENABLE    LATBbits.LATB5      //PIN 14 -- RB5

#define PIN_AUX1_TRIS		TRISBbits.TRISB3
#define PIN_AUX2_TRIS		TRISAbits.TRISA2
#define PIN_ENABLE_TRIS		TRISAbits.TRISA4
#define PIN_RS232_ENABLE_TRIS	TRISBbits.TRISB5
#define PIN_BBB_RESET_TRIS		TRISBbits.TRISB4
#define PIN_BBB_POWER_TRIS		TRISAbits.TRISA7

#define PIN_HMI1_TRIS		TRISBbits.TRISB12
#define PIN_HMI2_TRIS		TRISBbits.TRISB13
#define PIN_HMI3_TRIS		TRISBbits.TRISB14
#define PIN_HMI4_TRIS		TRISBbits.TRISB15

#define PIN_HMI1		LATBbits.LATB12
#define PIN_HMI2		LATBbits.LATB13
#define PIN_HMI3		LATBbits.LATB14
#define PIN_HMI4		LATBbits.LATB15


/*
 * Various microchip stuffs.
 */
#include <xc.h>

#endif	/* CONFIG_H */



