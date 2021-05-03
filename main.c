/*
 * GccApplication1.c
 *
 * Created: 22.04.2021 15.06.48
 * Author : Bruker
 */

#define F_CPU 16000000UL 	// System clock
#define BAUD 9600UL 		// BAUD rate

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serial.h"

volatile uint16_t potmeter = 0;
volatile uint16_t temperature = 0;		// Stores last ADC conversion from the ADC Data Register (0 - 1023)

/************************************************************************/
/* Initialization of analog-to-digital converter. 						*/
/* Prescaler is set to: 128. 											*/
/* Interrupts is enabled. Reference voltage is set to AVCC. ADC0 is the */
/* initial input channel.                                               */
/************************************************************************/
void initADC() {
	ADCSRA |= (1<<ADEN);										// Enable ADC
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS1);				// Configure prescaler
	ADCSRA |= (1<<ADIE);										// Configure interrupt
	ADMUX |= (0<<REFS1) | (1<<REFS0);							// Set reference to AVCC
	ADMUX |= (0<<ADLAR);										// Configure right adjusted result
	ADMUX |= (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0);		// Configure ADC0 as input channel
}

/************************************************************************/
/* Initialization of the timer using 8-bit Timer0.                      */
/* Prescaler is set to 256x.                                            */
/************************************************************************/
void initTimer() {
	DDRD |= (1<<DDD6);
	TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);	// Fast PWM, non-inverting
	TCCR0B |= (1<<CS02);	// Prescale 256x
}

/************************************************************************/
/* Interrupt service routine for analog-to-digital converter. ISR is    */
/* triggered on finished conversion. Saves conversion and toggles input */
/* channel.														        */
/************************************************************************/
ISR(ADC_vect) {
	static uint8_t inputChannel = 0;				// Stores current analog input channel (ADC0, ADC1...)
	switch (inputChannel) {
		case 0:
			inputChannel = 1; 						// Change input channel to temperature sensor
			temperature = ADC;
			break;
		case 1:
			inputChannel = 0; 						// Change input channel to potentiometer
			potmeter = ADC;
			break;
	}
	ADMUX = (ADMUX & 0b11110000) | inputChannel; 	// Changes input channel
}

/************************************************************************/
/* Main loop. Does something... ´\(o.o)/`                               */
/************************************************************************/
int main(void) {
	initADC();
	initTimer();
	initSerial();
	sei();
	uint16_t threshold = 500;
	while (1) {
		if (!(ADCSRA & (1<<ADSC))) { 	// If conversion is done
 			ADCSRA |= (1<<ADSC); 		// Start new conversion
    	}
		if (potmeter > threshold) {
			OCR0A = temperature/4;
			writeString("Temperature: ");
			writeNumber(temperature/4);
			writeString("\tChoose new threshold by writing: <number> with NULL termination\n");
		} else {
			OCR0A = 0;
		}
		if (UCSR0A & (1<<RXC0)) {		// If incoming data
			threshold = readNumber();
		}

	}
}
