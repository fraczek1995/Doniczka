#include <avr/io.h>

// Funkcja inicjalizuj�ca interfejs USART
void USART_Init(int baud)       
{
  UBRRH = (unsigned char)(baud>>8);
  UBRRL = (unsigned char)baud;
  UCSRB = (1<<RXEN)|(1<<TXEN);
  UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

// Funkcja realizuj�ca wys�anie pojedynczego znaku
void USART_PutChar(unsigned char data)  
{
  while (!(UCSRA & (1 << UDRE)));
  UDR = data;
}

// Funkcja realizuj�ca odbi�r pojedynczego znaku
unsigned char USART_GetChar(void)   
{
  while (!(UCSRA & (1 << RXC)));
  return UDR;
}

// Funkcja realizuj�ca wys�anie ci�gu znak�w
void USART_PutString(char * s)  
{
  while(*s)
	USART_PutChar(*s++);
}

// Funkcja realizuj�ca odbi�r ci�gu znak�w
void USART_GetString(char * s) 
{
  char c;
  do{
    c = USART_GetChar();
    USART_PutChar(c);
    if(c=='\b')   // if backspace
      s--;
    else     
      *s++ = c;
    if(c == '\n') // if CR
      *s = 0;
    }
  while(c != '\n');
}

