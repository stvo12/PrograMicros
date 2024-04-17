/*
 * ADC.c
 *
 * Created: 15/04/2024 11:26:32 p. m.
 *  Author: e-mel
 */ 

#include "Functions.h"
void initADC(uint8_t canal){
	ADMUX = 0;
	
	// Vref = 5V
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	ADMUX |= (1<<ADLAR); //Justificando IZQ
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADEN); //Enciende ADC
	ADCSRA |= (1<<ADIE); //ISR ADC
	
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //PrescaleR 128 > 16M/128 = 125kHz
	
	ADMUX = canal; //canal 6
}