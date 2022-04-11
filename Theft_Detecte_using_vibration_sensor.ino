//  THE TEAM OF E INFOTAINMENT   
      //https://www.youtube.com/channel/UCr688fGR4aI_tWYD-YKYrzQ
      //https://www.youtube.com/channel/UCr688fGR4aI_tWYD-YKYrzQ
                 //please subscribe to my youtube channel
// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
SoftwareSerial GSM(0,1);
const int Switch=8;
const int Sensor=9;
int Buzzer=13;

void setup() {
  pinMode(Sensor,INPUT);
  pinMode(Buzzer,OUTPUT);
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
}

void loop() {
  // Print a message to the LCD.  
    if(digitalRead(Switch)==HIGH)
    {     
        lcd.setCursor(0, 0);
        lcd.print("Security System ");
        lcd.setCursor(0, 1);
        lcd.print("       ON       ");

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
