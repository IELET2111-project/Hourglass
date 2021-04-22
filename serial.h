/*
 * UART.h
 *
 * Created: 22.04.2021 14:37:27
 *  Author: espen
 */

#ifndef USART_H_
#define USART_H_

#ifndef F_CPU
#define F_CPU 16000000UL								// System clock
#endif

#ifndef BAUD
#define BAUD 9600UL	
#endif

void initSerial();
void transmitByte(uint8_t data);
void printNumber(uint16_t word);
void printString(const char* string)
uint8_t receiveByte();

#endif /* USART_H_ */