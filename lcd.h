/*
 * lcd.h
 *
 * Created: 4/14/2024 5:21:40 PM
 *  Author: Sasoon Haikazian
 */ 


#ifndef LCD_H_
#define LCD_H_


void lcd_data_register(unsigned char data);
void lcd_instruction_register(unsigned char command);
void lcd_string(const char str[]);
void lcd_initialise();




#endif /* LCD_H_ */