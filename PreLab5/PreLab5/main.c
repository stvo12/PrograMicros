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
uint8_t Pot2PWM = 0;
uint8_t Pot3 = 0;
uint8_t Cycle = 0;
uint8_t PWM_Manual = 0;
uint8_t CounterPWM_M = 0;

uint16_t ADCtoServo(uint8_t ValorADC){
	return (uint16_t)(((float)ValorADC/255)*(124.9-62.45)+62.45);
}

void setup(void){
	DDRC = 0;
	
}

void initT0(){
	TCCR0A |=  (1 << WGM00) | (1 << WGM01);
	TCCR0B |= (1 << CS00); // Sin preescaler
	// Habilitar la interrupción de comparación de Timer0
	TIMSK0 |= (1 << OCIE0A);
	
	//Valor Comp
	OCR0A = PWM_Manual;
}

int main(void)
{
	DDRB |= (1<<PD6);
	initPWM0();
	
	initPWM1FastA(0, 0);
	initPWM1FastB(4, 0);
	
	initPWM2FastA(0, 0);
	initPWM2FastB(4, 0);
	
	initADC(1,6,128);
	_delay_ms(100);
	initADC(1,7,128);
	_delay_ms(100);
	initADC(1,5,128);
	
	sei();
    /* Replace with your application code */
    while (1) 
    {
		readADC(6);
		Pot2 = ValorADC;
		//Pot2PWM = ADCtoServo(Pot2);
		updateDutyCycleA(Pot2/6);
		_delay_ms(10);
		readADC(7);
		Pot3 = ValorADC;
		updateDutyCycleB(Pot3/6);
		_delay_ms(10);
		readADC(5);
		PWM_Manual = ValorADC;
		OCR0A = PWM_Manual;
		_delay_ms(5);
    }
}

ISR(ADC_vect){
	ValorADC =  ADCH;
	ADCSRA |= (1<<ADIF);
}

ISR(TIMER0_COMPA_vect) {
	// Incrementar el contador
	CounterPWM_M++;

	// Si el contador alcanza el valor máximo, reiniciar el contador y poner la salida en alto
	if (CounterPWM_M >= 255) {
		PORTB |= (1 << PD6);
		CounterPWM_M = 0;
		} else if (CounterPWM_M >= PWM_Manual) { // Si el contador alcanza el valor deseado, poner la salida en bajo
		PORTB &= ~(1 << PD6);
	}
}
