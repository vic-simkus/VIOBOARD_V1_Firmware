#include "config.h"
#include "support.h"

#define __STP_H_INT
#include "stp.h"

#include "chip.h"
#include <libpic30.h>

void stp_init(void)
{
	STP_DATA = 0;
	STP_CLK = 0;
	STP_STRB = 0;
	
	return;
}


#define SEND_BYTE STP_CLK = 0; ROTL_B(_byte); STP_DATA = _byte & 1; STP_CLK = 1
void stp_send_byte(unsigned char _byte)
{
	STP_STRB = 1;	
	SEND_BYTE;
	SEND_BYTE;
	SEND_BYTE;
	SEND_BYTE;
	SEND_BYTE;
	SEND_BYTE;
	SEND_BYTE;
	SEND_BYTE;	
	STP_STRB = 0;
	STP_CLK = 0;
	STP_DATA = 0;
	
	return;
}	
