/*
 * PreLab5.c
 *
 * Created: 15/04/2024 09:50:21 p. m.
 * Author : e-mel
 */ 

#define F_CPU 16000000
#include "Functions/Functions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

uint8_t ValorADC = 0;
uint8_t Pot = 0;
uint8_t Pot2 = 0;
uint8_t Pot3 = 0;
uint8_t Cycle = 0;

uint16_t ADCtoServo(uint8_t ValorADC){
	return (uint16_t)(((float)ValorADC/255)*(124.9-62.45)+62.45);
}

void setup(void){
	DDRC = 0;
	
}

int main(void)
{
	initPWM1FastA(0, 0);
	initPWM1FastB(4, 0);
	
	initADC(1,6,128);
	
	sei();
    /* Replace with your application code */
    while (1) 
    {
		readADC(6);
		Pot2 = ValorADC;
		updateDutyCycleA(Pot2/6);
		_delay_ms(10);
    }
}

ISR(ADC_vect){
	ValorADC =  ADCH;
	ADCSRA |= (1<<ADIF);
}

