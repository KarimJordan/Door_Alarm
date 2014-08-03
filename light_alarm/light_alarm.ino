#include <LiquidCrystal.h>
#include <stdlib.h>
#define timer_interval 61000

LiquidCrystal lcd (12, 11, 5, 4, 3, 2);

//sms data variable
char Rx_data[50];
unsigned char Rx_index = 0;
int i = 0;
char msg[160];
int sig;
//variable


String content;
int charac = 0;

const int led = 8;
const int button = 9;
int buttonState = 0;


//received message
int firstPosition;
int secondPosition;
int state;
int time;
String deviceState;
String deviceTime;

static unsigned long lastMillisReading = 0;
static long timer = 0;

int minutes = 0;
long seconds = 0;

int iSeconds;
unsigned long iMinutes =0;

unsigned long outputSec = 65;
unsigned long outputMin = 2;

void setup()
{
	lcd.begin(16, 2);
        lcd.setCursor(0,0);
	lcd.print("Timer");
	//timer = outputMin * 60 * 1000;
	lastMillisReading = millis();
        pinMode(led, OUTPUT);
        pinMode(button, INPUT);
        Serial.begin(19200);
        //Serial.print("AT+CMGF=1\r");
        //delay(100);
        //Serial.print("AT+CNMI=2,2,0,0,0\r");
        //delay(100);
        initSMS();
}

void initSMS()
{
  Serial.print("AT+CMGF=1\r");
  delay(100);
  Serial.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}



void loop()
{
	unsigned long deltaMillis = 0;
	unsigned long thisMillis = millis();
        
        buttonState = digitalRead(button);
        
          if(Serial.available() > 0) {
            char received = Serial.read();
            content += received;
            Serial.println(content);
            if((content.length() != charac) && (content.length() > 0))
            {
              firstPosition = content.indexOf(',');
              secondPosition = content.indexOf(',', firstPosition + 1);
              if(firstPosition == -1)
              {
                charac = 0;
              }else {
                time = firstPosition + 1;
                deviceState = content.substring(0, firstPosition);
                deviceTime = content.substring(time, secondPosition);
              
            }
              if(received == '\n')
              {
                
                Serial.println(content);
                Serial.println(deviceState);
                Serial.println(deviceTime);
                
                iMinutes = deviceTime.toInt();
                content = "";
                
                timer = iMinutes * 60 * 1000;
                
                Serial.println(iMinutes);
                Serial.println(timer);
                Serial.println(seconds);
                Serial.println(minutes);
           
                  
              }  
            } 
          }
          
        seconds = timer/1000;
        minutes = seconds/60;
        
        if(deviceState == "ON")
        {
          delay(100);
          digitalWrite(led, HIGH);
        }else if(deviceState == "OFF")
        {
          delay(100);
          digitalWrite(led, LOW);
          timer = 0;
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
          
          //insert PIR Read Here Code to be updated be tomorrow kapag may load na
          if(buttonState == LOW)
          {
          send_msg("09172796397", "Movement Detected");
          //Serial.println(buttonState);
          }
                //Serial.println(timer/1000);
                //lcd.setCursor(0,1);
                //lcd.print(timer/1000);
	   if(timer <= 0){
	      //lcd.setCursor(0,1);;
	      //lcd.print("Time Up!");
              delay(100);
              digitalWrite(led, LOW);
              deviceState = "OFF";
           }
	} 
        
        
        lcd.setCursor(0,1);
        lcd.print(" ");
        lcd.setCursor(0,1);
        
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
}

void send_msg(char *number, char *msg)
{
  char at_cmgs_cmd[30] = {'\0'};
  char msg1[160] = {'\0'};
  char ctl_z = 0x1A;

  sprintf(msg1, "%s%c", msg, ctl_z);
  sprintf(at_cmgs_cmd, "AT+CMGS=\"%s\"\r\n",number);
  
  sendGSM(at_cmgs_cmd);
  delay(100);
  delay(100);
  delay(100);
  sendGSM(msg1);
  delay(100);
}

void sendGSM(char *string){
  Serial.write(string);
  delay(90);
}

void clearString(char *strArray) {
  int j;
  for (j = 100; j > 0; j--)
    strArray[j] = 0x00;
}

void send_cmd(char *at_cmd, char clr){
  char *stat = '\0';
  while(!stat){
    sendGSM(at_cmd);
    delay(90);
    readSerialString(Rx_data);
    
    stat = strstr(Rx_data, "OK");
  }
  if (clr){
    clearString(Rx_data);
    delay(200);
    stat = '\0';
  }
}

void initGSM(){
  
  send_cmd("AT\r\n",1);						
//  send_cmd("ATE0\r\n",1); // Turn off automatic echo of the GSM Module	
	
  send_cmd("AT+CMGF=1\r\n",1);			// Set message format to text mode
  //Sucess
  
  Serial.println("Success");
	
  delay(1000);
  delay(1000);
  delay(1000);
}

void readSerialString (char *strArray) {
  
  if(!Serial.available()) {
    return;
  }
  
  while(Serial.available()) {
    strArray[i] = Serial.read();
    i++;
  }
}
