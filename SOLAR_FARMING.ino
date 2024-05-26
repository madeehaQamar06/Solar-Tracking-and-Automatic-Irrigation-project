#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(13,12,14,27,26,25);

// Network credentials
const char* ssid = "dell";
const char* password = "12345678";

// Initialize Telegram BOT Token and Chat ID
#define BOTtoken "5982198371:AAEL3cD2U6IyORf9zlOD_sl5-X85MXyhKbc"  

// #define CHAT_ID "969506549"
//#define CHAT_ID "5365066054"
#define CHAT_ID "1578674359"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

const int LDRR = 16;     
const int LDRL = 17;

const int moisturesensor = 4;

const int relay= 18; 

const int motorP= 19; 
const int motorN= 21; 

const int TEMPSensor = 22;

int LDRStateR = 0; 
int LDRStateL = 0; 

int moistureState = 0; 
int TempState = 0;

int flag=0;

void setup() {
   
   Serial.begin(115200);
  
   pinMode(LDRR, INPUT);
   pinMode(LDRL, INPUT);
   pinMode(moisturesensor, INPUT);
   
   pinMode(TEMPSensor, INPUT);
      
   pinMode(relay, OUTPUT);

   pinMode(motorP, OUTPUT);
   pinMode(motorN, OUTPUT);   
   
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   
   // Add root certificate for api.telegram.org
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 

   // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.print("SOLAR Tracking ");
  lcd.setCursor(0, 1);
  lcd.print("Sensor  SYSTEM ");
    
  bot.sendMessage(CHAT_ID, "Project started up", "");

}

void loop() {
  // put your main code here, to run repeatedly:
  LDRStateR     = digitalRead(LDRR);
  LDRStateL     = digitalRead(LDRL);
  moistureState = digitalRead(moisturesensor);

  TempState = digitalRead(TEMPSensor);
  
   
  if(LDRStateR == LOW)
 { 
  Serial.print("Senosr Trigger");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LDR right sensor");
  lcd.setCursor(0, 1);
  lcd.print("  STATUS : ON   ");
  // bot.sendMessage(CHAT_ID, "LDR RIGHT", "");  
  digitalWrite(motorP,HIGH);
  digitalWrite(motorN,LOW);
 }

else if(LDRStateL == LOW){
  Serial.print("Senosr Trigger");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LDR left sensor");
  lcd.setCursor(0, 1);
  lcd.print("  STATUS : ON   ");
  // bot.sendMessage(CHAT_ID, "LDR left", "");  
  digitalWrite(motorP,LOW);
  digitalWrite(motorN,HIGH);
  }

else if(moistureState == HIGH){
  Serial.print("Moisture Trigger");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Moisture sensor ");
  lcd.setCursor(0, 1);
  lcd.print("  STATUS : ON   ");
  digitalWrite(relay,HIGH);
  if(flag==0)
  {
  bot.sendMessage(CHAT_ID, "Moisture Sensor Trigger", "");
  flag=1;
  }
  }


else if(TempState == LOW){
  Serial.print("Temperature Trigger");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Temperature sensor ");
  lcd.setCursor(0, 1);
  lcd.print("STATUS : Anormal");
  if(flag==0)
  {
  bot.sendMessage(CHAT_ID, "Temperature High", "");
  flag=1;
  }
  }  

else 
{
 flag=0;
 lcd.begin(16, 2);
 lcd.clear();
 lcd.print("Checking Sensor ");
 lcd.setCursor(0, 1);
 lcd.print("Please wait.....");
 digitalWrite(relay,LOW);
 digitalWrite(motorP,LOW);
 digitalWrite(motorN,LOW);
}
}
