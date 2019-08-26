/* 
 * File:   I2C.h
 * Author: vic
 *
 * Created on August 22, 2019, 7:29 PM
 */

#ifndef I2C_H
#define	I2C_H

#include "globals.h"


#define I2CCON		I2C1CON
#define I2CSTAT		I2C1STAT
#define I2CCONbits	I2C1CONbits
#define I2CSTATbits	I2C1STATbits

#define I2CRCV		I2C1RCV
#define I2CTRN		I2C1TRN
#define I2CADD		I2C1ADD

#define SLAVE_IPC	_SI2C1P
#define	MASTER_IPC	_MI2C1P

#define	SLAVE_IEC	_SI2C1IE
#define MASTER_IEC	_MI2C1IE

#define	SLAVE_IF	_SI2C1IF
#define MASTER_IF	_MI2C1IF

#define I2CBRG 		I2C1BRG

#define I2CADD		I2C1ADD
#define I2CMSK		I2C1MSK


/**
\defgroup BUS_RW Bus direction
These constants define the direction that the bus will be operating in within the context of the two operating nodes.
 */

/**
\ingroup BUS_RW
I2C Master will be writing to slave
 */
#define I2C_RW_W	0

/**
\ingroup BUS_RW
I2C Master will be reading from slave
 */
#define I2C_RW_R	1

/**
\ingroup BUS_DA
 * Last byte was address (compared against I2CSTATbits.D_NOT_A )
 */
#define I2C_DA_A	0

/**
\ingroup BUS_DA
 * Last byte was data (compared against I2CSTATbits.D_NOT_A )
 */
#define I2C_DA_D	1

/// Wait for the transmit  buffer to become empty. 1 == transmit in progress, 0 == transmit complete
#define WAIT_FOR_TBF() 	while(I2CSTATbits.TBF == 1){Nop();}

extern void __linker_fixup_i2c_slave_isr( void );
extern void i2c_init( unsigned char _isr_priority, unsigned char _slave_address );

typedef struct
{
    unsigned char header[4];		// offset 0
    unsigned char eeprom_rev[2];	// offset 4
    unsigned char board_name[32];	// offset 6
    unsigned char version[4];		// offset 38
    unsigned char mfg[16];		// offset 42
    unsigned char pn[16];		// offset 58
    uint16_t pin_count;		// offset 74 ( 2 bytes))
    unsigned char serial[12];		// offset 76
    uint16_t pin_usage[74];		// offset 88 (2 bytes each)
    uint16_t i_3v3;			// offset 236 ( 2 bytes))
    uint16_t i_5v0;			// offset 238 ( 2 bytes))
    uint16_t i_dc;			// offset 240 ( 2 bytes))
} FAKE_EEPROM;

extern volatile FAKE_EEPROM eeprom_data;

/**
 * Configuration flags for the pin usage field in the eeprom
 * 
 * These are in big endian (ofcourse) so bit positions are 0-15 rather than the usual 15-0
 * 
 * EPC = EEPROM Pin Configuration
 */

/**
 * Pin is used - 15th bit is one
 */
#define EPC_PIN_USED		0b0000000000000001

/**
 * Slew control.  7th bit = 1 == Fast slew
 */
#define EPC_SLEWCTRL		0b0000001000000000

/**
 * RX control - 5th bit =1 == RX enabled
 * According to the AM335x tech reference manual, Set to 0 for output only.  1 for input or output.
 * Section 9.22, page 1446, table 9-1
 */
#define EPC_RXACTIVE		0b0000010000000000

/**
 * Type of pull up/pull down resistor.  4th bit = 1 == pullup
 */
#define EPC_PULLTYPESEL		0b0000100000000000

/**
 * Is pull up/pull down resistor enabled.  3rd bit =1 == 1 pullups are disabled.  If this is 1, EPC_PU_PD is irrelevant.
 */
#define EPC_PULLUDEN		0b0001000000000000


#define EPC_MUX_0			0b0000000000000000
#define EPC_MUX_1		   	0b1000000000000000
#define EPC_MUX_2			0b0100000000000000
#define EPC_MUX_3			0b1100000000000000
#define EPC_MUX_4			0b0010000000000000
#define EPC_MUX_5			0b1010000000000000
#define EPC_MUX_6			0b0110000000000000
#define EPC_MUX_7			0b1110000000000000

#define EPC_P9_22			0
#define EPC_P9_21			1
#define EPC_P9_18			2
#define EPC_P9_17			3
#define EPC_P9_42			4
#define EPC_P8_35			5
#define EPC_P8_33			6
#define EPC_P8_31			7
#define EPC_P8_32			8
#define EPC_P9_19			9
#define EPC_P9_20			10
#define EPC_P9_26			11
#define EPC_P9_24			12
#define EPC_P9_41			13
#define EPC_P8_19			14
#define EPC_P8_13			15
#define EPC_P8_14			16
#define EPC_P8_17			17
#define EPC_P9_11			18
#define EPC_P9_13			19
#define EPC_P8_25			20
#define EPC_P8_24			21
#define EPC_P8_05			22
#define EPC_P8_06			23
#define EPC_P8_23			24
#define EPC_P8_22			25
#define EPC_P8_03			26
#define EPC_P8_04			27
#define EPC_P8_12   		28
#define EPC_P8_11			29
#define EPC_P8_16			30
#define EPC_P6_15			31
#define EPC_P9_15			32
#define EPC_P9_23			33
#define EPC_P9_14			34
#define EPC_P9_16			35
#define EPC_P9_12			36
#define EPC_P8_26			37
#define EPC_P8_21			38
#define EPC_P8_20			39
#define EPC_P8_18			40
#define EPC_P8_07			41
#define EPC_P8_09			42
#define EPC_P8_10			43
#define EPC_P8_08			44
#define EPC_P8_45			45
#define EPC_P8_46			46
#define EPC_P8_43			47
#define EPC_P8_44			48
#define EPC_P8_41			49
#define EPC_P8_42			50
#define EPC_P8_39			51
#define EPC_P8_40			52
#define EPC_P8_37			53
#define EPC_P8_38			54
#define EPC_P8_36			55
#define EPC_P8_34			56
#define EPC_P8_27			57
#define EPC_P8_29			58
#define EPC_P8_28			59
#define EPC_P8_30			60
#define EPC_P9_29			61
#define EPC_P9_30			62
#define EPC_P9_28			63
#define EPC_P9_27			64
#define EPC_P9_31			65
#define EPC_P9_25			66
#define EPC_P9_39			67
#define EPC_P9_40			68
#define EPC_P9_37			69
#define EPC_P9_38			70
#define EPC_P9_33			71
#define EPC_P9_36			72
#define EPC_P9_35			73


#endif	/* I2C_H */