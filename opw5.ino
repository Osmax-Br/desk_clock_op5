#include "icons.h"
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <ESP8266HTTPClient.h>  
#include <ArduinoJson.h> 
#include <Wire.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const long utcOffsetInSeconds = 7200;
int hours;
char amPm;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
//////////////////////////
static const uint8_t Dd0   = 16;
static const uint8_t Dd1   = 5;
static const uint8_t Dd2   = 4;
static const uint8_t Dd3   = 0;
static const uint8_t Dd4   = 2;
static const uint8_t Dd5   = 14;
static const uint8_t Dd6   = 12;
static const uint8_t Dd7   = 13;
static const uint8_t Dd8   = 15;
static const uint8_t Dd9   = 3;
static const uint8_t Dd10  = 1;

/////////////////////////
const int switchPin = Dd0;
static int hits = 0;
int switchState = 0;
int prevSwitchState = 0;
String dayStamp;
String formattedDate;
const char *ssid     = "Home";
const char *password = "Hta87#Mi00";
void setup(void) {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 pinMode(switchPin,INPUT);
 WiFi.begin(ssid, password);   // access Wi-FI point
  display.clearDisplay();
 while ( WiFi.status() != WL_CONNECTED ) {
   display.setTextSize(2);
   display.setTextColor(WHITE);
   display.setCursor(4,25);
   display.println("CONNECTING");
   Serial.print("conetcin");
   display.display(); 
    }
    display.clearDisplay();
    timeClient.begin();
    }


void loop() {
 
  // put your main code here, to run repeatedly:
if (WiFi.status() == WL_CONNECTED){
    timeClient.update();
    HTTPClient http;  
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=Homs,SY&APPID=1041444a18cfb8448343254a45721b1d");
    int httpCode = http.GET();  
if (httpCode > 0) { 
      String payload = http.getString();
      DynamicJsonBuffer jsonBuffer(512);
      JsonObject& root = jsonBuffer.parseObject(payload);
if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        return;
      }
////////////////////////////////////      
 int cloud = root["clouds"]["all"];
 double sunset = root["sys"]["sunset"];
 int temp1 = root["main"]["temp"];
 int humidity = root["main"]["humidity"]; 
 int pressure = root["main"]["pressure"];
 float wind_speed = root["wind"]["speed"]; 
 int wind_degree = root["wind"]["deg"]; 
 String weather = root["weather"][0]["main"];
 const char* description = root["weather"][0]["description"];
 int temp = temp1 - 273;
 Serial.println(weather);
 Serial.println(description);
///////////////////////////////////// 
 switchState = digitalRead(switchPin);
  if (switchState != prevSwitchState) {
    if (switchState == HIGH) {
          hits = hits + 1;
          delay(10);
    }
  }
 formattedDate = timeClient.getFormattedDate();
 int splitT = formattedDate.indexOf("T");
 dayStamp = formattedDate.substring(0, splitT);
 
 
 hours=timeClient.getHours();
 if(hits==0){
   
 display.setCursor(70,40);
display.setTextSize(1);
 display.print(daysOfTheWeek[timeClient.getDay()]);
 display.setCursor(66,0);
 display.setTextSize(2);
 if(hours>12){
   hours=hours-12;
   display.print(hours);
   amPm='P'; }
else{
  display.print(hours);
  amPm='A';  }
display.print(":");
display.print(timeClient.getMinutes());
display.setCursor(83,20);
if(amPm=='P'){
  display.print("PM");
  }
 else if(amPm=='A'){
  display.print("AM");
  }
display.setCursor(66,55);
display.setTextSize(1);

display.print(dayStamp);

////////
if(weather=="Thunderstorm")
   display.drawBitmap(0, 0,wind, 40, 52, 1);
 else if(weather=="Rain"&&amPm=='A')
   display.drawBitmap(0, 0,rainy, 59, 46, 1);
 else if(weather=="Rain"&&amPm=='P')
   display.drawBitmap(0, 0,rainynight, 58, 45, 1);    
 else if(weather=="Snow")
   display.drawBitmap(0, 0,snowy, 59, 56, 1); 
 else if(weather=="Fog")
   display.drawBitmap(0, 0,foggy, 61, 41, 1);
 else if(weather=="Clear"&&amPm=='A')
   display.drawBitmap(0, 0,sunny, 49, 48, 1); 
 else if(weather=="Clear"&&amPm=='P')
   display.drawBitmap(0, 0,clearnight, 61, 48, 1);
 else if(weather=="Clouds"&&amPm=='A'&&description!="few clouds")
   display.drawBitmap(0, 0,cloudy, 59, 30, 1);
 else if(weather=="Clouds"&&amPm=='P')
   display.drawBitmap(0, 0,cloudynight, 56, 38, 1);
 else if(weather=="Clouds"&&amPm=='A'&&description=="few clouds")
   display.drawBitmap(0, 0,partlycloudy, 58, 38, 1);                    
///////
display.display();
}
  else if(hits==1)
  {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(30,0);
      display.print("Temperature"); 
      display.setTextSize(2); 
      display.setCursor(38,15);
      display.print(temp);
      display.print(" C");
      display.setTextSize(1);
      display.print((char)247);
      display.setCursor(40,35);
      display.print("Humidity"); 
      display.setTextSize(2); 
      display.setCursor(45,45);
      display.print(humidity);
      display.print("%");
      display.display();
  }else
  if(hits==2)
  {
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(35,0);
      display.print("Wind Speed"); 
      display.setTextSize(2); 
      display.setCursor(38,15);
      display.print(wind_speed);
      display.setTextSize(1);
      display.setCursor(75,19); 
      display.print("  m/s"); 
     
      display.setCursor(32,35);
      display.print("Wind Degree"); 
      display.setTextSize(2); 
      display.setCursor(45,45);
      display.print(wind_degree);
      display.setTextSize(1); 
      display.print((char)247);
      display.display();
  }
  else
  if(hits==3)
  {
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(42,0);
      display.print("Pressure"); 
      display.setTextSize(2); 
      display.setCursor(32,15); 
      display.print(pressure);
      display.setTextSize(1);
      display.setCursor(79,23); 
      display.print("Bar");
      
      display.setCursor(45,35);
      display.print("Clouds"); 
      display.setTextSize(2); 
      display.setCursor(50,47);
      display.print(cloud);
      display.setTextSize(1); 
      display.print("%");
      display.display();   
  }
   else if ( hits>=4) {
     hits = -1;}
  
delay(1000);
display.clearDisplay();
     }
    http.end();   
  }
}
/*
void weathericon(){
  if(weather=="Thunderstorm")
   display.drawBitmap(0, 0,wind, 40, 52, 1);
 else if(weather=="Rain"&&amPm=='A')
   display.drawBitmap(0, 0,rainy, 59, 46, 1);
 else if(weather=="Rain"&&amPm=='P')
   display.drawBitmap(0, 0,rainynight, 58, 45, 1);  
 else if(weather=="Rain"&&amPm=='A')
   display.drawBitmap(0, 0,rainy, 59, 46, 1);   
 else if(weather=="Snow")
   display.drawBitmap(0, 0,snowy, 59, 56, 1);      
  }*/
  
