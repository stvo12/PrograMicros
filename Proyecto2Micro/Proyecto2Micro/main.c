/*
 * Proyecto2Micro.c
 *
 * Created: 07/05/2024 09:32:05 p. m.
 * Author : e-mel
 */ 

#define F_CPU 16000000
#include "Functions/Functions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t bufferTX = 0;
uint8_t bufferRX = 0;
uint8_t modo = 0;
uint8_t pos = 0;

uint8_t* address = 0;

uint8_t EEMEM POT1_1 = 0;
uint8_t EEMEM POT2_1 = 0;
uint8_t EEMEM POT3_1 = 0;
uint8_t EEMEM POT4_1 = 0;
uint8_t EEMEM POT5_1 = 0;

uint8_t EEMEM POT1_2 = 0;
uint8_t EEMEM POT2_2 = 0;
uint8_t EEMEM POT3_2 = 0;
uint8_t EEMEM POT4_2 = 0;
uint8_t EEMEM POT5_2 = 0;

uint8_t EEMEM POT1_3 = 0;
uint8_t EEMEM POT2_3 = 0;
uint8_t EEMEM POT3_3 = 0;
uint8_t EEMEM POT4_3 = 0;
uint8_t EEMEM POT5_3 = 0;

uint8_t EEMEM POT1_4 = 0;
uint8_t EEMEM POT2_4 = 0;
uint8_t EEMEM POT3_4 = 0;
uint8_t EEMEM POT4_4 = 0;
uint8_t EEMEM POT5_4 = 0;
//uint8_t Tra1 = DDD5;
//uint8_t Tra2 = DDD4;

uint8_t ValorADC = 0;
uint8_t Pot1 = 0;
uint8_t Pot2 = 0;
uint8_t Pot3 = 0;
uint8_t Pot4 = 0;
uint8_t Pot5 = 0;

uint8_t centenas = 0;
uint8_t decenas = 0;
uint8_t unidades = 0;
uint8_t mosCen = 0;
uint8_t mosDec = 0;
uint8_t mosUni = 0;

uint8_t i;

uint8_t modoUART = 0;
uint8_t POT = 0;
uint8_t esperandoSegundoInput = 0;
uint8_t esperandoTercerInput = 0;
uint8_t inputCount = 0;
char potString[3] = {0};

bool mostrar = false;

void Menu(void);
void Setup(void);
void Servos(void);

int main(void)
{
    Setup();
	
	initUART();
	
	initPWM0FastA(0, 0);
	initPWM0FastB(4, 0);
	initPWM1FastA(0, 0);
	initPWM1FastB(4, 0);
	initPWM2FastA(0, 0);
	initPWM2FastB(4, 0);
	
	initADC(1, 6, 128);
	_delay_ms(100);
	initADC(1, 7, 128);
	_delay_ms(100);
	initADC(1, 5, 128);
	_delay_ms(100);
	initADC(1, 4, 128);
	_delay_ms(100);
	initADC(1, 3, 128);
	_delay_ms(100);
	
	sei();
	//Menu();
    while (1) 
    {
		if (modo == 0)
		{
			TextUART("Estas en la modo 1");
			Servos();
			
			centenas = Pot1/100;
			decenas = (Pot1 - (centenas*100))/10;
			unidades = Pot1 - (centenas*100 + decenas*10);
			mosCen = centenas + 48;
			mosDec = decenas + 48;
			mosUni = unidades + 48;
			
			WriteUART(10);
			WriteUART(13);
			char mensaje[30];
			sprintf(mensaje, "Valor Potenciometro: %c%c%c\r\n", mosCen, mosDec, mosUni);
			TextUART(mensaje);
			
		}
		else if (modo == 1)
		{
			TextUART("Estas en la modo 2");
		}
		else if (modo == 2)
		{
			TextUART("Estas en la modo 3");
		}
		else
		{
			//modo = 0;
		}
    }
}

void Setup(void){
	DDRC &= ~((1<<PC0) | (1<<PC1) | (1<<PC2));
	//DDRB &= ~((1<<PB0) | (1<<PB1) | (1<<PB2));
	//PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2);
	DDRD = 0xB0;
	DDRB = 0x01;
		
	// Habilitar la interrupción de cambio de pin en PC0 (PCINT8) y PC3 (PCINT11)
	PCICR |= (1 << PCIE1);  // Habilitar interrupciones de cambio de pin para el grupo PCINT[14:8]
	PCMSK1 |= (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10);  // Habilitar interrupciones de cambio de pin para PC0 (PCINT8), PC1 (PCINT9) y PC2 (PCINT10)
	/*
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2);*/
}

void Servos(void){
	readADC(7);
	Pot1 = ValorADC;
	updateDutyCycle2A(Pot1/6);
	_delay_ms(50);
	readADC(6);
	Pot2 = ValorADC;
	updateDutyCycle1B(Pot2/6);
	_delay_ms(50);
	readADC(5);
	Pot3 = ValorADC;
	updateDutyCycle1A(Pot3/6);
	_delay_ms(50);
	readADC(4);
	Pot4 = ValorADC;
	updateDutyCycle0A(Pot4/7);
	_delay_ms(50);
	readADC(3);
	Pot5 = ValorADC;
	updateDutyCycle2B(Pot5/6);
	_delay_ms(50);
}



ISR(ADC_vect){
	ValorADC =  ADCH;
	ADCSRA |= (1<<ADIF);
}

ISR(PCINT1_vect){
	if (PINC == 0x01)
	{
		pos++;
		if (pos > 3)
		{
			pos = 0;
		}
		PORTD = (PORTD & 0xCF) | ((pos & 0x03) << 4);
		if (modo == 2)
		{
			switch (pos){
				case 0:
				Pot1 = eeprom_read_byte(&POT1_1);
				updateDutyCycle2A(Pot1/6);
				Pot2 = eeprom_read_byte(&POT2_1);
				updateDutyCycle1B(Pot2/6);
				Pot3 = eeprom_read_byte(&POT3_1);
				updateDutyCycle1A(Pot3/6);
				Pot4 = eeprom_read_byte(&POT4_1);
				updateDutyCycle0A(Pot4/6);
				Pot5 = eeprom_read_byte(&POT5_1);
				updateDutyCycle2B(Pot5/6);
				break;
				case 1:
				Pot1 = eeprom_read_byte(&POT1_2);
				updateDutyCycle2A(Pot1/6);
				Pot2 = eeprom_read_byte(&POT2_2);
				updateDutyCycle1B(Pot2/6);
				Pot3 = eeprom_read_byte(&POT3_2);
				updateDutyCycle1A(Pot3/6);
				Pot4 = eeprom_read_byte(&POT4_2);
				updateDutyCycle0A(Pot4/6);
				Pot5 = eeprom_read_byte(&POT5_2);
				updateDutyCycle2B(Pot5/6);
				break;
				case 2:
				Pot1 = eeprom_read_byte(&POT1_3);
				updateDutyCycle2A(Pot1/6);
				Pot2 = eeprom_read_byte(&POT2_3);
				updateDutyCycle1B(Pot2/6);
				Pot3 = eeprom_read_byte(&POT3_3);
				updateDutyCycle1A(Pot3/6);
				Pot4 = eeprom_read_byte(&POT4_3);
				updateDutyCycle0A(Pot4/6);
				Pot5 = eeprom_read_byte(&POT5_3);
				updateDutyCycle2B(Pot5/6);
				break;
				case 3:
				Pot1 = eeprom_read_byte(&POT1_4);
				updateDutyCycle2A(Pot1/6);
				Pot2 = eeprom_read_byte(&POT2_4);
				updateDutyCycle1B(Pot2/6);
				Pot3 = eeprom_read_byte(&POT3_4);
				updateDutyCycle1A(Pot3/6);
				Pot4 = eeprom_read_byte(&POT4_4);
				updateDutyCycle0A(Pot4/6);
				Pot5 = eeprom_read_byte(&POT5_4);
				updateDutyCycle2B(Pot5/6);
				break;
			}
		}
		
	}
	else if (PINC == 0x02)
	{
		modo++;
		if (modo > 2)
		{
			modo = 0;
		}
		PORTD = (PORTD & ~(1 << PD7)) | ((modo & 0x01) << PD7);
		PORTB = (PORTB & ~(1 << PB0)) | ((modo & 0x02) >> 1);
	}
	else if (PINC == 0x04) {
		switch (pos){
			case 0:
			eeprom_write_byte(&POT1_1, Pot1);
			eeprom_write_byte(&POT2_1, Pot2);
			eeprom_write_byte(&POT3_1, Pot3);
			eeprom_write_byte(&POT4_1, Pot4);
			eeprom_write_byte(&POT5_1, Pot5);
			break;
			case 1:
			eeprom_write_byte(&POT1_2, Pot1);
			eeprom_write_byte(&POT2_2, Pot2);
			eeprom_write_byte(&POT3_2, Pot3);
			eeprom_write_byte(&POT4_2, Pot4);
			eeprom_write_byte(&POT5_2, Pot5);
			break;
			case 2:
			eeprom_write_byte(&POT1_3, Pot1);
			eeprom_write_byte(&POT2_3, Pot2);
			eeprom_write_byte(&POT3_3, Pot3);
			eeprom_write_byte(&POT4_3, Pot4);
			eeprom_write_byte(&POT5_3, Pot5);
			break;
			case 3:
			eeprom_write_byte(&POT1_4, Pot1);
			eeprom_write_byte(&POT2_4, Pot2);
			eeprom_write_byte(&POT3_4, Pot3);
			eeprom_write_byte(&POT4_4, Pot4);
			eeprom_write_byte(&POT5_4, Pot5);
			break;
		}
		//eeprom_update_byte(&eeprom_pos, pos);
	}
}

ISR(USART_RX_vect){
	bufferTX = UDR0;
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = bufferTX;
	if (modo == 1)
	{	
		if (!esperandoSegundoInput)
		{
			if (bufferTX == 48)
			{
				modoUART = 0;
				esperandoSegundoInput = 1;
			}
			else if (bufferTX == 97)
			{
				modoUART = 1;
				esperandoSegundoInput = 0;
			}
			else if (bufferTX == 49)
			{
				POT = 0;
			}
			else if (bufferTX == 50)
			{
				POT = 1;
			} 
			else if (bufferTX == 51)
			{
				POT = 2;
			}
			else if (bufferTX == 52)
			{
				POT = 3;
			}
			else if (bufferTX == 53)
			{
				POT = 4;
			}
			else if (bufferTX == 54)
			{
				pos = 0;
			}
			else if (bufferTX == 55)
			{
				pos = 1;
			}
			else if (bufferTX == 56)
			{
				pos = 2;
			}
			else if (bufferTX == 57)
			{
				pos = 3;
			}
			PORTD = (PORTD & 0xCF) | ((pos & 0x03) << 4);
		}
		
		/*if (modoUART == 0)
		{
			/ *potString[inputCount++] = bufferTX; // Almacenar el dígito recibido
			if (inputCount == 3 || (inputCount == 2 && potString[0] == '1')) { // Condición para completar el valor
				potString[inputCount] = '\0'; // Terminar la cadena* /
			if (bufferTX >= 11 && bufferTX <= 255)
			{
				static char inputBuffer[4]; // Buffer para almacenar el mensaje recibido
				static uint8_t inputPos = 0; // Posición actual en el buffer

				char receivedChar = UDR0; // Leer el carácter recibido

				if (receivedChar == '\n') {
					inputBuffer[inputPos] = '\0'; // Termina la cadena
					inputPos = 0; // Resetea la posición del buffer

					uint16_t potValue = atoi(inputBuffer);// Convierte la cadena a entero
					switch (POT) {
						case 0:
						Pot1 = potValue;
						updateDutyCycle2A(Pot1/6);
						break;
						case 1:
						Pot2 = potValue;
						updateDutyCycle1B(Pot2/6);
						break;
						case 2:
						Pot3 = potValue;
						updateDutyCycle1A(Pot3/6);
						break;
						case 3:
						Pot4 = potValue;
						updateDutyCycle0A(Pot4/6);
						break;
						case 4:
						Pot5 = potValue;
						updateDutyCycle2B(Pot5/6);
						break;
					}
				}
			}
		}*/
		
			if (modoUART == 0 && esperandoSegundoInput == 1)
			{
				/*if (bufferTX >= 49 && bufferTX <= 53) { // Acepta valores '1' a '4'
					POT = bufferTX - 49; // Ajustar POT según el segundo input
					esperandoSegundoInput = 0; // Resetear segundo input
					esperandoTercerInput = 1; // Esperar el tercer input
					inputCount = 0; // Reiniciar contador de caracteres del tercer input
				}*/
				potString[inputCount++] = bufferTX; // Almacenar el dígito recibido
				if (inputCount == 3 || (inputCount == 2 && potString[0] == '1')) { // Condición para completar el valor
					potString[inputCount] = '\n'; // Terminar la cadena
					uint16_t potValue = atoi(potString); // Convertir cadena a entero
					if(potValue >= 11 && potValue <= 255){
						switch (POT) {
							case 0:
							Pot1 = potValue;
							updateDutyCycle2A(Pot1/6);
							break;
							case 1:
							Pot2 = potValue;
							updateDutyCycle1B(Pot2/6);
							break;
							case 2:
							Pot3 = potValue;
							updateDutyCycle1A(Pot3/6);
							break;
							case 3:
							Pot4 = potValue;
							updateDutyCycle0A(Pot4/6);
							break;
							case 4:
							Pot5 = potValue;
							updateDutyCycle2B(Pot5/6);
							break;
						}
					}
					esperandoTercerInput = 0; // Resetear después de procesar el tercer input
				}
				
			}
			
			
	}
}
