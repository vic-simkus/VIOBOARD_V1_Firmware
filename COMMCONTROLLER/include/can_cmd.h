#ifndef __CAN_CMD_H
#define __CAN_CMD_H
#include "data_types.h"

#ifndef CAN_BUFFER_SIZE
	#define CAN_BUFFER_SIZE 16
#endif

#define CAN_CMD_RESET		0b11000000
#define CAN_CMD_READ		0b00000011
#define CAN_CMD_WRITE		0b00000010
#define CAN_CMD_READ_STAT	0b10100000
#define CAN_CMD_RX_STATUS	0b10110000
#define CAN_CMD_BIT_MOD	0b00000101

#define CAN_R_CANSTAT	0b00001110
#define CAN_R_CANCTRL	0b00001111

#define CAN_FILLER		0xAA

extern void can_reset(void);
extern UCHAR can_read_register(UCHAR _register);
#endif
