#include "adc.h"
#include "globals.h"

#include <xc.h>
#include <stdint.h>

#define WAIT_FOR_ADC while(AD1CON1bits.SAMP == 1 || AD1CON1bits.DONE == 0) Nop();

void adc_setup( void )
{
    /*
     * Because we don't trust the uC
     */
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CON5 = 0;

    AD1CHS = 0;

    ADC1BUF0 = 0;
    ADC1BUF1 = 0;
    ADC1BUF2 = 0;
    ADC1BUF3 = 0;
    ADC1BUF4 = 0;
    ADC1BUF5 = 0;
    ADC1BUF6 = 0;
    ADC1BUF7 = 0;
    ADC1BUF8 = 0;
    ADC1BUF9 = 0;
    ADC1BUF10 = 0;
    ADC1BUF11 = 0;
    ADC1BUF12 = 0;
    ADC1BUF13 = 0;
    ADC1BUF14 = 0;
    ADC1BUF15 = 0;
    ADC1BUF16 = 0;
    ADC1BUF17 = 0;

    AD1CON1bits.FORM = 0x0; // Absolute decimal result, unsigned, right justified
    AD1CON1bits.SSRC = 0b0111; // Internal counter ends sampling
    AD1CON1bits.ASAM = 0; // Sampling starts when SAMP bit is manually set
    AD1CON1bits.MODE12 = 1; // According to the data sheet we don't have this option.

    AD1CON2bits.PVCFG = 0x01; // Positive reference = External Vref+
    AD1CON2bits.NVCFG = 0x0; // Negative reference = AVss
    AD1CON2bits.BUFREGEN = 0x0; // FIFO mode
    AD1CON2bits.ALTS = 0x0; // Always use SAMPLE A

    AD1CON3bits.ADRC = 0; // Conversion clock derived from system clock
    AD1CON3bits.SAMC = 6; // Sample period is 10Tad or 4.032uS * 10 or 40.32uS
    AD1CON3bits.ADCS = 0b00111111; // Tad = Tcy * 64 -- Tcy = 4032nS.  Tad = 4032nS - 4.032uS

    AD1CON5bits.ASEN = 0; // Auto scan is disabled
    AD1CON5bits.BGREQ = 1; // Bandgap is enabled when ADC is enabled.  Why not...

    AD1CHSbits.CH0NA = 0x0; // Negative input = AVss
    AD1CHSbits.CH0SA = 0x1; // Positive input = AN1


    // AN1 (pin 3) is the ISENSE input
    TRISAbits.TRISA1 = 1; // AN11 is an input
    ANSAbits.ANSA1 = 1; // AN1 is analog input

    adc_setup_for_ss( );


    AD1CON1bits.ADON = 1; // Enable the AD module

    return;

}

void adc_setup_for_ss( void )
{
    AD1CON2bits.SMPI = 0; // Interrupt every conversion
    IFS0bits.AD1IF = 0; // Reset AD interrupt flag
    IPC3bits.AD1IP = 0; // Highest priority
    IEC0bits.AD1IE = 0; // Enable AD interrupts

    return;
}

void adc_setup_for_cont( void )
{
    AD1CON2bits.SMPI = 7; // Interrupt every eigth
    IFS0bits.AD1IF = 0; // Reset AD interrupt flag
    IPC3bits.AD1IP = 7; // Highest priority
    IEC0bits.AD1IE = 1; // Enable AD interrupts

    return;
}

#define CHSA_VBG 0b11010
#define CHSA_AVDD 0b11101

double adc_s_get_internal_bandgap( void )
{
    uint16_t ad1chs_save = AD1CHS;
    ADC1BUF0 = 0;

    AD1CHSbits.CH0SA = CHSA_VBG;

    AD1CON1bits.SAMP = 1;

    WAIT_FOR_ADC;

    AD1CHS = ad1chs_save;
    return _g_v_per_ads * ( float ) ADC1BUF0;
}

extern double adc_s_get_vdd( void )
{
    uint16_t ad1chs_save = AD1CHS;
    ADC1BUF0 = 0;

    AD1CHSbits.CH0SA = CHSA_AVDD;

    AD1CON1bits.SAMP = 1;

    WAIT_FOR_ADC;

    AD1CHS = ad1chs_save;
    return _g_v_per_ads * ( float ) ADC1BUF0;
}

extern double adc_s_get_av( void )
{
    return _g_v_per_ads * ( float ) _g_adc_reading_avg;
}
