/*
 Universidad del Valle de Guatemala
 IE2023: Programación de microcontrolodares
 Autor : e-mel
 Proyecto: PreLab1.asm
 Archivo: Lab1.asm
 Hardware: ATMEGA328P
 Created: 31/01/2024 02:25:55 p. m.
*/
//Encabezado
.include "M328PDEF.inc"
.cseg
.org 0x00

.equ LEDSUMA = (1<<PD6) | (1<<PD7) | (1<<PB4) | (1<<PB5) | (1<<PC5)
//Config
LDI R16, LOW(RAMEND)
OUT SPL, R16
LDI R17, HIGH(RAMEND)
OUT SPH, R17
//Cod
setup:
	LDI R16, 0b1000_0000
	LDI R16, (1<<CLKPCE)
	STS CLKPR, R16
	
	LDI R16, 0b0000_0011
	STS CLKPR, R16

	// I/O PORTS CONFIG
	LDI R16, 0b0001_1111
	OUT PORTC, R16
	//LDI R16, 0b0010_0000
	//OUT DDRC, R16

	LDI R16, 0b1111_1111 
	OUT DDRD, R16

	LDI R16, 0b0011_1111 
	OUT DDRB, R16

	CLR R17 ;contador1
	CLR R18 ;contador2}
	CLR R28
	//LDI R28, 0b0000_0000
	LDI R23, 0b0000_1111
loop:
	IN R16, PINC
	SBRS R16, PC0 ; salta si el bit PC0 esta en 1

	RJMP Incre1
	OUT PORTD, R19


	IN R16, PINC
	SBRS R16, PC1 ; salta si el bit PC1 esta en 1

	CALL Decre1
	OUT PORTD, R19


	IN R16, PINC
	SBRS R16, PC2 ; salta si el bit PC2 esta en 1

	RJMP Incre2
	OUT PORTB, R20


	IN R16, PINC
	SBRS R16, PC3 ; salta si el bit PC3 esta en 1

	CALL Decre2
	OUT PORTB, R20

	/*
	IN R16, PINC
	SBRS R16, PC4 ; salta si el bit PC4 esta en 1

	//CALL Decre2
	//OUT PORTB, R20
	CALL Suma
	// PD6 (carry)
    SBRS R21, 4
	LDI R28, 0b0001_0000
	SBRS R21, 4
    OUT PORTD, R28
    SBRC R21, 4
    OUT PORTD, R28

    // PD7 (bit3)
    SBRS R21, 3
	LDI R28, 0b0000_1000
	SBRS R21, 3
    OUT PORTD, R28
    SBRC R21, 3
    OUT PORTD, R28

    // PB4 (bit2)
    SBRS R21, 2
	LDI R28, 0b0000_0100
    //OUT PORTB, R28
    SBRC R21, 2
    OUT PORTB, R28

    // PB5 (bit1)
    SBRS R21, 1
	LDI R28, 0b0000_0010
	SBRS R21, 1
    OUT PORTB, R28
    SBRC R21, 1
    OUT PORTB, R28

    // PC5 (bit0)
    SBRS R21, 0
	LDI R28, 0b0000_0001
	SBRS R21, 0
    OUT PORTC, R28
    SBRC R21, 0
    OUT PORTC, R28
	*/
	RJMP loop
//
Incre1:
	LDI R16, 100
	delay1:
		DEC R16
		BRNE delay1

	SBIS PINC, PC0 ;salta si bit PC1 es1
	RJMP Incre1 ;repite antirrebote
	INC R17 // Contador1 incrementar 
	AND R17, r23
	
	MOV R19,R17
	LSL R19
	LSL R19

	RJMP Loop

Decre1:
    LDI R16, 100
	delay2:
		DEC R16
		BRNE delay2

	SBIS PINC, PC1 ;salta si bit PC1 es1
	RJMP Decre1 ;repite antirrebote
	DEC R17 // Contador1 decrece 
	AND R17, r23
	
	MOV R19,R17
	LSL R19
	LSL R19

	RJMP Loop

Incre2:
	LDI R16, 100
	delay3:
		DEC R16
		BRNE delay3

	SBIS PINC, PC2 ;salta si bit PC1 es1
	RJMP Incre2 ;repite antirrebote
	INC R18 // Contador2 incrementar 
	AND R18, R23
	
	MOV R20,R18 //
	//LSL R19
	//LSL R19

	RJMP Loop

Decre2:
    LDI R16, 100
	delay4:
		DEC R16
		BRNE delay4

	SBIS PINC, PC3 ;salta si bit PC1 es1
	RJMP Decre2 ;repite antirrebote
	DEC R18 // Contador decrece
	AND R18, R23
	
	MOV R20,R18 //
	//LSL R19
	//LSL R19

	RJMP Loop

Suma:
	LDI R16, 100
	delay5:
		DEC R16
		BRNE delay5

	SBIS PINC, PC4 ;salta si bit PC1 es1
	RJMP Suma
	//MOV R26, R19
	//MOV R27, R20
	ADD R17, R18
	MOV R21, R17
	RJMP Loop