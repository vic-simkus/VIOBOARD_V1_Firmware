#define __SPI_H_INT
#include "spi.h"
#include "chip.h"

void spi_init(void)
{
	SPI_SS_TRIS = 0;		//	Slave Select - output
	SPI_SS = 1;				//	SS - hight
	
	SPI1CON1bits.MSTEN = 1;		//	Master mode
	SPI1CON2bits.SPIBEN = 0;	//	Disable enhanced mode
	SPI1CON1bits.CKP = 0;		//	SPI mode 0,0 = 0; 1
	SPI1CON1bits.CKE = 1;		//	SPI mode 1,1 = is 1; 0
	
	SPI1CON1bits.PPRE = 0b11;	//	Primary prescaler 1:1
	SPI1CON1bits.SPRE = 0b110;	//	Secondary prescaler 2:1
								//	With a 16Mhz FCY this gives us a clock of 8Mhz
									
	SPI1STATbits.SPIROV = 0;	//	Clear overflow flag
	SPI1STATbits.SPIEN = 1;		//	Enable SPI module
	
	return;
}	

void spi_communicate(UCHAR * _data_out,UCHAR * _data_in,UINT _count)
{
	int i = 0;
	int rx_buffer_idx = 0;
	
	SPI_SS = 0;		//	Set the SS line low indicating that we're ready to do stuff
	
	for(i=0;i<_count;i++)
	{
		while(SPI1STATbits.SPITBF == 1)
		{
			/*
			Wait for data to be transferred from TX buffer to SR (shift register)
			Realistically we should never have to spin
			*/
			Nop();
		}
		
		SPI1BUF = _data_out[i];		//	Write data to the TX buffer.
		
		while(SPI1STATbits.SPIRBF == 0)
		{
			/*
			Wait for the data to be loaded from the SR to the RX buffer.
			*/
			Nop();		
		}	
		
		_data_in[rx_buffer_idx] = SPI1BUF;	//	Read data from the RX buffer
		rx_buffer_idx = rx_buffer_idx + 1;			
	}	
	
	SPI_SS = 1;		// We're done, pull the line back high.
	return;
}		
