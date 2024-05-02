/*
 * Functions.h
 *
 * Created: 15/04/2024 11:12:17 p. m.
 *  Author: e-mel
 */ 


#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <avr/io.h>
#include <stdint.h>

void initADC(uint8_t just, uint8_t canal, uint8_t prescale);
uint16_t readADC(uint16_t canal);


void initPWM0(void);

void initPWM1FastA(uint8_t inverted, uint8_t mode);//invertido = 1, modo (8bits= 0, 9=1, 10=2, ICR=3, OCR=4)
void initPWM1FastB(uint8_t prescaler, uint8_t mode);//prescaler (0=0,1=8,2=64,3=256,4=1024)

void initPWM2FastA(uint8_t inverted, uint8_t mode);//invertido = 1, modo (8bits= 0, 9=1, 10=2, ICR=3, OCR=4)
void initPWM2FastB(uint8_t prescaler, uint8_t mode);//prescaler (0=0,1=8,2=64,3=256,4=1024)

void updateDutyCycleA(uint8_t Cycle);
void updateDutyCycleB(uint8_t Cycle);

#endif /* FUNCTIONS_H_ */