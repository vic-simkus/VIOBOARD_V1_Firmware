#ifndef __LCD_H
#define __LCD_H

									
/*
	Various constants.
*/								

#define LCD_RS_INSTR		0					// Register select - instruction
#define LCD_RS_DATA			1					// Register select - data

/*
	Note:  I have no clue where these values come from... 
	I found some page on the internet that had the values listes as 0x0,0x40,0x20,0x60, but that wasn't right.
	Arrived at these values by trial and error
*/
#define LCD_LINE_1			0x00				// Starting DD (display data [ram]) address for line 1
#define LCD_LINE_2			0x40				// Starting DD address for line 2
#define LCD_LINE_3			0x14				// Starting DD address for line 3
#define LCD_LINE_4			0x54				// Starting DD address for line 4

/*
	Command constants
	
	The format is:
	LCD_CMD_XX			- command constant
	LCD_CMD_XX_YY_ZZ	- command parameters.  Should be ORed with the command constant
							i.e: LCD_CMD_XX | LCD_CMD_XX_YY_ZZ | (and so forth)
*/

/*
	For the following commands RS should be INSTRUCTION and RW should be WRITE.
*/
#define LCD_CMD_CLEAR				0x01				// Clears entire display.  Sets DD address to 0x0.

#define LCD_CMD_HOME				0x02				// Sets the DD address to 0x0

#define LCD_CMD_ENTRY_MODE			0x04				// Specifies direction of either 
#define LCD_CMD_ENTRY_MODE_INC		0x02				// Increments AC (Address Counter)
#define LCD_CMD_ENTRY_MODE_DEC		0x00				// Decrements AC.  Stuff written will be displayed right-to-left
#define LCD_CMD_ENTRY_MODE_CURS		0x00				// Moves the cursor in defined direction
#define LCD_CMD_ENTRY_MODE_DISP		0x01				// Scrolls the display in defined direction

#define LCD_CMD_DISP_ON				0x08				// Turns on/off the display and sets cursor options
#define LCD_CMD_DISP_ON_ON			0x04				// Display should be on
#define LCD_CMD_DISP_ON_OFF			0x00				// Display should be off
#define LCD_CMD_DISP_ON_CURS_ON		0x02				// Cursor should be on
#define LCD_CMD_DISP_ON_CURS_OFF	0x00				// Cursor should be off
#define LCD_CMD_DISP_ON_CURS_BLNK	0x01				// Cursor should blink
#define LCD_CMD_DISP_ON_CURS_NOBLNK	0x00				// Cursor should not blink

#define LCD_CMD_SHIFT				0x10				// Shift display/move cursor
#define LCD_CMD_SHIFT_DISP			0x08				// Shift display
#define LCD_CMD_SHIFT_CURS			0x00				// Move cursor
#define LCD_CMD_SHIFT_RIGHT			0x04				// Move/shift right
#define LCD_CMD_SHIFT_LEFT			0x00				// Move/shift left

#define LCD_CMD_FUNC_SET			0x20				// Set interface options
#define LCD_CMD_FUNC_SET_8BIT		0x10				// 8 bit interface
#define LCD_CMD_FUNC_SET_4BIT		0x00				// 4 bit interface
#define LCD_CMD_FUNC_SET_2LINE		0x08				// 2 lines
#define LCD_CMD_FUNC_SET_1LINE		0x00				// 1 line
#define LCD_CMD_FUNC_SET_FNT5x10	0x04				// 5x10 font
#define LCD_CMD_FUNC_SET_FNT5x8		0x00				// 5x8 font

#define LCD_CMD_SET_CG_ADDR			0x40				// Sets CG RAM address.  Lower 6 bits are the address
#define LCD_CMD_SET_DD_ADDR			0x80				// Sets DD RAM address.  Lower 7 bits are the address

#define LCD_COL_COUNT				20
#define LCD_LINE_COUNT				4
#define LCD_BUFFER_SIZE				LCD_LINE_COUNT * LCD_COL_COUNT

#ifdef __LCD_H_INT

#define __func_def

#else
#define __func_def extern


#endif


/**
	Initializes the LCD output system.
	\param _iu Immediate output - if set to 1 all changes to the output buffer will be immediately output to LCD
*/
__func_def void lcd_init(char _iu);

/**
	Clears the display.
*/
__func_def void lcd_clear(void);

/**
	Displays data at the specified line.
*/
__func_def void lcd_write_string(char _line,char _col,char * _data);

/**
	Shifts display up one line
*/
__func_def void lcd_shift_display_up(void);

/**
	Shifts the display down one line.
*/
__func_def void lcd_shift_display_down(void);


/**
	Prints a string to the display.  Shifts the display up as needed.
*/
__func_def void lcd_print_string(char * _line);

/**
	Gets total number of lines printed since last call to lcd_clear;
*/
__func_def int lcd_get_lines_printed(void);

/**
	Shifts the contents of the display specified number of spaces.
*/
__func_def void lcd_shift_display_left(char _amount);

/**
	Shifts the contents of the display specified number of spaces.
*/
__func_def void lcd_shift_display_right(char _amount);

/**
	Shifts the contents of the specified line the specified number of spaces.
*/
__func_def void lcd_shift_line_left(char _line,char _amount);

/**
	Shifts the contents of the specified line the specified number of spaces.
*/
__func_def void lcd_shift_line_right(char _line,char _amount);

__func_def void lcd_demo_1(void);

#undef __func_def

#endif



