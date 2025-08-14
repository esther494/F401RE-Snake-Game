/*
 * i2c-lcd.h
 *
 *  Created on: Jul 15, 2024
 *      Author: esthe
 */

#ifndef SRC_I2C_LCD_H_
#define SRC_I2C_LCD_H_

#include "stm32f4xx_hal.h"

void lcd_send_cmd (char cmd);
void lcd_send_data (char data);
void lcd_init (void);
void lcd_cursor_pos (int row, int col);
void lcd_send_string (char *str);
void lcd_clear_display (void);
void lcd_return_home (void);
void lcd_entry_mode (int dir, int s);
void lcd_display_on_off (int d, int c, int b);
void lcd_function_set (int dl, int n, int f);

#endif /* SRC_I2C_LCD_H_ */
