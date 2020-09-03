#define DHT11_PIN PD6

uint8_t c, W_CC, W_CD, T_CC, T_CD, CheckSum;  //W_CC  - wilgotnosc csesc calkowita, W_CD - wilgotnosc czesc dziesietna, T_CC - temteratura - czesc calkowita, T_CD - temperatura czesc dziesietna ;



void Request(void);
void Response(void);
int Receive_data(void);
