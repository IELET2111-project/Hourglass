
#include <avr/io.h>
#include "serial.h"
#include <util/setbaud.h>


void initSerial() {
	UBRR0 = UBRR_VALUE;								// Set UBBR according to system clock and desired baud rate
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);					// Turn on the transmission and reception circuitry
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);					// 8 data bits, 1 stop bit
}

void transmitByte(uint8_t data) {
	while (!(UCSR0A & (1<<UDRE0)));						// Wait for empty transmit buffer
	UDR0 = data;
}

uint8_t receiveByte() {
  while (!(UCSR0A & (1<<RXC0)));			// Wait for incoming data
  return UDR0;
}

void printString(char* string) {
	while (*string) {
		transmitByte(*string);
		string++;
	}
}

void printNumber(int16_t number) {
	if (!number) {
		transmitByte('0');
		return;
	}
	if (number < 0) {
		transmitByte('-');
		number *= -1;							// Necessary for ASCII convertion
	}
	for (int16_t i = 10000; i >= 1; i /= 10) {
		if (number/i || !number) {				// Only prints 0 if it is to the right of the most significant digit
			transmitByte(number/i + '0');
		}
		number %= i;
	}
}
