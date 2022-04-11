// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h> 

TinyGPSPlus gps;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
SoftwareSerial GSM(0,1);
SoftwareSerial gpsSerial(10,11);
#define mot A2
#define key 12 
const int Switch=8;
const int Sensor=9;
int Buzzer=13;
String lattitude,longitude;
String message; //String variable for storing incomming message
int con=0;      //for Counting 
int i=0;        //for counting
boolean ext=true; // boolean value for exit from loop
int j; //for gps
int k; //for gps


void setup() {
  pinMode(Sensor,INPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(mot,OUTPUT);
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  gpsSerial.begin(9600); // for GPS
  delay(1000);
  
}

void loop() {
  // Print a message to the LCD.  
    if(digitalRead(Switch)==HIGH)
    {     
        lcd.setCursor(0, 0);
        lcd.print("Security System ");
        lcd.setCursor(0, 1);
        lcd.print("       ON       ");
        digitalWrite(mot,HIGH);

      if(digitalRead(Sensor)==HIGH)      //Set vibration detection threshould 500
      {
          // Print a message to the LCD.
          lcd.setCursor(0, 0);
          lcd.print("Theft Detected  ");
          lcd.setCursor(0, 1);
          lcd.print("Sending SMS.....");
          
          //Turn on Alarm here
          digitalWrite(13,HIGH);    //Turn on Alarm connect buzzer to this pin through transistor
          
          sendSMS();
          delay(500);
          get_gsm();
          delay(500);
          sms();
      }
      else
      {
        digitalWrite(13,LOW);    //Turn off alarm
      }
    }
    else
    {
        lcd.setCursor(0, 0);
        lcd.print("Security System ");
        lcd.setCursor(0, 1);
        lcd.print("   Deactivated    ");
    }
}

void sendSMS()
{
     Serial.println("AT+CMGD=1");    //Delete privious sent SMS
     delay(100);
          
     Serial.println("AT+CMGF=1");   //Set SMS configuration
     delay(100);
     
     Serial.print("AT+CMGW=");          //Write New SMS
     Serial.write(34);                  //Double quotes ASCII Code
     Serial.print("+91**********");         //Enter Your Mobile number
     Serial.write(34);
     Serial.println();                  //Send Crrige return
     delay(400);
     
     Serial.println("Alert : Theft Detected");
     delay(100);

     Serial.write(26); //Cntrl+Z
     delay(100);
     delay(100);

     Serial.println("AT+CMSS=1");      //Send SMS from memory location 1
     delay(100);
}
void get_gsm() //function for reading GPS data and sending theft location through SMS
{
  for(k=0; k<50;k++){
    for(j=0; j<60;j++){
      gps.encode(gpsSerial.read());
      lattitude = String(gps.location.lat(),6);
      longitude = String(gps.location.lng(),6);
    }
    delay(10);
  }


  
  Serial.println("AT+CMGD=1");    //Delete privious sent SMS
  delay(1000);
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1 second
  Serial.println("AT+CMGS=\"+91xxxxxxxxx\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.print("Theft Detected: ");
  Serial.print("http://maps.google.com/maps?q=loc:");
  Serial.print(lattitude);
  Serial.print(",");
  Serial.print(longitude);
  delay(200);
  Serial.print("  ");
  
  Serial.print("If you want to stop the vehicle ");
  Serial.print("Send SMS STOP");
  delay(100);
  Serial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
  delay(3000);
  lcd.clear();
  lcd.print("Message Sent");
   
}
void sms() {
  while(i<=25){
    if(Serial.available()>0)
    {
      message = Serial.readString();
      delay(10);
    }
    if(message.indexOf("STOP") > -1)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("**SMS received**");
      delay(10);
      Serial.println("AT+CMGD=1,4"); //command for deleting all messages
      delay(10);
      while(ext){
        digitalWrite(key, LOW);
        digitalWrite(mot, LOW);
        if(con==0){
          delay(10);
          upsms();           // function
          con=30;
        }
        else{
          lcd.setCursor(0,0);
          lcd.print("Updating GPS....");
          con=con-1;
          delay(10);
          
        }
        delay(500);
        if(digitalRead(Switch)!=HIGH){
          ext=false;
        }
      }
      
    }
    i++;
    delay(1000);
    if(digitalRead(key)!=HIGH)
    {  //opopoopopopopo
      digitalWrite(mot,LOW);
    }
    else if(digitalRead(key)==HIGH)
    {
      digitalWrite(mot,HIGH);
    }
  }
  i=0;
}
void upsms(){   //Function for sending position of theft after turn off the engine

  for(k=0; k<50;k++){
    for(j=0; j<60;j++){
      gps.encode(gpsSerial.read());
      lattitude = String(gps.location.lat(),6);
      longitude = String(gps.location.lng(),6);
    }
    delay(10);
  }
  Serial.println("AT+CMGD=1");    //Delete privious sent SMS
  delay(1000);
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1 second
  Serial.println("AT+CMGS=\"+91809xxxxxxxx\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.println("Engine Stopped ");
  Serial.print("Present location is ");
  Serial.print("http://maps.google.com/maps?q=loc:");
  Serial.print(lattitude);
  Serial.print(",");
  Serial.print(longitude);
  Serial.println(" hurry up!");
  delay(200);
  Serial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
  delay(3000);
  lcd.clear();
  lcd.print("Message Sent");
  delay(1000);
  lcd.clear();
  lcd.print("System Ready");
  delay(1000);
  lcd.clear();
  delay(500);
  
}
