/*
 * GccApplication1.c
 *
 * Created: 22.04.2021 15.06.48
 * Author : Bruker
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL //system clock
#include <util/delay.h> 
#include <avr/interrupt.h>
volatile uint8_t potVal = 0;
volatile uint8_t tempVal = 0;

//Initialiersing og setup for ADC
//ADEN = 1 skrur på ADC
//ADPS1 = 1 og ADPS0 = 1 velger prescaler bits. Her velges division factor lik 8.
//I tillegg sørger dette for at ADLAR = 1, dvs avlesningen av ADC blir lagret med de 8 MSB i ADCH, og de 2 LSB i ADCL.
//ADLAR = 1 betyr å venstreskifte resultatbitsene 
//MUX0 = 1 velger ADC1 som analog input til ADCen.
//ADIE = 1 tillater interrupt for ADC.
//ADCSRB = 0 betyr freerunning mode aktivert. Dette gjør at kommunikasjonen automatisk gjenopptas med en gang en samtale avsluttes.

static inline void initADC0(void) {
	//External voltage AVCC, velger ADC1 som output, og venstreshifter resultatbitsene ved å sette ADLAR til 1 
	ADMUX |= (1 << REFS0)|(1 << ADLAR)|(1 << MUX0); 
	//Venstreshifter resultatbitsene ved å sette ADLAR til 1, enabler ADIE (ADC interrupt) 
	ADCSRA |= (1 << ADIE)|(1 << ADPS1)|(1 << ADPS0); 
	//Enable AVC 
	ADCSRA |= (1 << ADEN); 
	//Enable free running mode 
	ADCSRB |= (1 << 0x00); 
	//Må manuelt starte første ADC-konvertering 
	ADCSRA |= (1 << ADSC);
}


void tempFunction() {
	//Hvis temperaturen er avlest:
	if (tempVal == 1)
	{
		//kode her
		//kode her
		
		tempVal = 0;
	}
		
}

void potFunction() {
	//Hvis potensiometeret er avlest:
	if (potVal == 1)
		{	
			//Ønsker å starte motoren dersom pot-verdien er over 75% av maksverdi.
			if (adcVal >767){
				
				//Starte motor
				//kode her
				//kode her
			}
		potVal = 0;
	}
	
}

uint16_t readADC(void)
{
	//Leser ADC-verdi
	ADCSRA |= (1 << ADSC);
	//Kjører til ADCS-bit er slettet
	loop_until_bit_is_clear(ADCSRA, ADSC);
	//leser lavt register før det høye
	return ADCL | (ADCH<<8); 
}



int main(void)
{
	//Kjør init. funksjon
	initADC0();
	//Set global interrupt enable bit 
	sei();
	
	uint16_t adcVal;
	
		
	while (1)
	{	
		adcVal = readADC();
		
		tempFunction();
		potFunction();
	}
}

	
//Interrupt function. 
ISR(ADC_vect){
		
	switch (ADMUX)		//Switch-case: leser av verdien til ADMUX når ADC interrupt trigges
	{
		case 0x61:		//Dersom ADMUX = 0x61, betyr dette f.eks. temperatursensorpinne. Skriver kode under hva vi ønsker at skal skje
			tempVal = 1;
			ADMUX = 0x63;
			break;
		
		case 0x63:		//Dersom ADMUX = 0x63, betyr dette at potensiometeret har blitt trigget. Skriver hva vi vil at skal skje.
			potVal = 1;
			ADMUX = 0x61; //Etter hver loop av funksjonen, blir ADMUX togglet slik at ADC leser av den andre pinnen neste gang.
			break;
		default:
		break;
	}
	//Hindrer interrupt i å trigges før neste loop i main-løkken.
	cli();
}


//Ting jeg kan ha føkket til: 
//Usikker på om AV_CC = 1 er riktig for dette formålet.
//Usikker på om ADPS1 = 1 og ADPS0 = 1 er riktig. Sliter veldig med alt som har med klokker og frekvenser å gjøre. Vet ikke
//om dette vil føkke til noe med tanke på prescaling og klokkefrekvens osv. 
//Vet ikke om det er nødvendig med 10 bits og skifting av resultatbits, eller om det holder å bare lese av ett register.
//Vet ikke om nødvendig med default: break: i switch-case.


