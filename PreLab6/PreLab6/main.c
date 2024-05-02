/*
 * PreLab6.c
 *
 * Created: 23/04/2024 11:47:17 p. m.
 * Author : e-mel
 */ 

#define F_CPU 16000000
#include "Functions/Functions.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>

uint8_t ValorADC = 0;
uint8_t bufferTX = 0;
uint8_t bufferRX = 0;
uint8_t centenas = 0;
uint8_t decenas = 0;
uint8_t unidades = 0;
uint8_t mosCen = 0;
uint8_t mosDec = 0;
uint8_t mosUni = 0;
bool mostrar = false;

void initUART(void);
void WriteUART(char Caracter);
void TextUART(char * Texto);
void UART_msj();
void OutLEDS();
void Menu();

void initUART(void){
	//rx y tx como entrada y salida
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	UCSR0A = 0;
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	
	UCSR0C = 0;
	//asincrono, sin paridad, 1 stop, 8 caract
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	//valor baud y velocidad(9600)
	UBRR0 = 103;
}

void WriteUART(char Caracter){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = Caracter;
}

void TextUART(char * Texto){
	uint8_t i;
	for (i=0; Texto[i]!= '\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)) );
		UDR0 = Texto[i];
	}
}

void setup(void){
	DDRB = 0x3F;
	DDRC = 0x03;
	initUART();
}

void UART_msj(){
	WriteUART('\n');
	/*WriteUART('C');
	WriteUART('A');
	WriteUART('R');
	WriteUART('A');
	WriteUART('C');
	WriteUART('T');
	WriteUART('E');
	WriteUART('R');
	WriteUART(' ');
	WriteUART('A');
	WriteUART(' ');
	WriteUART('M');
	WriteUART('O');
	WriteUART('S');
	WriteUART('T');
	WriteUART('R');
	WriteUART('A');
	WriteUART('R');
	WriteUART('\n');
	WriteUART(10);
	WriteUART(13);*/
	
	TextUART("Caracter a mostrar");
	WriteUART(10);
	WriteUART(13);
	
}

void Menu(){
	WriteUART(10);
	WriteUART(13);
	TextUART("1. Leer valor potenciometro");
	WriteUART(10);
	WriteUART(13);
	TextUART("2. Enviar ASCII");
	WriteUART(10);
	WriteUART(13);
}

int main(void)
{
    setup();
	initADC(1,6,128);
	sei();
	Menu();
    while (1) 
    {
		readADC(6);
    }
}

void OutLEDS(void){
	PORTB = (PORTB & 0xC0) | (bufferTX & 0x3F);
	PORTC = (PORTC & 0xFC) | ((bufferTX >> 6) & 0x03);
}

ISR(USART_RX_vect){
	bufferTX = UDR0;
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = bufferTX;
	if (mostrar == true)
	{
		OutLEDS();
		mostrar = false;
		WriteUART(10);
		WriteUART(13);
		Menu();
	} 
	else
	{
		if (bufferTX == 49)
		{
			WriteUART(10);
			WriteUART(13);
			readADC(6);
			PORTB = ValorADC;
			PORTC = (ValorADC >> 6);
			centenas = ValorADC/100;
			decenas = (ValorADC - (centenas*100))/10;
			unidades = ValorADC - (centenas*100 + decenas*10);
			mosCen = centenas + 48;
			mosDec = decenas + 48;
			mosUni = unidades + 48;
			
			WriteUART(10);
			WriteUART(13);
			char mensaje[30];
			sprintf(mensaje, "Valor Potenciometro: %c%c%c\r\n", mosCen, mosDec, mosUni);
			TextUART(mensaje);
			
			/*WriteUART(mosCen);
			WriteUART(mosDec);
			WriteUART(mosUni);*/
			Menu();
		}
		else if (bufferTX == 50)
		{
			mostrar = false;
			WriteUART(10);
			WriteUART(13);
			UART_msj();
			//OutLEDS();
			mostrar = true;
			//Menu();
		} 
		else
		{
			WriteUART(10);
			WriteUART(13);
			TextUART("Input invalido");
			Menu();
			WriteUART(10);
			WriteUART(13);
		}
	}
	/*OutLEDS();
	UART_msj();*/
}

ISR(ADC_vect){
	ValorADC =  ADCH;
	ADCSRA |= (1<<ADIF);
}