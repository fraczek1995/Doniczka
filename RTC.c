#include <avr/io.h>

#include "RTC.h"
#include "TWI.h"



int RTC_SetTime(int godz, int min, int sek)
{
TWI_Start();            // Start komunikacji TWI
 
TWI_Write(DS1307_ID);	 // po��czenie z modu�em zegara czasu rzeczywistego poprzez wys�anie jego adresu
TWI_Write(TIME_ADDRESS); // Wyb�r rejestru sekund
 
TWI_Write(((sek/10)<<4)+(sek%10));			
TWI_Write(((min/10)<<4)+(min%10));			
TWI_Write(((godz/10)<<4)+(godz%10));			
 
TWI_Stop();
return 0;            	// zako�czenie komunikacji TWI po dokaniu zmiany czasu
}


int RTC_SetDate(int dzien, int miesiac, int rok)
{
TWI_Start();            // Start komunikacji TWI
 
TWI_Write(DS1307_ID);	// po��czenie z modu�em zegara czasu rzeczywistego
TWI_Write(DATE_ADDRESS); // Wyb�r rejestru dni miesiaca
 
TWI_Write(((dzien/10)<<4)+(dzien%10));			
TWI_Write(((miesiac/10)<<4)+(miesiac%10));			
TWI_Write(((rok/10)<<4)+(rok%10));			
 
TWI_Stop();
return 0;           	// zako�czenie komunikacji TWI po dokaniu zmiany czasu
}


void RTC_GetTimeDate(void)
{
TWI_Start();                                    
TWI_Write(0xD0);                                
TWI_Write(0x00);                                
TWI_Start();                                    
TWI_Write(0xD1);                               

for(i = 0; i < 8; i++)
  {
  B1 = TWI_Read((i==7)?NACK:ACK);               
  if (i==0) sekunda=B1;
  if (i==1) minuta=B1;
  if (i==2) godzina=B1;
  if (i==4) dzien=B1;
  if (i==5) miesiac=B1;
  if (i==6) rok=B1;
  }

TWI_Stop();  
}

// Funkcja zapisu danych do pami�ci
int RTC_SaveOptions(int x, int y, int z, int u)
{
  TWI_Start();               // Start komunikacji TWI
 
  TWI_Write(DS1307_ID);	     // Po��czenie z modu�em zegara RTC 
  TWI_Write(OPTION_ADDRESS); // Wyb�r pierwszego adresu kom�rki pami�ci, gdzie maj� zosta� zapisane dane
  
  //zapis danych do pami�ci (dane zapisywane s� w kolejnych kom�rkach pami�ci)
  TWI_Write(x);			
  TWI_Write(y);			
  TWI_Write(z);
  TWI_Write(u);			
 
  TWI_Stop();  // Zako�czenie komunikacji TWI 
  return 0;            	
}

// Funkcja odczytu danych do pami�ci
void RTC_GetOptions(void)
{
  TWI_Start();               // Start komunikacji TWI
  TWI_Write(DS1307_ID);      // Po��czenie z modu�em zegara RTC
  TWI_Write(OPTION_ADDRESS); // Wyb�r pierwszego adresu kom�rki pami�ci, sk�d maj� zosta� odczytane dane
  TWI_Start();               // Powt�rzenie startu komunikacji
  TWI_Write(0xD1);           // Uruchomienie konfiguracji odczytu danych

  // P�tla realizuj�ca odczyt danych
  for(i = 0; i < 8; i++)
  {
    B1 = TWI_Read((i==7)?NACK:ACK);               
    if (i==0) aa=B1;
    if (i==1) bb=B1;
    if (i==2) cc=B1;
    if (i==3) dd=B1;
  }

  TWI_Stop(); // Zako�czenie komunikacji TWI  
}

void RTC_SaveTime(void)
{
RTC_GetTimeDate();

TWI_Start();            // Start komunikacji TWI
 
TWI_Write(DS1307_ID);	 // po��czenie z modu�em zegara czasu rzeczywistego poprzez wys�anie jego adresu
TWI_Write(SAVED_TIME_ADDRESS); 
 
TWI_Write(minuta);
TWI_Write(godzina);			
TWI_Write(dzien);			
TWI_Write(miesiac);
TWI_Write(rok);				
 
TWI_Stop();
     

}


void RTC_GetSavedTime(void)
{
TWI_Start();                                    
TWI_Write(0xD0);                                
TWI_Write(SAVED_TIME_ADDRESS);                             
TWI_Start();                                   
TWI_Write(0xD1);                                

for(i = 0; i < 8; i++)
  {
  B1 = TWI_Read((i==7)?NACK:ACK);               
  if (i==0) m=B1;
  if (i==1) g=B1;
  if (i==2) d=B1;
  if (i==3) mi=B1;
  if (i==4) r=B1;
  
  }

TWI_Stop();  
}
