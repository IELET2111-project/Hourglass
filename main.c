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

volatile uint16_t inputValue;		// Stores last ADC conversion from the ADC Data Register (0 - 1023)
volatile uint8_t inputChannel = 0;	// Stores current analog input channel (ADC0, ADC1...)

/************************************************************************/
/* Initialization of analog-to-digital converter. Prescaler is set to:. */
/* Interrupts is enabled. Reference voltage is set to AVCC. ADC0 is the */
/* initial input channel.                                               */
/************************************************************************/
void initADC() {
	ADCSRA |= (1<<ADEN);										// Enable ADC
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS1);				// Configure prescaler
	ADCSRA |= (1<<ADIE);										// Configure interrupt
	ADMUX |= (0<<REFS1) | (1<<REFS0);							// Set reference to AVCC
	ADMUX |= (1<<ADLAR);										// Configure left adjusted result
	ADMUX |= (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0);		// Configure ADC0 as input channel
}

/************************************************************************/
/* Interrupt service routine for analog-to-digital converter. ISR is    */
/* triggered on finished conversion. Saves conversion and toggles input */
/* channel. Switch statement can be replaced by:						*/
/* inputChannel = !inputChannel; if only ADC0 and ADC1 is used.         */
/************************************************************************/
ISR(ADC_vect) {
	inputValue = ADC;
	inputChannel = ADMUX & 0b00001111;
	switch (inputChannel) {
		case 0:
			inputChannel = 1; // Change input channel to temperature sensor
			break;
		case 1:
			inputChannel = 0; // Change input channel to potentiometer
			break;
	}
<<<<<<< HEAD
	ADMUX = (ADMUX & 0b11110000) | inputChannel; // Changes input channel
=======
	
}

uint16_t readADC(void)
{
	//Leser ADC-verdi
	ADCSRA |= (1 << ADSC);
	//Kjører til ADCS-bit er slettet
	loop_until_bit_is_clear(ADCSRA, ADSC);
	//leser lavt register før det høye
	return ADCL | (ADCH<<8); 
>>>>>>> parent of 3ba4fea... Update main.c
}

/************************************************************************/
/* Main loop. Does something... ´\(o.o)/`                               */
/************************************************************************/
int main(void) {
	initADC();
	initSerial();
	sei();

	while (1) {
		if (!(ADCSRA & (1<<ADSC))) { 	// If conversion is done
 			ADCSRA |= (1<<ADSC); 		// Start new conversion
    	}
		switch (inputChannel) {
			case 0:
				// Pot code
				break;
			case 1:
				// Temp code
				break;
		}
	}
}


//Ting jeg kan ha føkket til:
//Usikker på om AV_CC = 1 er riktig for dette formålet.
//Usikker på om ADPS1 = 1 og ADPS0 = 1 er riktig. Sliter veldig med alt som har med klokker og frekvenser å gjøre. Vet ikke
//om dette vil føkke til noe med tanke på prescaling og klokkefrekvens osv.
//Vet ikke om det er nødvendig med 10 bits og skifting av resultatbits, eller om det holder å bare lese av ett register.
//Vet ikke om nødvendig med default: break: i switch-case.

//Initialiersing og setup for ADC
//ADEN = 1 skrur på ADC
//ADPS1 = 1 og ADPS0 = 1 velger prescaler bits. Her velges division factor lik 8.
//I tillegg sørger dette for at ADLAR = 1, dvs avlesningen av ADC blir lagret med de 8 MSB i ADCH, og de 2 LSB i ADCL.
//ADLAR = 1 betyr å venstreskifte resultatbitsene
//MUX0 = 1 velger ADC1 som analog input til ADCen.
//ADIE = 1 tillater interrupt for ADC.
//ADCSRB = 0 betyr freerunning mode aktivert. Dette gjør at kommunikasjonen automatisk gjenopptas med en gang en samtale avsluttes.

// static inline void initADC0(void) {
// 	//External voltage AVCC, velger ADC1 som output, og venstreshifter resultatbitsene ved å sette ADLAR til 1
// 	ADMUX |= (1 << REFS0)|(1 << ADLAR)|(1 << MUX0);
// 	//Venstreshifter resultatbitsene ved å sette ADLAR til 1, enabler ADIE (ADC interrupt)
// 	ADCSRA |= (1 << ADIE)|(1 << ADPS1)|(1 << ADPS0);
// 	//Enable AVC
// 	ADCSRA |= (1 << ADEN);
// 	//Enable free running mode
// 	ADCSRB |= (1 << 0x00);
// 	//Må manuelt starte første ADC-konvertering
// 	ADCSRA |= (1 << ADSC);
// }

// void tempFunction() {
// 	//Hvis temperaturen er avlest:
// 	if (tempVal == 1) {
// 		//kode her
// 		//kode her
// 		tempVal = 0;
// 	}
//
// }
//
// void potFunction() {
// 	//Hvis potensiometeret er avlest:
// 	if (potVal == 1) {
// 			//Ønsker å starte motoren dersom pot-verdien er over 75% av maksverdi.
// 		if (adcVal >767) {
// 				//Starte motor
// 				//kode her
// 				//kode her
// 		}
// 		potVal = 0;
// 	}
// }
