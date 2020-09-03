#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#include "ADC.h"
#include "USART.h"
#include "TWI.h"
#include "RTC.h"
#include "DHT11.h"

#define RxD_ON (1<<PD0)
#define TxD_ON (1<<PD1)
#define PLYWAK_PIN (1<<PB0) // Definicja, na który podawany jest sygna³ z czujnika
#define PLYWAK_DOWN !(PINB & PLYWAK_PIN) // Makro spradzaj¹ce czy wejœciu jest stan niski


// przetwarznie danych z telefonu
volatile char ramka[10];
volatile char wynik[5][3];
volatile int j = 0;
volatile char *p;

//dane do wys³ania do aplikacji mobilnej
char data[43];

// dane  przechowuj¹ce pomiary
int pomiar_w_g;
float w_g;
int bufor_w_g, poz_bat;
char p_w, p_b; //flagi wyra¿aj¹ce stan wody w zbiorniku i poziom naadowania baterii

//flagi zezwalaj¹ce na podlanie rosliny 
int ilosc_wody;
int zezwolenie_czasowe;
int czas_p_p; // czas pomiedzy podlewaniami


int dzien_buf;



int main(void)
{
DDRD |= (RxD_ON | TxD_ON);    // konfiguracja Uart
USART_Init(BAUD(9600));

TWI_Init();    // konfiguracja TWI

ADC_Init();    // konfiguracja przetwornika ADC



DDRD |= (1<<PD5); // piny, do których  pod³¹czona jest pompa
DDRD |= (1<<PD7);
PORTD &=~ (1<<PD7); 

// Konfiguracja TIMER1
TCCR1A |= (1<<WGM10);		// tryb Fast PWM 8-bit
TCCR1B |= (1<<WGM12);
TCCR1A |= ((1<<COM1A1)|(1<<COM1A0));
TCCR1B |= (1<<CS10);					// preskaler = 1
OCR1A=127;
	

DDRB &=~ PLYWAK_PIN;    // piny, do których pod³¹czony jest p³ywak
PORTB |= PLYWAK_PIN;

sei(); //globalne zezwolenie na przerwanie 
UCSRB |= (1<<RXCIE); // zezwolenie na prerwanie gdy RX odbierze jakieœ dane 

DDRB &=~(1<<PB1);  //wejscie sygna³u STATE z modu³u Bluetooth

//------LAMKI SYGNALIZACYJNE-----------

DDRB |=(1<<PB2); // dioda sygnalizuj¹ca po³¹czenie urz¹dzenia z aplikacj¹ mobiln¹ 
PORTB |=(1<<PB2); 

DDRB |=(1<<PB3); // dioda sygnalizuj¹ca pod³¹czenie zasilania urz¹dzenia 
PORTB &=~(1<<PB3);

DDRB |=(1<<PB4); // dioda sygnalizuj¹ca brak wody w zbiorniku
PORTB |=(1<<PB4);

DDRB |=(1<<PB5); // dioda sygnalizuj¹ca potrzebê pod³¹czenia ³adowarki 
PORTB |=(1<<PB5);



// wczytanie ustawieñ z pamiêci zewnêtrznej
RTC_GetOptions();
itoa(aa,wynik[0],10);

itoa(bb,wynik[1],10);

itoa(cc,wynik[2],10);

itoa(dd,wynik[3],10);


while(1)
{
//sygna³ STATE z modu³u Bluetooth
if (PINB &(1<<PB1))
{
PORTB &=~ (1<<PB2);
}
if (!(PINB &(1<<PB1)))
{
PORTB |= (1<<PB2);
}


DHT11_pomiar(); //pomiar wilgotnoœci i temperatury powietrza 

//pomiar wilgotnoœci gleby
ADC_pomiar(0); // odrzucam dwa pierwsze pomiary ze wzgledu na pojemnosc wejsciowa przetwornika
ADC_pomiar(0);
pomiar_w_g = ADC_pomiar(0); 
w_g = ((pomiar_w_g - 80)/6);
_delay_ms(100);

//pomiar stanu baterii
ADC_pomiar(1); // odrzucam dwa pierwsze pomiary ze wzgledu na pojemnosc wejsciowa przetwornika
ADC_pomiar(1);
poz_bat = ADC_pomiar(1);  
  
bufor_w_g = 100 - (int) w_g;
if (bufor_w_g >=100) bufor_w_g = 99;
if (bufor_w_g <=0) bufor_w_g = 0;

// Pomiar poziomu wody w zbiorniku
if ( PLYWAK_DOWN)
   {
     p_w = 1;            // Ustawinie flagi braku wody na 1
	 PORTB &=~ (1<<PB4); // Za³¹czenie lamki sygnalizuj¹cej brak wody w zbiorniku
   }
   else
   {
     p_w = 0;            // Ustawinie flagi braku wody na 0
	 PORTB |= (1<<PB4);  // Wy³¹czenie lamki sygnalizuj¹cej brak wody w zbiorniku
   }

if (poz_bat < 875)
   {
     p_b = 1;
	 PORTB &=~ (1<<PB5);
   }
if (poz_bat > 905)
   {
     p_b = 0;
	 PORTB |= (1<<PB5);
   }


//zezwolenie czasowe
if (wynik[1][0]=='0')
{
czas_p_p=0;
}
if (wynik[1][0]=='1')
{
czas_p_p=1;
}
if (wynik[1][0]=='2')
{
czas_p_p=2;
}
if (wynik[1][0]=='3')
{
czas_p_p=3;
}

RTC_GetTimeDate();

dzien_buf = (((dzien>>4) * 10)+(dzien & 0b00001111));
if ((czas_p_p ==0) || ((czas_p_p != 0 )&&(dzien_buf>=czas_p_p)) || (miesiac>0) || (rok>0))
{
 zezwolenie_czasowe=1;
}
else 
{
zezwolenie_czasowe=0;
}

RTC_GetOptions();
// Podlewanie automatyczne
if ((dd > bufor_w_g) &&( zezwolenie_czasowe==1) && (p_w == 0))   
{
   
  
  PORTD |= (1<<PD7); // Za³¹czanie pompy
  _delay_ms(2000);

  if (wynik[2][0] == '2')
  {
    _delay_ms(2000);
  }
  if (wynik[2][0] == '3')
  {
    _delay_ms(4000);
  }

 
PORTD &=~ (1<<PD7);// Wy³¹czanie pompy

wynik[4][0] = '0';   // Wykasowanie zezwolenia czasowego      
RTC_SetDate(0,0,0);  // Wyzerowanie up³ywu czasu od ostatniego podlania
RTC_SetTime(0,0,0);
}

// Podlewanie rêczne
if ((wynik[4][0] == '1') && (p_w == 0)) 
{
  
  PORTD |= (1<<PD7);
  _delay_ms(4000);

  if (wynik[2][0] == '2') 
  {
    _delay_ms(2800);
  }
  if (wynik[2][0] == '3')
  {
    _delay_ms(5200);
  }

PORTD &=~ (1<<PD7);
wynik[4][0] = '0';
RTC_SetDate(0,0,0);
RTC_SetTime(0,0,0);
}

//dane wysy³ane do aplikacji mobilnej
itoa(p_w,data,10);

*(data+1) = '^';
itoa(p_b,data+2,10);

*(data+3)='^';
if (T_CC < 10)
  {
    *(data+4)=' ';
    itoa(T_CC,data+5,10);
  } 
else 
  {
    itoa(T_CC,data+4,10);
  }
*(data+6)='^';
itoa(W_CC,data+7,10);

*(data+9)='^';
if (bufor_w_g < 10)
  {
    *(data+10)=' ';
    itoa(bufor_w_g,data+11,10);
  }
else 
  {
    itoa(bufor_w_g,data+10,10);
  }

*(data+12)='^';
*(data+13) = wynik[0][0];

*(data+14) = '^';
*(data+15) = wynik[1][0];

*(data+16) = '^';
*(data+17) = wynik[2][0];

*(data+18) = '^';
itoa(dd,data+19,10);

*(data+19) = wynik[3][0];
*(data+20) = wynik[3][1];

*(data+21)='^';
itoa(zezwolenie_czasowe,data+22,10);

*(data+23)='^';
if (minuta <= 9)
  {
    *(data+24)=' ';
    itoa(minuta,data+25,16);
  }
else 
  {
    itoa(minuta,data+24,16);
  }

*(data+26)='^';
if (godzina <= 9)
  {
    *(data+27)=' ';
    itoa(godzina,data+28,16);
  }
else 
  {
    itoa(godzina,data+27,16);
  }

*(data+29)='^';
if (dzien <= 9)
  {
    *(data+30)=' ';
    itoa(dzien,data+31,16);
  }
else 
  {
    itoa(dzien,data+30,16);
  }

*(data+32)='^';
if (miesiac <=9)
  {
    *(data+33)=' ';
    itoa(miesiac,data+34,16);
  }
else 
  {
    itoa(miesiac,data+33,10);
  }

*(data+35)='^';
itoa(poz_bat,data+36,10);
*(data+39)='^';
itoa(pomiar_w_g,data+40,10);

USART_PutString(data);




_delay_ms(1500);

}

}


//obs³uga danych przychodz¹cych z aplikacji mobilnej
ISR (USART_RXC_vect)
{

 USART_GetString(ramka);

 j=0;

 p = strtok(ramka, "^");
     while (p != NULL)
     {
         strcpy(wynik[j++],p);
         p = strtok(NULL, "^");
     }

 //prodedura zapisu danych do zewnêtrznej pamiêci
 int aabuf, bbbuf, ccbuf, ddbuf;
 aabuf=atoi(wynik[0]);
 bbbuf=atoi(wynik[1]);
 ccbuf=atoi(wynik[2]);
 ddbuf=atoi(wynik[3]);

 RTC_SaveOptions(aabuf,bbbuf,ccbuf,ddbuf);
}
