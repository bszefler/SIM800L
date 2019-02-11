
#define pinCzujnikaRuchu 8                   // pin czujnika ruchu  
boolean czujnik_ruchu_zazb = false;           // informacja o zazbrojeniu czujnika ruchu --> 0 - niezazbrojony, 1 - zazbrojony
boolean wykryto_ruch = false;                 // informacja o wykryciu ruchu przez czujnik ---> 0 - niewykryto, 1 - wykryto
unsigned long czasWykryciaRuchu = 10000;      // po wykryciu ruchu, zmienna wykryto_ruch jest ustawiana na True na 10 sekund
unsigned long start_czujnika = 0;


void czytaj_czujnik_ruchu(){
  
  int ruch = digitalRead(pinCzujnikaRuchu);
        
  if (ruch == HIGH && !wykryto_ruch)
  {   
      wykryto_ruch = true;
      
      start_czujnika = millis();
                                        
      Serial.println("Wysylam smsa z alarmem. Zmienna 'wykryto_ruch' jest ustawiana na TRUE na czas 10 sekund");
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

void setup(){
  //Inicjalizacja po resecie
  Serial.begin(9600);        
  pinMode(pinCzujnikaRuchu, INPUT);   
  
  Serial.println("Zczytywane danych z czujnika ruchu"); 

  // Czujnik ruchu ustaw: w stan aktywny (zazbrojony), niewykryto ruchu, czas 10 sekund
  czujnik_ruchu_zazb = true;
  wykryto_ruch = false;
  czasWykryciaRuchu = 10000;
}

void loop(){

  //zczytywanie danych z czujnika ruchu
  if (czujnik_ruchu_zazb)
  {
      czytaj_czujnik_ruchu();
  }                     
}
