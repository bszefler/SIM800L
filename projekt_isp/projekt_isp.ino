
int czujnik = 8;            // pin czujnika ruchu  

void setup(){
  Serial.begin(9600);        
  pinMode(czujnik, INPUT);   
  
  Serial.println("Zczytywane danych z czujnika ruchu"); 
}

void loop(){

  //zczytywanie danych z czujnika ruchu
  
  int ruch = digitalRead(czujnik);
        
  if(ruch == HIGH)                      
  {                                     
      Serial.println("RUCH WYKRYTY!");
  }
  else  
  {
      Serial.println("brak ruchu");
  }
  
  delay(200);                         
}
