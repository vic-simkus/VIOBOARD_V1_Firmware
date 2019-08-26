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
    AD1CON1bits.ASAM = 1; // Sampling starts when SAMP bit is manually set
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

    AD1CON2bits.SMPI = 7; // Interrupt every eigth
    IFS0bits.AD1IF = 0; // Reset AD interrupt flag
    IPC3bits.AD1IP = 7; // Highest priority
    IEC0bits.AD1IE = 1; // Enable AD interrupts
    
    AD1CON1bits.ADON = 1; // Enable the AD module

    return;

}
