/*
 Universidad del Valle de Guatemala
 IE2023: Programación de microcontrolodares
 Autor : e-mel
 Proyecto: PreLab2.asm
 Archivo: Lab2.asm
 Hardware: ATMEGA328P
 Created: 07/02/2024 03:03:47 p. m.
*/
//Encabezado
.include "M328PDEF.inc"
.cseg
.org 0x00

//Config
LDI R16, LOW(RAMEND)
OUT SPL, R16
LDI R17, HIGH(RAMEND)
OUT SPH, R17

	//Tabla
TABLA7SEG: .DB 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x58, 0x5E, 0x79, 0x71

LDI ZL, LOW(TABLA7SEG << 1)
LDI ZH, HIGH(TABLA7SEG << 1)

LDI R24, 0x07 ;una posicion abajo de la tabla
LDI R25, 0x18 ;una posicion arriba de la tabla

//Cod
Setup:
	SBI DDRB, PB5
	CBI PORTB, PB5
	//LDI R16, 100
	//CALL Init_T0

	LDI R20, 0

	LDI R21, 0
	//I/O PORTS CONFIG
	//Puerto B y D como salidas
	LDI R16, 0b1111_1111 
	OUT DDRB, R16
	OUT DDRD, R16
	//Establecer Rx y Tx como salida
	LDI R16,0x00
	STS UCSR0B, R16
	OUT	PORTD, R16
	//PC0 y PC1 como entrada
	LDI R16, 0x03
	OUT PORTC, R16

Loop:
	/*IN R16, TIFR0
	CPI R16, (1<<TOV0)//SBIS registro de banderas, overflow.
	BRNE Loop

	LDI R16, 120
	OUT TCNT0, R16
	
	SBI TIFR0, TOV0 

	INC R20
	CPI R20, 10
	BRNE Loop

	CLR R20

	INC R21*/
	OUT PORTB, R21
	OUT PORTD, R21

	IN R16, PINC ;lee el inputs de los botones
	SBRS R16, PC0
	RJMP Incre
	OUT PORTD, R17

	IN R16, PINC
	SBRS R16, PC1
	RJMP Decre
	OUT PORTD, R17
	//SBI PINB, PB5

	RJMP Loop
/*
Init_T0:
	LDI R16, (1 << CS02) | (1 << CS00)
	OUT TCCR0B, R16

	LDI R16, 100
	OUT TCNT0, R16
	RET*/

//Logica delay bounce e incremento en el display
Incre:
	LDI R16, 100
	delay1:
		DEC R16
		BRNE delay1

	SBIS PINC, PC0 ;salta si bit PC0 es1
	RJMP Incre
	LPM R17, Z
	INC ZL
	CPSE ZL, R25 ;compara la posicion de zl con el r25 el cual es 0x0018, si son iguales regresa al inicio de la tabla
	RJMP Loop
	LDI ZL, 0x08 ;se realiza el skip si son iguales y pone zl hasta el inicio de la tabla
	RJMP Loop

//Logica delay bounce y decrece el display
Decre:
	LDI R16, 100
	delay2:
		DEC R16
		BRNE delay2

	SBIS PINC, PC1 ;salta si bit PC1 es1
	RJMP Decre
	LPM R17, Z
	DEC ZL
	CPSE ZL, R24 ;compara la posicion de zl con el r24 el cual es 0x0007, si son iguales regresa al final de la tabla
	RJMP Loop
	LDI ZL, 0x17 ;se realiza el skip si son iguales y pone zl hasta el final de la tabla
	RJMP Loop