//     0x00 sekundy
//     0x01 minuty
//     0x02 godziny
//     0x03 dni tygodnia
//     0x04 dni miesi¹ca 
//     0x05 miesi¹ce 
//     0x06 lata

#define TIME_ADDRESS   0x00    //wskazuje na pierwszy rejest pamiêci regara 
#define DATE_ADDRESS   0x04    //wskazuje na pi¹ty rejest pamiêci zegara
#define OPTION_ADDRESS   0x08
#define SAVED_TIME_ADDRESS 0x0C

#define DS1307_ID 0xD0 // adres modu³u DS1307_ID

char i, B1;

int sekunda, minuta, godzina, dzien, miesiac, rok, aa, bb, cc, dd, m, g, d, mi, r;


int RTC_SetTime(int godz, int min, int sek);
int RTC_SetDate(int dzien, int miesiac, int rok);
void RTC_GetTimeDate(void);
int RTC_SaveOptions(int x, int y, int z, int u);
void RTC_GetOptions(void);
void RTC_SaveTime(void);
void RTC_GetSavedTime(void);
