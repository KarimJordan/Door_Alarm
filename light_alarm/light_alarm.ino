#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 1, 0);
SoftwareSerial sms (2, 3);

char inChar = 0;
String raw_command;

int firstPosition;
int secondPosition;
int state;
int time;
int charac = 0;

String deviceState;
String deviceTime;

//output
const int output = 8;
const int input = 6;
int buttonState = 0;

//timer
static unsigned long lastMillisReading;
static long timer = 0;
int minutes = 0;
long seconds =0;
unsigned long iMinutes = 0;

void setup()
{
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Timer");
  pinMode(output, OUTPUT);
  pinMode(input, INPUT);
  //Serial.begin(19200);
  sms.begin(19200);
  lastMillisReading = millis();
  delay(10000);
  sms.print("AT+CMGF=1\r");
  delay(100);
  sms.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}

void powerSIM900()
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);  
}

void sendSMS()
{
  sms.print("AT+CMGF=1\r");
  delay(100);
  sms.println("AT + CMGS = \"+639172796397\""); 
  delay(100);
  sms.println("Movement Detected");
  delay(100);
  sms.println((char)26);
  delay(100);
  sms.println();
  delay(5000);
  powerSIM900();
}

void loop()
{
  unsigned long deltaMillis = 0;
  unsigned long thisMillis = millis();
  
  buttonState = digitalRead(input);
  
  if(sms.available() > 0)
  {
    inChar = sms.read();
    raw_command += inChar;
    String message = raw_command;
    if(inChar == '\n')
    {
      message.trim();
      if((message.length() != charac) && (message.length() > 0))
      {
        firstPosition = message.indexOf(',');
        secondPosition = message.indexOf(',', firstPosition + 1);
        if(firstPosition == -1)
        {
          charac = 0;
        }else{
          time = firstPosition + 1;
          deviceState = message.substring(0, firstPosition);
          deviceTime = message.substring(time, secondPosition);
          iMinutes = deviceTime.toInt();
          timer = iMinutes * 60 * 1000;
        }
      }
      //Serial.println(message); 
      //Serial.println(message.length());
      //Serial.println(deviceState);
      //Serial.println(iMinutes);
      raw_command = "";
    }
  }
      if(deviceState == "ON")
      {
        delay(100);
        digitalWrite(output, HIGH);
      }else if(deviceState == "OFF")
      {
         delay(100);
         digitalWrite(output, LOW);
         timer =0;
         seconds = 0;
         minutes = 0;
         thisMillis = 0;
      }
      
      if(thisMillis != lastMillisReading)
      {
        deltaMillis = thisMillis - lastMillisReading;
        lastMillisReading = thisMillis;
      }
       if(timer > 0)
      {
        timer -= deltaMillis;
        seconds = timer/1000;
        minutes = (seconds/60);
        //Serial.println(seconds);
        //Serial.println(minutes);
        if(buttonState == HIGH)
        {
          sendSMS();
          
        }
        if(timer <= 0)
        {
          delay(100);
          digitalWrite(output, LOW);
          deviceState = "OFF";
        }
      }
      
      lcd.print(" ");
      lcd.setCursor(0,1);
      if(minutes > 60)
      {
        timer =0;
        seconds = 0;
        minutes = 0;
        thisMillis = 0;
        digitalWrite(output, LOW);
        //lcd.print("Invalid Time");
        //lcd.setCursor(0,1);
      }
      if(minutes < 10)
      {
        lcd.print("00");
        lcd.setCursor(1,1);
      }
        lcd.print(minutes);
        lcd.setCursor(2,1);
        lcd.print(":");
        lcd.print(" ");
        lcd.setCursor(3,1);
       if(seconds < 10)
       {
         lcd.print("00");
         lcd.setCursor(4,1);
       }
         lcd.print(seconds % 60);
         delay(50);
  deviceState = "";
  deviceTime = "";
}