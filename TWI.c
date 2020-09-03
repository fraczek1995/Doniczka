#include <avr/io.h>

#include "TWI.h"

#define ADRW_ACK 18
#define DATA_ACK 28

// Funkcja generuj�ca sygna� startu komunikacji
void TWI_Start(void)
{
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
}

// Funkcja generuj�ca sygna� stopu komunikacji
void TWI_Stop(void)
{
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
  while ((TWCR & (1<<TWSTO)));
}

// Funkcja realizuj�ca zapis danych do urz�dzenia zewn�trznego
char TWI_Write(char data)
{
  TWDR = data;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  if((TWSR == ADRW_ACK) | (TWSR == DATA_ACK))
    return 0;
  else
    return 1;
}

// Funkcja realizuj�ca odczyt danych z urz�dzenia zewn�trznego
char TWI_Read(char ack)
{
  TWCR = ack 
	     ? ((1 << TWINT) | (1 << TWEN) | (1 << TWEA)) 
	     : ((1 << TWINT) | (1 << TWEN)) ;
  while (!(TWCR & (1<<TWINT)));
  return TWDR;
}

// Funkcja inicjalizuj�ca komunikacj� TWI
void TWI_Init(void)
{
  TWBR = 100;
}
