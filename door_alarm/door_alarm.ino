//sms data variable
char Rx_data[50];
unsigned char Rx_index = 0;
int i = 0;
char msg[160];
int sig;
//variable


String content;
String deviceState;
int charac = 0;

const int led = 8;
const int button = 9;
int buttonState = 0;

void setup()
{
	//timer = outputMin * 60 * 1000
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
   buttonState = digitalRead(button);
   
   if(Serial.available() > 0) {
            char received = Serial.read();
            content += received;
            Serial.println(content);
              if(received == '\n')
              {
                Serial.println(content);   
              }  
    }
    
    if(deviceState == "ON")
    {
    }
}
