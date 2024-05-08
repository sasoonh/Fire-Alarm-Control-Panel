/*
 * FACP_REV_2.c
 *
 * Created: 4/14/2024 5:09:59 PM
 * Author: Sasoon Haikazian
 */ 
#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "firealarm.h"


/*
FRONT PANEL LEDS AND NAC is PORTL
ACKL and silence switch is PINL
DRILL and Smoke supervision input is PINE
ZONES is PIND
BATTERY INPUT is PINB
other leds are PORTC
*/



int main(void){

	DDRA |= 0xFF;     //DATA Direction of PORTA
	DDRC |= 0xFF;     //DATA Direction of PORTC
	DDRL |= 0x1F;    //DATA Direction of PORTL
	DDRE |= (1<<3); //DATA Direction of PORTE
	DDRB |= (1<<4); //data direction of PORTE

	lcd_initialise();
	lcd_string("INITIALISING...");
	DDRD  &= ~0b00001111;
	PORTL |= 0b00101000; // enable smoke detectors
	PORTL |= 0b00010100; // test buzzer and alarm led
	PORTC |= 0b00111111; //all other led test

	_delay_ms(2000);
	PORTL &= ~0b00010100; //buzzer and alarm led off
	PORTC &= ~0b00111111; //all other leds off
	_delay_ms(10000);
		
	
	EIMSK |= 0b00111111; //INT0,INT1,INT2,INT3,INT4,INT5 enable (ZONE1,ZONE2,ZONE3,ZONE4, DRILL, SMOKE SUPERVISORY)
	EICRA  = 0; //edge select for INT0,INT1,INT2,INT3 set to low level for all
	EICRB = 0; //edge select for INT4, INT5 set to low level will trigger interrupt
	PCICR |= (1<<0); //pin change interupt control register
	PCMSK0 |= (1<<4) | (1<<6); //PCINT4 and 6 enabled (BAT and low batt) (PCMSK0)
	sei(); //global interrupt enable
	if( ~PINB & (1<<4) ){PORTB |= (1<<4);} //check if battery is connected
	


	lcd_initialise();
	lcd_string("Fire Alarm Panel");
	lcd_instruction_register(0xC0);
	lcd_instruction_register(0x14);
	lcd_string("System Normal");


	while(1){
		;; //standby mode
	}
	

	return 0;
}

ISR (INT0_vect) //zone 1 alarm
{
	_delay_ms(800);
	lcd_initialise();
	lcd_string("Zone1 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_string("East Wing");
	PORTL |= 0b00000011; //nacs and alarm led on
	PORTC |= (1<<5); //zone 1 alarm led
	temporal();
	
	PORTL |= (1<<2); //alarm led solid
	lcd_instruction_register(0x01);
	lcd_instruction_register(0x80);
	lcd_string("Zone1 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_instruction_register(0x14);
	lcd_instruction_register(0x14);
	lcd_string("Acknowledged");

	while( PINL & (1<<7) ){
		;;
	}
	PORTL &= ~0b00000011;
	_delay_ms(1500);

	lcd_initialise();
	lcd_string("Zone1 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_instruction_register(0x14);
	lcd_instruction_register(0x14);
	lcd_string("Silenced");

	
	while(1){
		if( (~PIND & (1<<1)) | (~PIND & (1<<2)) | (~PIND & (1<<3)) ){
			break;
		}
	}
	

}

ISR (INT1_vect) //zone 2 alarm
{
	_delay_ms(800);
	lcd_initialise();
	lcd_string("Zone2 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_string("West Wing");
	PORTC |= 0b00010000; //zone 2 alarm
	PORTL |= 0b00000011; //buzzer,nacs, and alarm led on
	temporal(); //go into function

	PORTL |= (1<<2); //alarm led solid
	lcd_initialise();
	lcd_string("Zone2 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_instruction_register(0x14);
	lcd_instruction_register(0x14);
	lcd_string("Acknowledged");

	while( PINL & (1<<7) ){
		;;
	}

	PORTL &= ~0b00000011;
	_delay_ms(1500);

	lcd_initialise();
	lcd_string("Zone2 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_instruction_register(0x14);
	lcd_instruction_register(0x14);
	lcd_string("Silenced");

	while(1){
		if( (~PIND & (1<<0)) | (~PIND & (1<<2)) | (~PIND & (1<<3)) ){
			break;
		}

	}

}

ISR (INT2_vect) //zone 3 alarm
{
	_delay_ms(800);
	lcd_initialise();
	lcd_string("Zone3 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_string("North Wing");
	PORTL |= 0b00000011; //buzzer,nacs, and alarm led on
	PORTC |= 0b00001000; //zone 3 alarm
	temporal();

	PORTL |= (1<<2); //alarm led solid
	lcd_initialise();
	lcd_string("Zone3 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_instruction_register(0x14);
	lcd_instruction_register(0x14);
	lcd_string("Acknowledged");

	while( PINL & (1<<7) ){
		;;
	}

	PORTL &= ~0b00000011;
	_delay_ms(1500);

	lcd_initialise();
	lcd_string("Zone3 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_instruction_register(0x14);
	lcd_instruction_register(0x14);
	lcd_string("Silenced");

	while(1){
		if( (~PIND & (1<<0)) | (~PIND & (1<<1)) | (~PIND & (1<<3)) ){
			break;
		}
	}

}

ISR (INT3_vect) //zone 4 alarm
{
	_delay_ms(800);
	lcd_initialise();
	lcd_string("Zone4 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_string("South Wing");
	PORTC |= 0b00000100; //zone 4 alarm
	PORTL |= 0b00000011; //buzzer,nacs, and alarm led on
	temporal();
	PORTL |= (1<<2); //alarm led solid
	lcd_initialise();
	lcd_string("Zone4 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_instruction_register(0x14);
	lcd_instruction_register(0x14);
	lcd_string("Acknowledged");

	while( PINL & (1<<7) ){
		;;

	}

	PORTL &= ~0b00000011;
	_delay_ms(1500);

	lcd_initialise();
	lcd_string("Zone4 Fire Alarm");
	lcd_instruction_register(0xC0);
	lcd_instruction_register(0x14);
	lcd_instruction_register(0x14);
	lcd_string("Silenced");

	while(1){
		if( (~PIND & (1<<0)) | (~PIND & (1<<1)) | (~PIND & (1<<2)) ){
			break;
		}
	}


}

ISR (INT4_vect) //DRILL alarm
{
	_delay_ms(2000);
	lcd_initialise();
	lcd_string("FIRE DRILL");
	PORTC |= (1<<2) | (1<<3) | (1<<4) | (1<<5) ;
	PORTL |= (1<<0) | (1<<1); //buzzer,nacs, and alarm led on
	temporal();
	PORTL |= (1<<2);
	while( PINL & (1<<7) ){
		;;
	}

	PORTL &= ~0b00000011;
	_delay_ms(1500);
	lcd_initialise();
	lcd_string("FIRE DRILL");
	lcd_instruction_register(0xC0);
	lcd_instruction_register(0x14);
	lcd_instruction_register(0x14);
	lcd_string("Silenced");
	while(1){
		if( (~PIND & (1<<0)) | (~PIND & (1<<1)) | (~PIND & (1<<2)) | (~PIND & (1<<3)) ){
			PORTC &= ~0b00111100;
		break;}

	}
	
}

ISR (INT5_vect) //SMOKE circuit toruble
{
	lcd_initialise();
	lcd_string("COMMON TROUBLE:");
	lcd_instruction_register(0xC0);
	lcd_string("Smoke Detectors");
	common_trouble();

	while(1){
		if( (~PIND & (1<<0)) | (~PIND & (1<<1)) | (~PIND & (1<<2)) | (~PIND & (1<<3)) ){return;}
		if( (~PINE & (1<<4)) ){return;}
		if( (PINE & (1<<5)) ){
			PORTC &= ~(1<<1);
			lcd_initialise();
			lcd_string("Fire Alarm Panel");
			lcd_instruction_register(0xC0);
			lcd_instruction_register(0x14);
			lcd_string("System Normal");
			break;
		}
	}
	
}

ISR (PCINT0_vect) //batt trouble
{
	PORTB &= ~(1<<4);
	DDRB &= ~(1<<4);
	PORTL &= ~(1<<5); //pwr led off
	PORTC |= (1<<1); //trouble led on, batt led on
	lcd_initialise();
	lcd_string("BATTERY TROUBLE:");
	lcd_instruction_register(0xC0);
	lcd_string("LOW BATT /NO AC");
	battery_trouble();
	while(1){
		if( (~PIND & (1<<0)) | (~PIND & (1<<1)) | (~PIND & (1<<2)) | (~PIND & (1<<3)) ){return;}
		if( (~PINE & (1<<4)) ){return;}
		if( (PINB & (1<<4)) && (PINB & (1<<6)) ){
			PORTC &= ~(1<<1);
			PORTC &= ~(1<<0);
			PORTL |= (1<<5);
			lcd_initialise();
			lcd_string("Fire Alarm Panel");
			lcd_instruction_register(0xC0);
			lcd_instruction_register(0x14);
			lcd_string("System Normal");
			break;
		}

	}

}