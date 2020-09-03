#include <avr/io.h>

#include "ADC.h"

// Funkcja do konfiguracji ADC
void ADC_Init(void)
{
  ADMUX |= (1<<REFS0);   //wybor napiecia odniesienia
  ADCSRA |= (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0); // wlaczenie ADC i wybor preskalera (64)
}

//Funkcja realizuj¹ca pomiar na danym kanale ADC
int ADC_pomiar(int kanal)
{
  
     
  
  ADMUX = (ADMUX & 0xF8) | kanal; // ustawienie kanalu ADC w rejestrze ADMUX
  ADCSRA |= (1<<ADSC);            // start programu przez ustawienie bitu ADCS w rejestrze ADCSR
  while (ADCSRA & (1<<ADSC));     // oczekiwanie na koniec programu (dopuki ADSR = 1)
  return ADC;
}
