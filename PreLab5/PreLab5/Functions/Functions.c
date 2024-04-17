/*
 * Functions.c
 *
 * Created: 15/04/2024 11:28:25 p. m.
 *  Author: e-mel
 */ 

#include "Functions.h"
#include <avr/io.h>
#include <stdint.h>

void initADC(uint8_t just, uint8_t canal, uint8_t prescale){
	ADMUX = 0;
	
	// Vref = 5V
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADEN); //Enciende ADC
	ADCSRA |= (1<<ADIE); //Int ADC
	
	//Justificación 1(Izq), 0(Der)
	if(just == 1){
		ADMUX |= (1 << ADLAR);
	}
	else{
		ADMUX &= ~(1 << ADLAR);
	}
	
	//Prescaler (cases)
	switch (prescale){
		case 2:
			ADCSRA &= ~((1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2));
			break;
		case 4:
			ADCSRA |= (1 << ADPS1);
			ADCSRA &= ~((1 << ADPS0) | (1 << ADPS2));
			break;
		case 8:
			ADCSRA |= (1 << ADPS0 ) | (1 << ADPS1);
			ADCSRA &= ~(1 << ADPS2);
			break;
		case 16:
			ADCSRA |= (1 << ADPS2);
			ADCSRA &= ~((1 << ADPS0) | (1 << ADPS1));
			break;
		case 32:
			ADCSRA |= (1 << ADPS0 ) | (1 << ADPS2);
			ADCSRA &= ~(1 << ADPS1);
			break;
		case 64:
			ADCSRA |= (1 << ADPS1) | (1 << ADPS2);
			ADCSRA &= ~(1 << ADPS0);
			break;
		case 128:
			ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
			break;
		default:
			ADCSRA &= ~((1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2));
	}
}

uint16_t readADC(uint16_t canal){
	ADMUX = (ADMUX & 0xF0) | canal;
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1 << ADSC));
		return ADC;
}

void initPWM1FastA(uint8_t inverted, uint8_t mode){
	DDRB |= (1 << DDB1);
	TCCR1A = 0;
	// Invertido o no
	if(inverted == 1){
		TCCR1A |=  (1 << COM1A1) | (1 << COM1A0);
	}
	else {
		TCCR1A |= (1 << COM1A1);
	}
	
	// Modo
	switch (mode){
		case 0:
			TCCR1A |= (1 << WGM10);
		break;
		case 1:
			TCCR1A |= (1 << WGM11);
		break;
		case 2:
			TCCR1A |= (1 << WGM10) | (1 << WGM11);
		break;
		case 3:
			TCCR1A |= (1 << WGM10);
		break;
		case 4:
			TCCR1A |= (1 << WGM10) | (1 << WGM11);
		break;
		
		default:
			TCCR1A |= (1 << WGM10);
	}
}

void initPWM1FastB(uint8_t prescaler, uint8_t mode){
	TCCR1B = 0;
	// Prescaler
	switch (prescaler){
		case 0:
			TCCR1B |= (1 << CS10);
		break;
		case 1:
			TCCR1B |= (1 << CS11);
		break;
		case 2:
			TCCR1B |= (1 << CS10) | (1 << CS11);
		break;
		case 3:
			TCCR1B |= (1 << CS12);
		break;
		case 4:
			TCCR1B |= (1 << CS10) | (1 << CS12);
		break;
		default:
			TCCR1B |= (1 << CS10);
	}
	// Modo
	switch (mode){
		case 0:
			TCCR1B |= (1 << WGM12);
		break;
		case 1:
			TCCR1B |= (1 << WGM12);
		break;
		case 2:
			TCCR1B |= (1 << WGM12);
		break;
		case 3:
			TCCR1B |= (1 << WGM12) | (1 << WGM13);
		break;
		case 4:
			TCCR1B |= (1 << WGM12) | (1 << WGM13);
		break;
		default:
			TCCR1B |= (1 << WGM12);
	}
}

void updateDutyCycleA(uint8_t Cycle){
	OCR1A = Cycle;
}