/*
 * serial.c
 *
 * Created: 22.04.2021 14:37:33
 *  Author: espen
 */

#include <avr/io.h>
#include "serial.h"
#include <util/setbaud.h>

void initSerial() {
	UBRR0 = UBRR_VALUE;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

void transmitByte(uint8_t data) {
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

uint8_t receiveByte() {
  	while (!(UCSR0A & (1<<RXC0)));
  	return UDR0;
}

void writeNumber(int16_t number) {
	if (number == 0) {							// Consider case where number = 0
		transmitByte('0');
		transmitByte(0);
		return;
	}
	if (number < 0) {							// Case where number is negative
		transmitByte('-');
		number *= -1;							// Necessary for ASCII conversion
	}
	uint8_t mostSignificantDigitFound = 0;
	for (int16_t i = 10000; i >= 1; i /= 10) {
		if (!mostSignificantDigitFound) {
			mostSignificantDigitFound = number/i;
		} 
		if (mostSignificantDigitFound) {		// Only prints 0 if it is to the right of the most significant digit
			transmitByte(number/i + '0');
		}
		number %= i;
	}
	transmitByte(0); // NULL-termination
}

int16_t readNumber() {
	char thousands = '0';
	char hundreds = '0';
	char tens = '0';
	char ones = '0';
	char digit = '0';
	int16_t sign = 1;
	do {
		thousands = hundreds;
		hundreds = tens;
		tens = ones;
		ones = digit;
		digit = receiveByte();
		if (digit == '-') {						// Handle negative numbers
			sign = -1;
			digit = '0';
		}
	} while (digit != 0);                      // NULL-termination
	return sign*(1000*(thousands - '0') + 100*(hundreds - '0') + 10*(tens - '0') + ones - '0');
}

void writeString(char* string) {
	while (*string) {
		transmitByte(*string);
		string++;
	}
}

void readString(char* string) {
	while (*string) {
		char response = receiveByte();
		if ((response == 0) || (response == '\r')) {
			break;
		} else {
			*string = response;
			string++;
		}
	}
	*string = 0;                           // NULL termination
}
