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

String lista_odbiorcow[5] = {"000000000", "000000000", "000000000", "000000000", "000000000",};

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
  
  //Po inicjalizacji pracy programu wczytujemy liste odbiorcow przechowywana w pamieci nieulotnej RTC
  przepisz_odbiorcow();
 
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
  
  for(int i=0; i<5; i++){
    if (lista_odbiorcow[i] == "000000000"){
        Serial.println("Dodaje numer");
        lista_odbiorcow[i] = nr_telefonu;
        break;
    }
  }
  aktualizuj_odbiorcow();
}

void usun_odbiorce(String nr_telefonu){
  
  for(int i=0; i<5; i++){
    if (lista_odbiorcow[i] == nr_telefonu){
      Serial.println("Usuwam numer");
       lista_odbiorcow[i] = "000000000";
       i = 5;
    }
  }
  aktualizuj_odbiorcow();
  
}

void aktualizuj_odbiorcow(){

  delay(2000);
  for (byte i = 0; i<5; i++){
    for (byte j = 0; j < 9; j++){
      zegar.writeByte(j+(i*9), lista_odbiorcow[i][j]);
    }
  }
}

void przepisz_odbiorcow(){
   for (int i = 0; i<5; i++){
    for (byte j = 0; j < 9; j++){
      lista_odbiorcow[i][j] = zegar.readByte(j+(i*9));
    }
   }
}

void odczyt_pamieci(){
  char tmp[16];
  for (byte j = 0; j < 4; j++)
  {
    sprintf(tmp, " 0x%.2X : ", (j*16), 2);
 
    Serial.print(tmp);
 
    for (byte i = 0; i < 16; i++)
    {
      if ((j*16 + i) > 55)
      {
        break;
      }
 
      sprintf(tmp, "0x%.2X ", zegar.readByte(j*16 + i), 2);
 
      Serial.print(tmp);
    }
 
    Serial.println();
  } 
}

void odczyt_listy(){

  for(int i=0; i<5; i++){
      Serial.println(lista_odbiorcow[i]);
  }
}
