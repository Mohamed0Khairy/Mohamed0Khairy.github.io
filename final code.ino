#include <WiFi.h>
#include "ThingSpeak.h"

#define mq5   35
#define phpin    34
#define CHANNEL_ID 1976657    // the channel id 
#define CHANNEL_API_KEY "12C59YVUBGKRHS6P"  //the  Api key of the channel 


int gas, so2lvl;
int SensorValue[10];
int Calib_VAL = 1045;
char j, k;
long result;
float ph;
float Value=0;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

unsigned long myChannelNumber = 2; // the channel number 

String apiKey = "JXP1EU0EL6EV1T1B";     
const char* ssid =  "Mohamed Khairy";     // the network that connect the ESP 
const char* password =  "32100123";       // network password
const char* server = "api.thingspeak.com";
WiFiClient client;

void setup() {

  pinMode(mq5, INPUT);
  pinMode(phpin,INPUT);

  Serial.begin(115200);
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
     {
            Serial.println("Wifi connecting...");
            delay(500);
     }
    while (WiFi.status() != WL_CONNECTED)
    {
            Serial.print(".");
            delay(500); 
    }
  
  Serial.println("");
  Serial.println("WiFi connected");
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  delay(1000);
}


void loop() {
    if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(2000);     
      } 
      Serial.println("\nConnected.");
    }
  
  for (j = 0; j < 10; j++) {
     SensorValue[j] = analogRead(sensor);
     delay(20);
    }

    // we now have 10 readings, so average them
    result = 0;
    for (j = 0; j < 10; j++) {
    result = result + SensorValue[j]; // add them up
   }
  result = analogRead(mq5);               // this is our averaged result
  so2lvl = result - 400 ;
  Serial.print("SO2 LEVEL = ");
  Serial.print(so2lvl);
  Serial.println(" PPM");
  
  Value= analogRead(phpin);
  Serial.print(Value);
  Serial.print(" | ");
  float voltage=Value*(3.3/4095.0);
  ph=(3.3*voltage);
  Serial.print("ph Val = ");
  Serial.println(ph);

  ThingSpeak.setField(1,so2lvl);
  ThingSpeak.setField(2,ph);
  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);
 
}
}
