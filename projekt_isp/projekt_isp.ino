#include <DS1307.h>
#include <Wire.h>
#include <SoftwareSerial.h>

char data[100];
#define pinCzujnikaRuchu 9          // pin czujnika ruchu                                     
#define SIM800_TX_PIN 8             //SIM800 TX is connected to Arduino D8                                    
#define SIM800_RX_PIN 7             //SIM800 RX is connected to Arduino D7

DS1307 zegar;
RTCDateTime dt;
boolean czujnik_ruchu_zazb = false;           // informacja o zazbrojeniu czujnika ruchu --> 0 - niezazbrojony, 1 - zazbrojony
boolean wykryto_ruch = false;                 // informacja o wykryciu ruchu przez czujnik ---> 0 - niewykryto, 1 - wykryto
unsigned long czasWykryciaRuchu = 30000;      // po wykryciu ruchu, zmienna wykryto_ruch jest ustawiana na True na 10 sekund
unsigned long start_czujnika = 0;

String lista_odbiorcow[5] = {"000000000", "000000000", "000000000", "000000000", "000000000",};

//Create software serial object to communicate with SIM800
SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);
String response;
String apn = "internet";                       //APN
String apn_u = "internet";                     //APN-Username
String apn_p = "internet";                     //APN-Password
String url = "http://ptsv2.com/t/e35py-1550165945/post";  //URL for HTTP-POST-REQUEST

String data1;   //String for the first Paramter (e.g. Sensor1)
String data2;   //String for the second Paramter (e.g. Sensor2)
char sms[140];
char bufor[50];
int k = 0;
char h;
String str;

void setup(){
  //Inicjalizacja po resecie
  Serial.begin(9600);        
  pinMode(pinCzujnikaRuchu, INPUT);

   serialSIM800.begin(9600);
  delay(1000);
  serialSIM800.write("ATE0\r\n");
  delay(1000);
  serialSIM800.write("AT+CPIN=\"9303\"\r\n");
  while(!serialSIM800.available())
  //delay(1000);
  serialSIM800.write("AT+CMGF=1\r\n");
//  runs1();
  delay(1000);
  //sendSMS();
  serialSIM800.write("AT+CMGF=1\r\n");
  delay(1000);
  serialSIM800.println("AT+CNMI=2,2,0,0,0");
  
 
  Serial.println("Setup Complete!");
   
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
  aktualizuj_odbiorcow();
  przepisz_odbiorcow();
  odczyt_pamieci();

  //Inicjalizacja modulu GSM
   //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
   
  //Begin serial communication with Arduino and SIM800
 
 // readSMS();
 
}

void loop(){

  dt = zegar.getDateTime();
  
  //zczytywanie danych z czujnika ruchu tylko wtedy, gdy jest zazbrojony
  if (czujnik_ruchu_zazb)
  {
      czytaj_czujnik_ruchu();
  } 
  delay(100);    

  //gsm_sendthttp();
  //  Read SIM800 output (if available) and print it in Arduino IDE Serial Monitor
  
//  if(serialSIM800.available()){
//    //Serial.write(serialSIM800.read());
//    h = serialSIM800.read();
//    Serial.write(h);
//  }
//  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
//  if(Serial.available()){    
//    serialSIM800.write(Serial.read());
//  }

                
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

      data1 = data;
      gsm_sendthttp();
      sendSMS();
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
void check_response()
{
  int i = 0;
  int j = 0;
  while(serialSIM800.available())
  {
    bufor[i] = serialSIM800.read();
   // Serial.print(bufor);
    i++;
  }
 str[0] =  bufor[0];
 str[1] =  bufor[1];
 str[2] =  bufor[2];
 if(str == "485")
 {
  while(bufor[i] != '\n') i++;
  while(bufor[i] != '\0')
  {
    sms[j++] = bufor[i];
    i++;
  }
 }
 else Serial.println("NIE wszedlem");
 Serial.println(sms);
}

void gsm_sendthttp()
{ 
  serialSIM800.println("AT");
  Serial.println("AT");
  runs1();//Print GSM Status an the Serial Output;
  delay(4000);
  serialSIM800.println("AT+SAPBR=3,1,Contype,GPRS");
  Serial.println("AT+SAPBR=3,1,Contype,GPRS");
  runs1();
  delay(100);
  serialSIM800.println("AT+SAPBR=3,1,APN," + apn);
  Serial.println("AT+SAPBR=3,1,APN," + apn);
  runs1();
  delay(100);
  serialSIM800.println("AT+SAPBR=3,1,USER," + apn_u); //Comment out, if you need username
  runs1();
  delay(100);
  serialSIM800.println("AT+SAPBR=3,1,PWD," + apn_p); //Comment out, if you need password
  runs1();
  delay(100);
  serialSIM800.println("AT+SAPBR =0,1");
  Serial.println("AT+SAPBR =0,1");
  delay(2000);
  runs1();
  serialSIM800.println("AT+SAPBR =1,1");
  Serial.println("AT+SAPBR =1,1");
  runs1();
  delay(100);
  serialSIM800.println("AT+SAPBR=2,1");
  Serial.println("AT+SAPBR=2,1");
  runs1();
  delay(2000);
  serialSIM800.println("AT+HTTPINIT");
  Serial.println("AT+HTTPINIT");
  runs1();
  delay(100);
  serialSIM800.println("AT+HTTPPARA=CID,1");
  Serial.println("AT+HTTPPARA=CID,1");
  runs1();
  delay(100);
  serialSIM800.println("AT+HTTPPARA=URL," + url);
  Serial.println("AT+HTTPPARA=URL," + url);
  runs1();
  delay(100);
  serialSIM800.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  Serial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  runs1();
  delay(100);

  serialSIM800.println("AT+HTTPDATA=192,10000");
  Serial.println("AT+HTTPDATA=192,20000");
  runs1();
  delay(100);
  //root.printToSerial(serialSIM800);
  data1 = "wykryto ruch";
  serialSIM800.println("params=" + data1);
  runs1();
  delay(10000);
  serialSIM800.println("AT+HTTPACTION=1");
  Serial.println("AT+HTTPACTION=1");
  runs1();
  delay(5000);
  serialSIM800.println("AT+HTTPREAD");
  Serial.println("AT+HTTPREAD");
  runs1();
  delay(100);
  
  serialSIM800.println("AT+HTTPTERM");
  Serial.println("AT+HTTPTERM");
  runs1(); 
  //Serial.println("Sent");
}

void runs1() {
  while (serialSIM800.available()) {
    Serial.write(serialSIM800.read());
  }

}
void sendSMS()
{
  Serial.println("Sending SMS...");
  serialSIM800.write("AT+CMGS=\"510312067\"\r\n");
  delay(1000);

  //Send SMS content
  serialSIM800.write("Wykryto ruch");
  delay(1000);
   
  //Send Ctrl+Z / ESC to denote SMS message is complete
  serialSIM800.write((char)26);
  delay(1000);
     
  Serial.println("SMS Sent!");
}
