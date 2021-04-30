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
//ADEN = 1 skrur p� ADC
//ADPS1 = 1 og ADPS0 = 1 velger prescaler bits. Her velges division factor lik 8.
//I tillegg s�rger dette for at ADLAR = 1, dvs avlesningen av ADC blir lagret med de 8 MSB i ADCH, og de 2 LSB i ADCL.
//ADLAR = 1 betyr � venstreskifte resultatbitsene 
//MUX0 = 1 velger ADC1 som analog input til ADCen.
//ADIE = 1 tillater interrupt for ADC.
//ADCSRB = 0 betyr freerunning mode aktivert. Dette gj�r at kommunikasjonen automatisk gjenopptas med en gang en samtale avsluttes.

static inline void initADC0(void) {
	//External voltage AVCC, velger ADC1 som output, og venstreshifter resultatbitsene ved � sette ADLAR til 1 
	ADMUX |= (1 << 0x61); 
	//Venstreshifter resultatbitsene ved � sette ADLAR til 1, enabler ADIE (ADC interrupt) 
	ADCSRA |= (1 << 0x0B); 
	//Enable AVC 
	ADCSRA |= (1 << ADEN); 
	//Enable free running mode 
	ADCSRB |= (1 << 0x00); 
	//M� manuelt starte f�rste ADC-konvertering 
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
			//�nsker � starte motoren dersom pot-verdien er over 75% av maksverdi.
			if (ADC >767){
				
				//Starte motor
				//kode her
				//kode her
			}
		potVal = 0;
	}
	
}


int main(void)
{
	//Kj�r init. funksjon
	initADC0();
	//Set global interrupt enable bit 
	sei();
	//Starter avlesning av ADC
	ADCSRA |= (1 << ADSC); 
	

	tempFunction();
	potFunction();
	
		
	while (1)
	{	
	}
}

	
//Interrupt function. 
ISR(ADC_vect){
		
	switch (ADMUX)		//Switch-case: leser av verdien til ADMUX n�r ADC interrupt trigges
	{
		case 0x61:		//Dersom ADMUX = 0x61, betyr dette f.eks. temperatursensorpinne. Skriver kode under hva vi �nsker at skal skje
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
	//Hindrer interrupt i � trigges f�r neste loop i main-l�kken.
	cli();
}


//Ting jeg kan ha f�kket til: 
//Usikker p� om AV_CC = 1 er riktig for dette form�let.
//Usikker p� om ADPS1 = 1 og ADPS0 = 1 er riktig. Sliter veldig med alt som har med klokker og frekvenser � gj�re. Vet ikke
//om dette vil f�kke til noe med tanke p� prescaling og klokkefrekvens osv. 
//Vet ikke om det er n�dvendig med 10 bits og skifting av resultatbits, eller om det holder � bare lese av ett register.
//Vet ikke om n�dvendig med default: break: i switch-case.


