/*
 * lcd.c
 *
 * Created: 4/14/2024 5:21:13 PM
 *  Author: Sasoon Haikazian
 */ 
#include "config.h"
#include "lcd.h"
#include <util/delay.h>
#include <avr/io.h>


void lcd_data_register(unsigned char data){
	PORTA = data; //sends char to data ram of lcd byte at a time
	PORTC |= (1<<7); //Reg select to 1 (data reg)
	PORTE |= (1<<3); // enable write to high
	_delay_ms(10);
	PORTE &= ~(1<<3); //enable write to low

}

void lcd_instruction_register(unsigned char command){
	PORTA = command;
	PORTC &= ~(1<<7); //Reg select to 0 (instruction reg)
	PORTE |= (1<<3); // write enable to high
	_delay_ms(10);
	PORTE &= ~(1<<3); //write enable to low
}

void lcd_string(const char str[]){
	unsigned int i = 0;
	while(str[i] != '\0'){
		lcd_data_register(str[i]);
		i++;
	}

}

void lcd_initialise(){
	lcd_instruction_register(0x38);
	lcd_instruction_register(0x06);
	lcd_instruction_register(0x0C);
	lcd_instruction_register(0x01);
}
