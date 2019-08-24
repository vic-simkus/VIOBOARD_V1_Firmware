#include "setup.h"
#include <libpic30.h>
#include <math.h>
#include <stdio.h>

#define __FUDGE_CONST
#include "globals.h"

/*
 * Fudge the const qualifier for these two
 */
extern uint16_t _pwm_multx10_100;
extern uint16_t _pwm_multx05_100;
extern float _g_pwm_1_percent;

void setup_control_pins( void )
{
    PIN_AUX1_TRIS = 1;
    PIN_AUX2_TRIS = 1;
    PIN_ENABLE_TRIS = 1;
    PIN_RS232_ENABLE_TRIS = 0;

    PIN_BBB_RESET_TRIS = 0;
    PIN_BBB_POWER_TRIS = 0;

    PIN_BBB_RESET = 0;
    PIN_BBB_POWER = 1;
    PIN_RS232_ENABLE = 0;

    _g_cn_state = PIN_PACK_VALUES( );

    CNPD1bits.CN0PDE = 1; // Enable pull-down resistors
    CNPD1bits.CN1PDE = 0; // Disable pull-down resistors -- BBB RESET
    CNPD1bits.CN9PDE = 0; // Disablepull-down resistors -- BBB POWER

    CNPD1bits.CN7PDE = 1; // Enable pull-down resistors
    CNPD2bits.CN30PDE = 1; // Enable pull-down resistors

    CNPU1bits.CN0PUE = 0; // Disable pull-up resistors
    CNPU1bits.CN1PUE = 0; // Disable pull-down resistors -- BBB RESET
    CNPU1bits.CN9PUE = 0; // Disable pull-down resistors -- BBB POWER
    CNPU1bits.CN7PUE = 0; // Disable pull-up resistors
    CNPU2bits.CN30PUE = 0; // Disable pull-up resistors


    CNEN1bits.CN0IE = 1; // Enable CN interrupt for CN0 (ENABLE)
    CNEN1bits.CN1IE = 1; // Enable CN interrupt for CN1 (BBB RESET)
    CNEN1bits.CN9IE = 1; // Enable CN interrupt for CN9 (BBB POWER)
    CNEN1bits.CN7IE = 1; // Enable CN interrupt for CN7 (AUX1)
    CNEN2bits.CN30IE = 1; // Enable CN interrupt for CN30 (AUX2)

    IFS1bits.CNIF = 0; // Clear change notification IFS flag
    IEC1bits.CNIE = 1; // Enable change notification interrupts
    
    //IPC4bits.CNIP = 0x06  // Change notification ISR priority

    return;
}

void setup_control_timer( void )
{
    T2CON = 0; // SOP
    T2CONbits.TCKPS = 1; // 1:8 prescaler
    PR2 = 19841; // We want the timer to fire 100 timer per second - 100Hz
    // Our Tcy is 63nS or 0.000,000,063 of a second
    // Prescaler i 8
    // ((1÷100)÷0.000,000,063)÷8

    IPC1bits.T2IP = 5; // Set T2 timer interrupt priority
    IFS0bits.T2IF = 0; // Clear interrupt flag just in case
    IEC0bits.T2IE = 1; // Turn on the interrupt

    T2CONbits.TON = 1; // Turn on timer

    return;
}

void setup_serial( void )
{
    U1MODE = 0; // SOP
    U1STA = 0; // SOP

    TRISBbits.TRISB7 = 0; // Set the TX pin to output
    TRISBbits.TRISB2 = 1; // Set the RX pin to input

    U1MODEbits.STSEL = 0x01; // 1 stop bit;
    U1MODEbits.PDSEL = 0x00; // 8 bit no parity
    U1MODEbits.BRGH = 0; // Slow mode
    U1BRG = 8; // Set baud: 115K

    U1STAbits.URXISEL = 0x01; // Enable receive interrupt on every character

    IEC0bits.U1RXIE = 1; // Enable receive ISR
    IPC2bits.U1RXIP = 0x02; // Second lowest priority.  Only T1 is lower.

    U1MODEbits.UARTEN = 1; // Enable the UART
    U1STAbits.UTXEN = 1; // Enable the transmit pin

    printf( "\nUART^\n" );

    return;

}

/*
 * Precalculates the various values used in PWM management.
 */
static void pwm_precalc_values( void )
{
    uint16_t i = 0;

    _pwm_percent_table[0] = 0;
    _pwm_duty_sqrt_table[0] = 0;

    for( i = 1; i <= 100; i++ )
    {
	_pwm_percent_table[i] = ( uint16_t ) ( ( float ) R_PWM_PERIOD * ( ( float ) i / 100 ) );
	_pwm_duty_sqrt_table[i] = ( float ) ( sqrt( ( float ) _pwm_percent_table[i] / ( float ) R_PWM_PERIOD ) );
    }

    _pwm_multx10_100 = _pwm_percent_table[10] * 10;
    _pwm_multx05_100 = _pwm_percent_table[5] * 20;

    _g_pwm_1_percent = R_PWM_PERIOD / 100;

    return;
}

/**
 * Sets up timer 1 and the OC2 for heartbeat LED output.
 */
void setup_heart_beat( void )
{
    TRISBbits.TRISB11 = 0;

    uint16_t i = 0;

    for( i = 0; i < 5; i++ )
    {
	__delay_ms( 100 );
	P_HTBT_PIN ^= 1;
    }

    TR_HTBT_BITS = 0; // Set the tristate config for the pin as output
    P_HTBT_PIN = 0; // Clear the pin for fun

    /*
     * We're using OC3 for our PWM output.
     */

    HTBT_OCCON1 = 0; // Clear as a matter of SOP
    HTBT_OCCON2 = 0; // Clear as a matter of SOP

    TR_HTBT_BITS = 0; // Set pin to output
    P_HTBT_PIN = 0; // Clear the pin just for fun

    HTBT_OCCON1bits.OCTSEL = 0b111; // System clock as source of clock

    R_HTBT_PERIOD = PWM_PERIOD; // With peripheral clock post-scaling disabled, and Fcy=16Mhz, the period is 244Hz (accoding to datasheet)
    R_HTBT_DUTY = 0; // Set duty cycle

    HTBT_OCCON2bits.SYNCSEL = 0x1F; // The Ref manual says to do this.  The data sheet says not to do this... I feel like a child caught in an ugly divorce
    HTBT_OCCON1bits.OCM = 0b110; // Set edge aligned PWM mode and start

    R_HTBT_DUTY = _pwm_percent_table[0];

    T1CON = 0; // SOP
    T1CONbits.TCS = 0; //Internal clock; Fosc / 2
    T1CONbits.TCKPS = 0b11; // 1:256 pre-scaler
    PR1 = 6250; // 10 times per second;

    IPC0bits.T1IP = 0x01; // Priority 1 -- lowest priority interrupt
    IEC0bits.T1IE = 1; // Enable T1 interrupts.
    T1CONbits.TON = 1; // Turn on the timer

    printf( "HTBT^\n" );

    return;
}

/*
 * Sets up the system clock.
 */
void setup_clock( void )
{
    CLKDIVbits.RCDIV = 0; // Set post-scaler to 1:1
    CLKDIVbits.DOZEN = 0; // Peripheral clock ratio 1:1

    return;
}

/*
 * Sets up the main PWM drive on OC3
 */
void setup_pwm( void )
{
    /*
     * We're using OC3 for our PWM output.
     */

    PWM_OCCON1 = 0; // Clear as a matter of SOP
    PWM_OCCON2 = 0; // Clear as a matter of SOP

    TR_PWM_BITS = 0; // Set pin to output
    P_PWM_PIN = 0; // Clear the pin just for fun

    PWM_OCCON1bits.OCTSEL = 0b111; // System clock as source of clock

    //R_PWM_PERIOD = 0xFFFF;				// With peripheral clock post-scaling disabled, and Fcy=16Mhz, the period is 244Hz (accoding to datasheet)
    R_PWM_PERIOD = PWM_PERIOD; // With peripheral clock post-scaling disabled, and Fcy=16Mhz, the period is 3.9KHz (accoding to datasheet)
    R_PWM_DUTY = 0; // Set duty cycle

    PWM_OCCON2bits.SYNCSEL = 0x1F; // The Ref manual says to do this.  The data sheet says not to do this... I feel like a child caught in an ugly divorce
    PWM_OCCON1bits.OCM = 0b110; // Set edge aligned PWM mode and start

    //PWM_OCCON2bits.OCMPINV = 1;		// Invert the output


    /*
     * This has to be last because it utilizes the register values.
     */
    pwm_precalc_values( );

    return;
}

void setup_hmi_leds( void )
{  
    PIN_HMI1_TRIS = 0;
    PIN_HMI2_TRIS = 0;
    PIN_HMI3_TRIS = 0;
    PIN_HMI4_TRIS = 0;    

    PIN_HMI1 = 1;
    PIN_HMI2 = 1;
    PIN_HMI3 = 1;
    PIN_HMI4 = 1;
   
      __delay_ms(250);
    
    PIN_HMI1 = 0;
    PIN_HMI2 = 0;
    PIN_HMI3 = 0;
    PIN_HMI4 = 0;
  
    printf("HMI^\n");
    return;

}
