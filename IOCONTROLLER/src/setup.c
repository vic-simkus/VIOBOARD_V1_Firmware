/*
Vic's IO Board V1.0 Copyright (C) 2017 Vidas Simkus

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "setup.h"
#include "config.h"
#include "ioconfig.h"
#include "i2c_callbacks.h"

#include "I2C/i2c.h"
#include "I2C/simkus_net.h"

#include <xc.h>
#include <libpic30.h>

void setup_adc(void)
{

	/*
	 * Configure the ADC parameters
	 */
	ADCON1bits.FORM = 0;		// unsigned integer format
	ADCON1bits.SSRC = 0b111;	// Conversion trigger source - internal counter

	ADCON2bits.VCFG = 0b001;	// use VRef+ pin for VrefH and and AVss for VrefL
	ADCON2bits.CSCNA = 1;		// scan inputs
	ADCON2bits.SMPI = 8;		// interrupt every 8 sequences
	ADCON2bits.ALTS = 0;		// Always use MUX A input settings
	ADCON2bits.BUFM = 0;		// buffer configures as one 16 word buffer

	ADCON3bits.SAMC = 0x0F;	// Auto sample time.  Number of Tad cycles the ADC will spend sampling before starting conversion
	ADCON3bits.ADRC = 1;	// internal rc clock


	TRISB = 0;
	ADPCFG = 0;
	ADPCFG = 1;

	/*
	 * Set pins as analog inputs
	 */
	TRISBbits.TRISB1 = 1;	//ICD3 -- T7
	TRISBbits.TRISB2 = 1;	//ICD4 -- T8
	TRISBbits.TRISB3 = 1;	//420_4 -- T4
	TRISBbits.TRISB4 = 1;	//420_3 -- T3
	TRISBbits.TRISB5 = 1;	//420_2 -- T2
	TRISBbits.TRISB8 = 1;	//420_1 -- T1
	TRISBbits.TRISB9 = 1;	//ICD1 -- T5
	TRISBbits.TRISB10 = 1;	//ICD2 -- T6

	/*
	 * Set which pins to auto scan
	 */
	ADCSSL = TRISB;

	TRISBbits.TRISB0 = 1;	// VrefH reference

	/*
	 * Configure ports as ADC
	 */

	ADPCFGbits.PCFG0 = 0;
	ADPCFGbits.PCFG1 = 0;
	ADPCFGbits.PCFG2 = 0;
	ADPCFGbits.PCFG3 = 0;
	ADPCFGbits.PCFG4 = 0;
	ADPCFGbits.PCFG5 = 0;
	ADPCFGbits.PCFG8 = 0;
	ADPCFGbits.PCFG9 = 0;
	ADPCFGbits.PCFG10 = 0;

	ADCON1bits.ASAM = 1;	// automatically start after last conversion

	ADCON1bits.ADON = 1;	// enable ADC module

	IFS0bits.ADIF = 0;		// clear flag
	IPC2bits.ADIP = IP_ADC;	// interrupt priority.
	IEC0bits.ADIE = 1;		// enable ADC interrupts.

	return;

}

void setup_i2c_custom_registers(void)
{
	/*
	 * Setup the custom registers provided by IOCONTROLLER
	 * - Get analog input status
	 * - Get digital output status
	 * - Set digital output status
	 */


	/*
	 * Setup get analog input status callback
	 * Register: 0x07
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 1;
	i2c_registers.register_list[i2c_registers.count].cb_write = 0;
	i2c_registers.register_list[i2c_registers.count].cb_read = i2c_cb_get_ai_status;
	i2c_registers.count += 1;

	/*
	 * Setup get digital output status callback
	 * Register: 0x08
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 1;
	i2c_registers.register_list[i2c_registers.count].cb_write = 0;
	i2c_registers.register_list[i2c_registers.count].cb_read = i2c_cb_get_do_status;
	i2c_registers.count += 1;

	/*
	 * Setup set digital output status callback
	 * Register: 0x09
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 0;
	i2c_registers.register_list[i2c_registers.count].cb_write = i2c_cb_set_do_status;
	i2c_registers.register_list[i2c_registers.count].cb_read = 0;
	i2c_registers.count += 1;

	/*
	 * Setup PMIC output status callback
	 * Register: 0x0A
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 1;
	i2c_registers.register_list[i2c_registers.count].cb_read = i2c_cb_get_pmic_status;
	i2c_registers.register_list[i2c_registers.count].cb_write = 0;
	i2c_registers.count += 1;

	/*
	 * Setup PMIC configuration callback
	 * Register: 0x0B
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 0;
	i2c_registers.register_list[i2c_registers.count].cb_read = 0;
	i2c_registers.register_list[i2c_registers.count].cb_write = i2c_cb_set_pmic_status;
	i2c_registers.count += 1;

	/*
	 * Setup L1 calibration getter callback
	 * Register: 0x0C
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 1;
	i2c_registers.register_list[i2c_registers.count].cb_read = i2c_cb_get_l1_cal;
	i2c_registers.register_list[i2c_registers.count].cb_write = 0;
	i2c_registers.count += 1;

	/*
	 * Setup L2 calibration getter callback
	 * Register: 0x0D
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 1;
	i2c_registers.register_list[i2c_registers.count].cb_read = i2c_cb_get_l2_cal;
	i2c_registers.register_list[i2c_registers.count].cb_write = 0;
	i2c_registers.count += 1;

	/*
	 * Setup L1 calibration setter callback
	 * Register: 0x0E
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 0;
	i2c_registers.register_list[i2c_registers.count].cb_read = 0;
	i2c_registers.register_list[i2c_registers.count].cb_write = i2c_cb_set_l1_cal;
	i2c_registers.count += 1;

	/*
	 * Setup L2 calibration setter callback
	 * Register: 0x0F
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 0;
	i2c_registers.register_list[i2c_registers.count].cb_read = 0;
	i2c_registers.register_list[i2c_registers.count].cb_write = i2c_cb_set_l2_cal;
	i2c_registers.count += 1;


	/*
	 * Setup boot counter getter callback
	 * Register: 0x10
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 1;
	i2c_registers.register_list[i2c_registers.count].cb_read = i2c_cb_get_boot_count;
	i2c_registers.register_list[i2c_registers.count].cb_write = 0;
	i2c_registers.count += 1;

	/*
	 * Setup boot counter getter callback
	 * Register: 0x11
	 */
	i2c_registers.register_list[i2c_registers.count].ro = 0;
	i2c_registers.register_list[i2c_registers.count].cb_read = 0;
	i2c_registers.register_list[i2c_registers.count].cb_write = i2c_cb_set_output_confirm;
	i2c_registers.count += 1;

	return;

}

void setup_i2c(void)
{
	/*
	 * This is a workaround the dsPIC30 broken i2c module.  This needs to be here.
	 *
	 * We enable a higher priority module on the I2C pins so that when
	 * the I2C module is started it doesn't cause a glitch on the bus.
	 */
	SPI1STATbits.SPIEN = 1;

	/*
	 * The I2CBRG value is based on a 10MHz crystal with a 4xPLL which results
	 * in an Fcy of 10MHz.  Based on the calcs in the docs 90 is the required
	 * baud rate generator value for a 100kHz bus.
	 *
	 * With fancy LED startup the delay is approx 450mS + 10mS for the bus
	 * unfucking delay.  For a grand total of 450mS.  Let's round it up to 500mS
	 */

	i2c_setup_default_registers();
	setup_i2c_custom_registers();

	i2c_init(IP_I2C, I2C_ADDR_IO_CTRL, 90);
	//2c_init(IP_I2C, I2C_ADDR_IO_CTRL, 0x00F);

	sn_init(1000, 10);

	/*
	 * This is a workaround the dsPIC30 broken i2c module.  This needs to be here.
	 * See above.
	 */
	SPI1STATbits.SPIEN = 0;


}

void setup_pmics(void)
{
	PWR_ENABLE_AI_CONFIG = 0;		// Enable output for analog sensors PMIC
	PWR_ENABLE_DO_CONFIG = 0;		// Enable output for digital output PMIC

	PWR_FAULT_AI_CONFIG  = 1;		// Fault input for analog sensors PMIC
	PWR_FAULT_DO_CONFIG  = 1;		// Fault input for digital output PMIC

	PWR_FAULT_AI_LED_CONFIG = 0;	// AI PMIC fault LED
	PWR_FAULT_DO_LED_CONFIG = 0;	// DO PMIC fault LED

	PWR_ENABLE_AI = 1;
	PWR_ENABLE_DO = 1;

	PWR_FAULT_AI_LED = 0;
	PWR_FAULT_DO_LED = 0;

	return;
}

void setup_status_led_pins()
{
	/*
	 * Primary status LED -- the blue LED on RE0
	 */
	//TRISEbits.TRISE0 = 0;


	TRISDbits.TRISD8 = 0;	// heartbeat LED
	TRISDbits.TRISD2 = 0;	// bus out LED
	TRISDbits.TRISD9 = 0;	// bus in LED
	TRISCbits.TRISC14 = 0;	// aux LED

	/*
	 * Misc status LEDs because LEDs are cool.
	 */

	BUS_OUT_LED = 0;
	BUS_IN_LED = 0;
	AUX_LED = 0;
	HEARTBEAT_LED = 0;

}

void setup_do_pins(void)
{
	/*
	 * Setup digital output pins
	 */

	D01_CONFIG = 0;		//output
	D02_CONFIG = 0;		//output
	D03_CONFIG = 0;		//output
	D04_CONFIG = 0;		//output

	D01 = 0;
	D02 = 0;
	D03 = 0;
	D04 = 0;

	return;
}

void setup_T1()
{
	/*
	 * TxCON - timer configuration bits
	 * TMRx - timer counter register
	 * PRx - timer period
	 *
	 * TxIE - interrupt enable
	 * TxIP - interrupt priority
	 */


	IFS0bits.T1IF = 0; // Clear T1 interrupt
	IPC0bits.T1IP = IP_T1;


	T1CONbits.TCKPS = 0b11; // 1:256 prescaler
	PR1 = 19531; // Set period

	T1CONbits.TCS = 0; // Internal clock (Fosc/4 = 10MHz)
	IEC0bits.T1IE = 1; // Enable T1 interrupt

	T1CONbits.TON = 1; // start timer
	return;
}
