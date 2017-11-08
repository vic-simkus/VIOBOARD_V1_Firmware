#include "config.h"

#define __LCD_H_INT
#include "lcd.h"
#include "stp.h"

#include "chip.h"
#include <libpic30.h>	// for __delay_* functions
#include <string.h>
#include <stdio.h>

#define BUFF_OFFSET(_line,_col) ((20 * _line) + _col)
#define LCD_TOGGLE_STROBE() 		LCD_E = 1;__delay_us(100);LCD_E = 0		//This is 10 time more than the spec.  See bellow

#define LCD_DATA_DELAY_US	40			// These are more than the spec.
#define LCD_COMMAND_DELAY_S_US 40		// My cheap Chinese knock-off LCD sucks balls apperently.
#define LCD_COMMAND_DELAY_L_US	3000	// This is twice as big as the spec. 

char __line_0_buffer[LCD_COL_COUNT];
char __line_1_buffer[LCD_COL_COUNT];
char __line_2_buffer[LCD_COL_COUNT];
char __line_3_buffer[LCD_COL_COUNT];
char * __lcd_buffer;

char __immediate_update = 0;
char __current_line = 0;
char __lock = 0;
int __lines_printed = 0;

void __hard_init_lcd(void);
void __lcd_send_command(char);
void __turn_on_lcd_power(void);
void __lcd_turn_on(char);
void __reset_buffer(void);
void __lcd_set_cursor_addr(char _line_addr,char _cursor);
void __lcd_set_cursor(char _line,char _cursor);
void __copy_buffer_to_lcd(void);
void __lcd_write_byte(char);

void lcd_init(char _iu)
{
	stp_init();
	__immediate_update = _iu;	
	__lcd_buffer = __line_0_buffer;
	
	/*
	ADPCFG = 1;

		
	LCD_DATA_TRIS_B0 = 0;
	LCD_DATA_TRIS_B1 = 0;
	LCD_DATA_TRIS_B2 = 0;
	LCD_DATA_TRIS_B3 = 0;
	LCD_DATA_TRIS_B4 = 0;
	LCD_DATA_TRIS_B5 = 0;
	LCD_DATA_TRIS_B6 = 0;
	LCD_DATA_TRIS_B7 = 0;
	
	LCD_CTRL_TRIS_B0 = 0;
	LCD_CTRL_TRIS_B1 = 0;
	LCD_CTRL_TRIS_B4 = 0;
	*/
		
	__hard_init_lcd();
	
	__lcd_send_command(LCD_CMD_FUNC_SET | LCD_CMD_FUNC_SET_8BIT | LCD_CMD_FUNC_SET_2LINE | LCD_CMD_FUNC_SET_FNT5x8);	
	__lcd_send_command(LCD_CMD_ENTRY_MODE | LCD_CMD_ENTRY_MODE_INC | LCD_CMD_ENTRY_MODE_CURS);	
	__lcd_turn_on(LCD_CMD_DISP_ON_ON | LCD_CMD_DISP_ON_CURS_OFF);
	
	lcd_clear();	
	
	return;
}

void lcd_clear(void)
{
	__lcd_send_command(LCD_CMD_CLEAR);
	__reset_buffer();
	__current_line = 0;
	__lines_printed = 0;
	
	return;
}	

void lcd_shift_line_left(char _line,char _amount)
{
	int i=0;
	
	for(i=0;i<LCD_COL_COUNT - _amount;i++)
	{
		__lcd_buffer[BUFF_OFFSET(_line,i)] = __lcd_buffer[ BUFF_OFFSET(_line,_amount + i)];	
	}	
	
	for(i=0;i<_amount;i++)
	{
		__lcd_buffer[BUFF_OFFSET(_line,(LCD_COL_COUNT - 1) - i)] = 0x20;
	}
		
	if(__immediate_update)
	{
		__copy_buffer_to_lcd();
	}
		
	return;
}	

void lcd_shift_line_right(char _line,char _amount)
{
	memmove(__lcd_buffer + BUFF_OFFSET(_line,0) + _amount,__lcd_buffer + BUFF_OFFSET(_line,0),LCD_COL_COUNT - _amount);
	memset(__lcd_buffer + BUFF_OFFSET(_line,0),0x20,_amount);	
	
	if(__immediate_update)
	{
		__copy_buffer_to_lcd();
	}
	
	return;
}

void lcd_shift_display_left(char _amount)
{
	lcd_shift_line_left(0,_amount);
	lcd_shift_line_left(1,_amount);
	lcd_shift_line_left(2,_amount);
	lcd_shift_line_left(3,_amount);
	
	if(__immediate_update)
	{
		__copy_buffer_to_lcd();
	}
	
	return;
}	

void lcd_shift_display_right(char _amount)
{
	lcd_shift_line_right(0,_amount);
	lcd_shift_line_right(1,_amount);
	lcd_shift_line_right(2,_amount);
	lcd_shift_line_right(3,_amount);
	
	if(__immediate_update)
	{
		__copy_buffer_to_lcd();
	}	
	
	return;
}	

void lcd_shift_display_up(void)
{
	memmove(__line_0_buffer,__line_1_buffer,LCD_COL_COUNT);
	memmove(__line_1_buffer,__line_2_buffer,LCD_COL_COUNT);
	memmove(__line_2_buffer,__line_3_buffer,LCD_COL_COUNT);
	memset(__line_3_buffer,0x20,LCD_COL_COUNT);
	
	if(__immediate_update)
	{
		__copy_buffer_to_lcd();
	}	
	
	return;
}

void lcd_shift_display_down(void)
{
	memmove(__line_3_buffer,__line_2_buffer,LCD_COL_COUNT);
	memmove(__line_2_buffer,__line_1_buffer,LCD_COL_COUNT);
	memmove(__line_1_buffer,__line_0_buffer,LCD_COL_COUNT);
	memset(__line_0_buffer,0x20,LCD_COL_COUNT);
		
	if(__immediate_update)
	{
		__copy_buffer_to_lcd();
	}
	
	return;
}

void lcd_write_string(char _line,char _col,char * _data)
{
	int i = 0;
	int c = 0;
	int si = 0;
	
	for(i=0;i<LCD_COL_COUNT;i++)
	{
		__lcd_buffer[BUFF_OFFSET(_line,i)] = 0x20;
	}	
	for(i = 0;i<(LCD_COL_COUNT - (_col));i++)
	{
		c = _data[si];
		si += 1;
		
		if(c == 0)
		{
			break;
		}
			
		__lcd_buffer[BUFF_OFFSET(_line,(_col) + i)] = c;
	}	
	
	if(__immediate_update)
	{
		__copy_buffer_to_lcd();
	}
		
	return;
}		

void lcd_print_string(char * _line)
{
	char siu = __immediate_update;	// Turn off immediate update so that all of the
	__immediate_update = 0;			// operations performed here are batched
	
	if(__current_line > 3)
	{
		__current_line = 3;
		lcd_shift_display_up();
	}		
	
	lcd_write_string(__current_line,0,_line);
	
	__immediate_update = siu;
	__current_line += 1;
	__lines_printed += 1;
	
	if(__immediate_update)
	{
		__copy_buffer_to_lcd();
	}	
	
	return;
}

int lcd_get_lines_printed(void)
{
	return __lines_printed;
}			

/*
	Internal, non-exported functions from here on.
*/
void __hard_init_lcd(void)
{
	__lcd_send_command(0x30);			//magic value from HD44780U documentation
	__delay_ms(5);
	
	__lcd_send_command(0x30);
	__delay_ms(5);
	
	__lcd_send_command(0x30);
	__delay_ms(5);
	
	return;	
}

void __reset_buffer(void)
{
	memset(__lcd_buffer,0x20,LCD_COL_COUNT * LCD_LINE_COUNT);
	
	return;
}	

void __copy_buffer_to_lcd(void)
{
	int i;
	int line=0;
	int curs=0;

	__lcd_send_command(LCD_CMD_ENTRY_MODE | LCD_CMD_ENTRY_MODE_INC | LCD_CMD_ENTRY_MODE_CURS);
	
	for(i = 0;i < LCD_BUFFER_SIZE;i++)
	{
		if(curs == 20)
		{
			curs = 0;
			line = line + 1;
			__lcd_set_cursor(line,0);
		}	
		__lcd_write_byte(__lcd_buffer[i]);
		curs = curs + 1;
	}	
	
	return;
}	

void __lcd_send_command(char _cmd)
{
	LCD_RS = LCD_RS_INSTR;
	
	stp_send_byte(_cmd);

	//__asm__ volatile ("MOV.B %[cmd],LATB" : [cmd] "=a" (_cmd) );
	
	LCD_TOGGLE_STROBE();
	
	if(_cmd < 4) 
	{
		/*
		Only two commands - display clear and cursor home require 2 ms.  
		All other commands need 40 uS
		*/
		__delay_us(LCD_COMMAND_DELAY_L_US);
	}
	else
	{
		__delay_us(LCD_COMMAND_DELAY_S_US);
	}		
	
	return;
}	

void __lcd_set_cursor_addr(char _line_addr,char _cursor)
{
	__lcd_send_command(LCD_CMD_SET_DD_ADDR | (_line_addr + _cursor));
	return;
}	

void __lcd_set_cursor(char _line,char _cursor)
{
	switch(_line)
	{
		case 0:
			__lcd_set_cursor_addr(LCD_LINE_1,_cursor);
			break;
		case 1:
			__lcd_set_cursor_addr(LCD_LINE_2,_cursor);
			break;
		case 2:
			__lcd_set_cursor_addr(LCD_LINE_3,_cursor);
			break;
		case 3:
			__lcd_set_cursor_addr(LCD_LINE_4,_cursor);
			break;
		default:
			__lcd_set_cursor_addr(LCD_LINE_1,_cursor);
			break;
	}	
	
	return;
}	

void __lcd_turn_on(char _flags)
{
	__lcd_send_command(LCD_CMD_DISP_ON | LCD_CMD_DISP_ON_ON | _flags);
	
	return;
}

void __lcd_turn_off(void)
{
	__lcd_send_command(LCD_CMD_DISP_ON | LCD_CMD_DISP_ON_OFF);
	
	return;
}	

void __lcd_write_byte(char _data)
{
	LCD_RS = LCD_RS_DATA;
	
	stp_send_byte(_data);
	LCD_TOGGLE_STROBE();

	__delay_us(LCD_DATA_DELAY_US);
	
	return;
}	

void lcd_demo_1(void)
{
	int i =0;	
	char buff[20];
	
	lcd_clear();	
	
	for(i=0;i<3;i++)
	{
		sprintf(buff,"%X Line",lcd_get_lines_printed());
		lcd_print_string(buff);
		//__delay_ms(32);
	}	
	
	//lcd_clear();

	return;
}	

