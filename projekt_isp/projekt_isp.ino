#include <DS1307.h>
#include <Wire.h>

char data[100];
#define pinCzujnikaRuchu 8                    // pin czujnika ruchu  

DS1307 zegar;
RTCDateTime dt;
boolean czujnik_ruchu_zazb = false;           // informacja o zazbrojeniu czujnika ruchu --> 0 - niezazbrojony, 1 - zazbrojony
boolean wykryto_ruch = false;                 // informacja o wykryciu ruchu przez czujnik ---> 0 - niewykryto, 1 - wykryto
unsigned long czasWykryciaRuchu = 10000;      // po wykryciu ruchu, zmienna wykryto_ruch jest ustawiana na True na 10 sekund
unsigned long start_czujnika = 0;
String lista_odbiorcow[5] = {"510312067", "0", "0", "0", "0"};

void czytaj_czujnik_ruchu(){
  
  int ruch = digitalRead(pinCzujnikaRuchu);
        
  if (ruch == HIGH && !wykryto_ruch)
  {   
      wykryto_ruch = true;
      
      start_czujnika = millis();

      sprintf(data, "Data: %d, %d, %d, Godzina: %d, %d, %d", dt.year, dt.month, dt.day, dt.hour - 1, dt.minute + 6, dt.second);
      Serial.print(data);  
      Serial.print(" ---> ");                            
      Serial.println("Wysylam smsa z alarmem!");
      // wyslij_smsa();
      // wyslij_info_na_serwer()
  }
  else if ((millis() - start_czujnika >= czasWykryciaRuchu) && wykryto_ruch) 
  {
      wykryto_ruch = false;  
      Serial.println("Zresetowalem zmienna 'wykryto_ruch', bo minelo 10 sekund");    
  }
}

void zazbrojenie_czujnika(){
  if (!czujnik_ruchu_zazb){
    czujnik_ruchu_zazb = true;
  }
}

void rozbrojenie_czujnika(){
  if (czujnik_ruchu_zazb){
    czujnik_ruchu_zazb = false;
  }
}

void zmien_czas_wykrycia(int nowy_czas_wykrycia){
  czasWykryciaRuchu = nowy_czas_wykrycia*1000;  
}

void dodaj_odbiorce(String nr_telefonu){
  
  int i = 0;
  
  for(i=0; i<5; i++){
    if (strcmp(lista_odbiorcow[i], "0") != 0)
       lista_odbiorcow[i] = nr_telefonu;
       i = 5;
  }
}

void usun_odbiorce(String nr_telefonu){

  int i = 0;
  
  for(i=0; i<5; i++){
    if (strcmp(lista_odbiorcow[i],nr_telefonu) != 0){
      lista_odbiorcow[i] = "0";
    }
  }
  
}
void setup(){
  //Inicjalizacja po resecie
  Serial.begin(9600);        
  pinMode(pinCzujnikaRuchu, INPUT);
   
  Serial.println("Zczytywane danych z czujnika ruchu"); 
  Serial.println(__TIME__);

  // Czujnik ruchu ustaw: w stan aktywny (zazbrojony), niewykryto ruchu, czas 10 sekund
  czujnik_ruchu_zazb = true;
  wykryto_ruch = false;
  czasWykryciaRuchu = 10000;

  //Wlacz zegar RTC
  zegar.begin();

  if (!zegar.isReady())
  {
    //Gdy zegar nie ma ustawione daty i godziny, ustaw tez z momentu kompilacji programu
    zegar.setDateTime(__DATE__, __TIME__);
  }
}

void loop(){

  dt = zegar.getDateTime();

  //zczytywanie danych z czujnika ruchu tylko wtedy, gdy jest zazbrojony
  if (czujnik_ruchu_zazb)
  {
      czytaj_czujnik_ruchu();
  }   
  delay(1000);                  
}
