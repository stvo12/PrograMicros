/*
 * PreLab6.c
 *
 * Created: 23/04/2024 11:47:17 p. m.
 * Author : e-mel
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t bufferTX = 0;
uint8_t bufferRX = 0;

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
	WriteUART('C');
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
	WriteUART(13);
	/*
	TextUART("Hola mundo SI");
	WriteUART(10);
	WriteUART(13);
	*/
}

int main(void)
{
    setup();
	sei();
	UART_msj();
    while (1) 
    {
		
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
	OutLEDS();
	UART_msj();
}