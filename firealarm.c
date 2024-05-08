/*
 * firealarm.c
 *
 * Created: 4/14/2024 5:22:35 PM
 *  Author: Sasoon Haikazian
 */ 
#include "config.h"
#include <avr/io.h>
#include <avr/delay.h>
#include "firealarm.h"
#include "lcd.h"


void common_trouble(){
	
	while( PINL & (1<<6) ){

		if( (~PIND & (1<<0)) | (~PIND & (1<<1)) | (~PIND & (1<<2)) | (~PIND & (1<<3)) | (~PINE & (1<<4)) ){break;}

		if((PINE) & (1<<5) ){
			PORTC &= ~(1<<1);
			lcd_instruction_register(0x01);
			lcd_instruction_register(0x80);
			lcd_string("Fire Alarm Panel");
			lcd_instruction_register(0xC0);
			lcd_instruction_register(0x14);
			lcd_string("System Normal");
			return;
		}
		PORTC |= (1<<1);
		PORTL |= (1<<4);
		_delay_ms(1000);
		PORTL &= ~(1<<4);
		PORTC &= ~(1<<1);
		_delay_ms(1000);

	}
	PORTC |= (1<<1);
	PORTL &= ~(1<<4);
}




void battery_trouble(){

	while( PINL & (1<<6) ){

		if( (~PIND & (1<<0)) | (~PIND & (1<<1)) | (~PIND & (1<<2)) | (~PIND & (1<<3)) | (~PINE & (1<<4)) ){break;}

		if( (PINB & (1<<4)) && (PINB & (1<<6)) ){
			PORTC &= ~(1<<1);
			PORTC &= ~(1<<0);
			lcd_initialise();
			lcd_string("Fire Alarm Panel");
			lcd_instruction_register(0xC0);
			lcd_instruction_register(0x14);
			lcd_string("System Normal");
			return;
		}

		PORTC |= (1<<0);
		PORTL |= (1<<4);
		_delay_ms(1000);
		PORTL &= ~(1<<4);
		PORTC &= ~(1<<0);
		_delay_ms(1000);

	}

	PORTC |= (1<<0) | (1<<1);
	PORTL &= ~(1<<4);

}



void temporal(){
	while( PINL & (1<<6) ){
		
		PORTL |= 0b00010100;
		_delay_ms(500);


		
		PORTL &= ~0b00010100;
		
		_delay_ms(400);



		PORTL |= 0b00010100;
		_delay_ms(500);


		PORTL &= ~0b00010100;
		
		_delay_ms(400);


		PORTL |= 0b00010100;

		_delay_ms(500);


		PORTL &= ~0b00010100;

		_delay_ms(1200);
	}
	
}
