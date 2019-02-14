 #include <SoftwareSerial.h>
 
//SIM800 TX is connected to Arduino D8
#define SIM800_TX_PIN 8
 
//SIM800 RX is connected to Arduino D7
#define SIM800_RX_PIN 7
 
//Create software serial object to communicate with SIM800
SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);
String response;
String apn = "internet";                       //APN
String apn_u = "internet";                     //APN-Username
String apn_p = "internet";                     //APN-Password
String url = "ptsv2.com/t/8qeqn-1550145914/post";  //URL for HTTP-POST-REQUEST
String data1;   //String for the first Paramter (e.g. Sensor1)
String data2;   //String for the second Paramter (e.g. Sensor2)
 
void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  data1 = "elooo";
  data2 = "adasd";
  Serial.begin(9600);
  while(!Serial);
   
  //Begin serial communication with Arduino and SIM800
  serialSIM800.begin(9600);
  delay(1000);
  serialSIM800.write("ATE0\r");
  runs1();
  serialSIM800.write("AT+CMGF=1\r\n");
  runs1();
  delay(1000);
  serialSIM800.write("AT+CPIN=\"9303\"\r\n");
  runs1();
  delay(1000);

  Serial.println("Setup Complete!");
  //sendSMS();
  gsm_inithttp();
}
 
void loop() {
  gsm_sendthttp();
//  Read SIM800 output (if available) and print it in Arduino IDE Serial Monitor
//  if(serialSIM800.available()){
//    Serial.write(serialSIM800.read());
//  }
//  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
//  if(Serial.available()){    
//    serialSIM800.write(Serial.read());
//  }
}

void gsm_sendthttp()
{
  serialSIM800.println("AT+HTTPDATA=192,10000");
  Serial.println("AT+HTTPDATA=192,10000");
  runs1();
  delay(100);
  serialSIM800.println("params=" + data1 + "~" + data2);
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
}
void gsm_inithttp() {
  
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
  
 // serialSIM800.println("AT+HTTPTERM");
//  Serial.println("AT+HTTPTERM");
//  runs1(); 
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
  serialSIM800.write("To ja, super arduino, ktore nie umie jeszcze czytac");
  delay(1000);
   
  //Send Ctrl+Z / ESC to denote SMS message is complete
  serialSIM800.write((char)26);
  delay(1000);
     
  Serial.println("SMS Sent!");

}
