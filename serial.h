/*
 * serial.h
 *
 * Created: 22.04.2021 14:37:27
 *  Author: espen
 */

#ifndef USART_H_
#define USART_H_

#ifndef F_CPU
#define F_CPU 16000000UL	// System clock
#endif

#ifndef BAUD
#define BAUD 9600UL			// BAUD rate
#endif

/************************************************************************/
/* Sets UBBR according to system clock and desired baud rate. Turns on  */
/* the transmission and reception circuitry. Configures 8 data bits and */
/* 1 stop bit.					                                        */
/************************************************************************/
void initSerial();

/************************************************************************/
/* Waits for empty transmission buffer and sends data.                  */
/************************************************************************/
void transmitByte(uint8_t data);

/************************************************************************/
/* Waits for incoming data and returns it.                              */
/************************************************************************/
uint8_t receiveByte();

/************************************************************************/
/* Sends any number between -32,768 and +32,767 via serial with NULL    */
/* termination.                                                         */
/************************************************************************/
void writeNumber(int16_t number);

/************************************************************************/
/* Reads any number between -32,768 and +32,767 via serial. The number  */
/* must be NULL terminated.                                             */
/************************************************************************/
int16_t readNumber();

/************************************************************************/
/* Sends any ASCII string via serial. String must be NULL terminated.   */
/************************************************************************/
void writeString(char* string);

/************************************************************************/
/* Reads any ASCII string via serial. String must be NULL or '\r'       */
/* terminated. Resulting string is NULL terminated.                     */
/************************************************************************/
void readString(char* string);

#endif /* USART_H_ */
