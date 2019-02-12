 #include <SoftwareSerial.h>
 
//SIM800 TX is connected to Arduino D8
#define SIM800_TX_PIN 8
 
//SIM800 RX is connected to Arduino D7
#define SIM800_RX_PIN 7
 
//Create software serial object to communicate with SIM800
SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);
String response;
 
void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  while(!Serial);
   
  //Begin serial communication with Arduino and SIM800
  serialSIM800.begin(9600);
  delay(1000);
  serialSIM800.write("ATE0\r");

  while(!serialSIM800.available());
  Serial.print(serialSIM800.readStringUntil('\n'));
  while(!serialSIM800.available());
  response = serialSIM800.readStringUntil('\n');
  if(response == "OK")
    Serial.println("Wyłączono echo");
  else
    Serial.println("Błąd konfiguracji");
  serialSIM800.write("AT+CMGF=1\r\n");
  delay(1000);
  while(!serialSIM800.available());
  Serial.print(serialSIM800.readStringUntil('\n'));
  while(!serialSIM800.available());
  response = serialSIM800.readStringUntil('\n');
  if(response == "OK")
    Serial.println("Tryb wiadomości w ASCII");
  else
    Serial.println("Błąd konfiguracji");
  
  serialSIM800.write("AT+CPIN=\"9303\"\r\n");
  delay(1000);
  while(!serialSIM800.available());
  Serial.print(serialSIM800.readStringUntil('\n'));
  while(!serialSIM800.available());
  response = serialSIM800.readStringUntil('\n');
  if(response == "OK")
    Serial.println("PIN poprawny, gotowy do wyslania SMS");
  else
    Serial.println("Błąd konfiguracji");

  Serial.println("Setup Complete!");
}
 
void loop() {
//  Read SIM800 output (if available) and print it in Arduino IDE Serial Monitor
  if(serialSIM800.available()){
    Serial.write(serialSIM800.read());
  }
  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
  if(Serial.available()){    
    serialSIM800.write(Serial.read());
  }
}

boolean sendSMS()
{
  Serial.println("Sending SMS...");
  serialSIM800.write("AT+CMGS=\"510312067\"\r\n");
  delay(1000);

  //Send SMS content
  serialSIM800.write("To ja, super arduino, ktore nie umie jeszcze czytac");
  delay(1000);
   
  //Send Ctrl+Z / ESC to denote SMS message is complete
  serialSIM800.write((char)26);
  delay(1000);
     
  Serial.println("SMS Sent!");

}
