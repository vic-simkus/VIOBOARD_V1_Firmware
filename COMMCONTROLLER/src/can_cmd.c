#include "can_cmd.h"
#include "spi.h"
#include "chip.h"

UCHAR com_buffer[CAN_BUFFER_SIZE];

void can_reset(void)
{
	com_buffer[0] = CAN_CMD_RESET;
	spi_communicate(com_buffer,com_buffer,1);
	
	return;
}	

UCHAR can_read_register(UCHAR _register)
{
	com_buffer[0] = CAN_CMD_READ;
	com_buffer[1] = _register;
	com_buffer[2] = CAN_FILLER;
	
	spi_communicate(com_buffer,com_buffer,3);
	
	return com_buffer[2];
}	

