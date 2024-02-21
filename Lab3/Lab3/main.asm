/*
 Universidad del Valle de Guatemala
 IE2023: Programación de microcontrolodares
 Autor : e-mel
 Proyecto: PreLab2.asm
 Archivo: Lab3.asm
 Hardware: ATMEGA328P
 Created: 19/02/2024 10:19:02 p. m.
*/
//Encabezado
.include "M328PDEF.inc"
.cseg
.org 0x00
JMP Config
.org 0x08
JMP PC_Int
.org 0x0020
JMP TIMER0

//Config STACK POINTER
Config:
LDI R16, LOW(RAMEND)
OUT SPL, R16
LDI R17, HIGH(RAMEND)
OUT SPH, R17

//Tabla																	;"9"
TABLA7SEG: .DB 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F;, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71

//I/O Ports
Setup:
	LDI R16, 0b1000_0000//(1<<CLKPCE)
	STS CLKPR, R16
	
	LDI R16, 0b0000_0001
	STS CLKPR, R16
	//Puerto C como entrada pullup
	LDI R16, 0b0000_0000
	OUT DDRC, R16
	LDI R16, 0b1111_1111
	OUT PORTC, R16
	//Puerto B y D como salidas
	LDI R16, 0b1111_1111 
	OUT DDRB, R16
	OUT DDRD, R16
	//Establecer Rx y Tx como salida
	LDI R16,0x00
	STS UCSR0B, R16
	OUT	PORTD, R16
	//Interrupciones
	//Botones
	LDI R16, (1<<PCINT9) | (1<<PCINT8)
	STS PCMSK1, R16

	LDI R16, (1<<PCIE1)
	STS PCICR, R16
	//T0
	LDI R16, (1<<TOIE0)
	STS TIMSK0, R16

	SEI

	RCALL INIT_T0

	LDI R20, 0x00 //Contador LED
	LDI R26, 0 //OUT LEDS
	LDI R21, 0 //Contador seg
	LDI R22, 0 //Contador dec
	LDI R24, 0

	LDI ZL, LOW(TABLA7SEG << 1)
	LDI ZH, HIGH(TABLA7SEG << 1)
	LPM R21, Z
	OUT PORTD, R21

	LDI ZL, LOW(TABLA7SEG << 1)
	LDI ZH, HIGH(TABLA7SEG << 1)
	LPM R24, Z
	OUT PORTD, R24

Loop:
	//Seleccionar transistor seg
	LDI R16, 0xF
	AND R26, R16
	LDI R16, 0x10 //primer display
	OR R26, R16
	OUT PORTB, R26
	OUT PORTD, R21

	RCALL Delay1

	//Seleccionar transistor dec
	LDI R16, 0xF
	AND R26, R16
	LDI R16, 0x20 //segundo display
	OR R26, R16
	OUT PORTB, R26
	OUT PORTD, R24

	RCALL Delay1
	RJMP Loop

//INT
TIMER0:
	LDI R29, 178
	OUT TCNT0, R29
	INC R25
	CPI R25, 100
	BRNE REGR

	CLR R25
	INC R18
	CPI R18, 10
	BREQ Reset
	INCRE:
		LDI ZL, LOW (TABLA7SEG << 1)
		LDI ZH, HIGH (TABLA7SEG << 1)
		ADD ZL, R18
		LPM R21, Z

		LDI ZL, LOW (TABLA7SEG << 1)
		LDI ZH, HIGH (TABLA7SEG << 1)
		ADD ZL, R22
		LPM R24, Z

REGR:
	RETI

Reset: //Reset seg
	LDI R18, 0
	INC R22
	CPI R22, 6 //Compara Decenas
	BREQ Reset2
	INCRE2:
	RJMP INCRE
Reset2: //Reset decenas
	LDI R22, 0
	RJMP INCRE2


INIT_T0: //Interrupcion de Timer0
	LDI R16, (1<<CS02)|(1<<CS00) //Pre escalado a 1024
	OUT TCCR0B, R16
	LDI R29, 178
	OUT TCNT0, R29
	RET

Delay1:
	LDI R16, 255
	LDI R17, 100
	BUCLE:
		DEC R16
		BRNE BUCLE
		LDI R16, 255
		DEC R17
		BRNE BUCLE
		RET

PC_Int:
	MOV R28, R25
	IN R20, PINC
	SBRS R20, PC0
	INC R26
	SBRS R20, PC1
	DEC R26
	RETI

