#include <avr/io.h>

#include "TWI.h"

#define ADRW_ACK 18
#define DATA_ACK 28

// Funkcja generująca sygnał startu komunikacji
void TWI_Start(void)
{
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
}

// Funkcja generująca sygnał stopu komunikacji
void TWI_Stop(void)
{
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
  while ((TWCR & (1<<TWSTO)));
}

// Funkcja realizująca zapis danych do urządzenia zewnętrznego
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

// Funkcja realizująca odczyt danych z urządzenia zewnętrznego
char TWI_Read(char ack)
{
  TWCR = ack 
	     ? ((1 << TWINT) | (1 << TWEN) | (1 << TWEA)) 
	     : ((1 << TWINT) | (1 << TWEN)) ;
  while (!(TWCR & (1<<TWINT)));
  return TWDR;
}

// Funkcja inicjalizująca komunikacją TWI
void TWI_Init(void)
{
  TWBR = 100;
}
