/*
 * i2c-lcd.c
 *
 *  Created on: Jul 15, 2024
 *      Author: esthe
 */
#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;
#define SLAVE_ADDRESS_LCD 0x4E // default address

// rs is 0 for commands
void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_init (void)
{
	// 4 bit initialization
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_cursor_pos (int row, int col) {
	uint8_t address;
	if (row == 0) {
		address = col;
	}
	else if (row == 1) {
		address = 0x40 + col;
	}
	else if (row == 2) {
		address = 0x14 + col;
	}
	else if (row == 3) {
		address = 0x54 + col;
	}
	else {
		return;
	}
	lcd_send_cmd(0x80 | address);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_clear_display (void) {
	lcd_send_cmd (0x80);
	for (int i = 0; i < 70; i++) {
		lcd_send_data(' ');
	}
}

void lcd_return_home (void) { // return the cursor to its default position
	lcd_send_cmd (0x02);
	HAL_Delay(1);
}

void lcd_entry_mode (int dir, int s) { // direction of DDRAM address, shift the display left or right
	lcd_send_cmd (0x04 | dir << 1 | s);
	HAL_Delay(1);
}

void lcd_display_on_off (int d, int c, int b) { // display on/off, cursor on/off (no cursor), cursor blink on/off
	lcd_send_cmd (0x08 | d << 2 | c << 1 | b);
	HAL_Delay(1);
}

void lcd_function_set (int dl, int n, int f) { // 8bit or 4bit, display lines, 5x8 or 5x11
	lcd_send_cmd (0x10 | dl << 2 | n << 1 | f);
	HAL_Delay(1);
}
