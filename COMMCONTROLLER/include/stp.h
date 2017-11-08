#ifndef __STP_H
#define __STP_H

#ifdef __STP_H_INT
	#define __func_def
#else
	#define __func_def extern
#endif

__func_def void stp_init(void);
__func_def void stp_send_byte(unsigned char _byte);

#undef __func_def

#endif
