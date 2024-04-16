/*
 * PreLab4.2.c
 *
 * Created: 09/04/2024 09:09:51 p. m.
 * Author : e-mel
 */ 
#define F_CPU 16000000 // Frecuencia del microcontrolador (16MHz)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void init_ports(void);
void initADC(void);
void Salidas(void);

int Dec = 0;
int Uni = 0;
uint8_t ValorADC;
uint8_t contador = 0;
const int seg7[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

int main(void) {
	cli();
	init_ports();
	initADC();
	sei();

	uint8_t boton_estado_PC4 = 1; // Estado anterior del botón PC4
	uint8_t boton_estado_PC5 = 1; // Estado anterior del botón PC5

	while (1) {
			
		// Incrementar contador si el botón PC4 se presiona y se libera
		if (!(PINC & (1 << PC4)) && boton_estado_PC4) {
			contador--;
			if (contador > 255) {
				contador = 0;
			}
			boton_estado_PC4 = 0; // Marcar que el botón PC4 está presionado
		}
		// Actualizar el estado anterior del botón PC4
		if (PINC & (1 << PC4)) {
			boton_estado_PC4 = 1;
		}

		// Decrementar contador si el botón PC5 se presiona y se libera
		if (!(PINC & (1 << PC5)) && boton_estado_PC5) {
			contador++;
			if (contador < 0) {
				contador = 255;
			}
			boton_estado_PC5 = 0; // Marcar que el botón PC5 está presionado
		}
		// Actualizar el estado anterior del botón PC5
		if (PINC & (1 << PC5)) {
			boton_estado_PC5 = 1;
		}

		ADCSRA |= (1<<ADSC); //Habilita conv
		
		Salidas();
		
		if (contador == ValorADC)
		{
			PORTD |= (1<<PD7);
		} 
		else
		{
			PORTD &= ~(1<<PD7);
		}
	}

	return 0;
}

void init_ports(void) {
	// Configuración de pines de entrada y salida
	// Pines PC0-PC3 como salida para los LEDs
	DDRC |= 0x0F;
	// Pines PC4 y PC5 como entrada para los botones
	DDRC &= ~(1 << PC4) & ~(1 << PC5);
	// Pines PB0-PB3 como salida para los LEDs
	DDRB |= 0xF;
	
	// Puerto D como salida
	DDRD = 0xFF;
	PORTD = 0;
	// Utilizar Rx y Tx
	UCSR0B = 0;
	
	// Pines PB4 y PB5 como salida para transistores
	DDRB |= (1 << PB4) | (1 << PB5);
}

void initADC(void){
	ADMUX = 0;
	
	// Vref = 5V
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	ADMUX |= (1<<ADLAR); //Justificando IZQ
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADEN); //Enciende ADC
	ADCSRA |= (1<<ADIE); //ISR ADC
	
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //PrescaleR 128 > 16M/128 = 125kHz
	
	ADMUX |= (1<<MUX1) | (1<<MUX2); //canal 6
}

ISR(ADC_vect){
	ValorADC =  ADCH;
	Dec = (ValorADC >> 4) & 0x0F;
	Uni = ValorADC & 0x0F;
	ADCSRA |= (1<<ADIF);
}

void Salidas(void){
	//Mostrar valor unidades (1erDisplay)
	_delay_ms(10);
	PORTD = seg7[Uni];
	PORTB &= ~(1<<PB5);
	PORTB |= (1<<PB4);
	_delay_ms(10);
	//Mostrar valor decenas (2ndoDisplay)
	PORTD = seg7[Dec];
	PORTB &= ~(1<<PB4);
	PORTB |= (1<<PB5);
	_delay_ms(5);
	// Mostrar el valor del contador en los LEDs
	PORTB = contador & 0x0F; // Los 4 bits menos significativos en PB0-PB3
	PORTC = (contador >> 4) & 0x0F; // Los 4 bits más significativos en PC0-PC3
}