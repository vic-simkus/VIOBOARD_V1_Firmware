#include "I2C.h"
#include <xc.h>

#include <string.h>

void i2c_init( unsigned char _isr_priority, unsigned char _slave_address )
{
    __linker_fixup_i2c_slave_isr( );
    
    memset((void *)&eeprom_data,0,sizeof(eeprom_data));
    
    
    eeprom_data.header[0] = 0xAA;
    eeprom_data.header[1] = 0x55;
    eeprom_data.header[2] = 0x33;
    eeprom_data.header[3] = 0xEE;
    
    eeprom_data.eeprom_rev[0] = 'A';
    eeprom_data.eeprom_rev[1] = '1';
    
    strcpy((char *)eeprom_data.board_name,"VPSB");	// So the bootloader tries to load /lib/firmware/board_name-version.dtbo automatically
    strcpy((char*)eeprom_data.version,"V1.0");		// Kind of neat.
    strcpy((char*)eeprom_data.mfg,"Vic Simkus");
    strcpy((char*)eeprom_data.pn,"VPSB");
    eeprom_data.pin_count = 19;
    strcpy((char*)eeprom_data.serial,"SERIAL");
    
    eeprom_data.i_dc = 2000;
    
    eeprom_data.pin_usage[EPC_P8_33] = EPC_PIN_USED | EPC_RXACTIVE | EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_6;	//U4RTSN
    eeprom_data.pin_usage[EPC_P8_35] = EPC_PIN_USED | EPC_RXACTIVE | EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_6;	//U4CTSN
    eeprom_data.pin_usage[EPC_P8_37] = EPC_PIN_USED | EPC_RXACTIVE | EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_6;	//U2CTSN
    eeprom_data.pin_usage[EPC_P8_38] = EPC_PIN_USED | EPC_RXACTIVE | EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_6;	//U2RTSN
    eeprom_data.pin_usage[EPC_P8_39] = EPC_PIN_USED |  EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_7;			//AUX2
    eeprom_data.pin_usage[EPC_P8_40] = EPC_PIN_USED |  EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_7;			//ENABLE    
    
    eeprom_data.pin_usage[EPC_P9_11] = EPC_PIN_USED | EPC_RXACTIVE | EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_6;	//U4RXD
    eeprom_data.pin_usage[EPC_P9_13] = EPC_PIN_USED | EPC_RXACTIVE | EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_6;	//U4TXD
    eeprom_data.pin_usage[EPC_P9_19] = EPC_PIN_USED | EPC_RXACTIVE | EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_3;	//SCL1
    eeprom_data.pin_usage[EPC_P9_20] = EPC_PIN_USED | EPC_RXACTIVE | EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_3;	//SDA1
    eeprom_data.pin_usage[EPC_P9_21] = EPC_PIN_USED | EPC_RXACTIVE | EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_1;	//U2TXD
    eeprom_data.pin_usage[EPC_P9_22] = EPC_PIN_USED | EPC_RXACTIVE | EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_1;	//U2RXD
    eeprom_data.pin_usage[EPC_P9_27] = EPC_PIN_USED |  EPC_PULLTYPESEL | EPC_PULLUDEN | EPC_MUX_7;			//AUX1    
    

    I2CCONbits.STREN = 1; //	Enable clock stretching
    I2CCONbits.A10M = 0; //	Disable 10 bit addresses

    I2CADD = _slave_address;

    SLAVE_IPC = _isr_priority;
    SLAVE_IEC = 1; // Enable slave interrupts

    SLAVE_IF = 0;

    I2CCONbits.I2CEN = 1; //	Enable I2C module

    //I2CCONbits.PEN = 1;

    return;
}