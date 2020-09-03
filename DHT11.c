#include <avr/io.h>
#include "DHT11.h"
#include <util/delay.h>




// Funkcja realizuj¹ca wys³anie przez mikrokontroler impulsu staru/zapytania
void Request()				
{
	DDRD |= (1<<DHT11_PIN);
	PORTD &= ~(1<<DHT11_PIN);	// Ustawinie stanu niskiego
	_delay_ms(20);			    // Odczekanie 20 ms
	PORTD |= (1<<DHT11_PIN);	// Ustawinie stanu wysokiego
}

// Funkcja odczytuj¹ca impuls odpowiedzi od czujnika DHT11
void Response()			
{
	DDRD &= ~(1<<DHT11_PIN);
	while(PIND & (1<<DHT11_PIN));
	while((PIND & (1<<DHT11_PIN))==0);
	while(PIND & (1<<DHT11_PIN));
}

// Funkcja realizuj¹ca odbiór danych z czujnika DHT11
int Receive_data()			
{	
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0);  // Sprawdzanie czy otrzymany bit jest 0 czy 1
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN))             // Jeœli stan wysoki trwa d³u¿ej ni¿ 30 us
		c = (c<<1)|(0x01);	                  // to jest to logiczna 1
		else			                      // jesli nie to jest to logiczne 0
		c = (c<<1);
		while(PIND & (1<<DHT11_PIN));
	}
	return c;
}


void DHT11_pomiar(void)
{
    Request();		        // wys³anie impulsu staru
	Response();		        // odbiór impulsu odpowiedzi od czujnika DHT11
	W_CC=Receive_data();	// Zapis pierwszych 8 bitów do zmiennej W_CC
	W_CD=Receive_data();	// Zapis kolejnych 8 bitów do zmiennej W_CD 
	T_CC=Receive_data();	// Zapis kolejnych 8 bitów do zmiennej T_CC
	T_CD=Receive_data();	// Zapis kolejnych 8 bitów do zmiennej T_CD
	CheckSum=Receive_data();// Zapis kolejnych 8 bitów do zmiennej CheckSum 

}
