////////////// Idea from https://www.hackster.io/adrian-shurmer/environment-monitor-using-nodemcu-esp8266-5c215f /////////
#include "PMS.h"
#include <Wire.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
// #define SLEEP_DELAY_IN_SECONDS 30*1 // every 1 mins 

char *ssid = "****";    //  your network SSID (name) 
char *pass = "****";   // your network password
int status = WL_IDLE_STATUS;
WiFiClient  client;

ESP8266WiFiMulti WiFiMulti;

unsigned long myChannelNumber = ****; // my thing speak account channel number
const char * myWriteAPIKey = "*****";// my thing speak apikey

PMS pms(Serial);
PMS::DATA data;

void setup() {
    Serial.begin(9600); // so we can out put to the screen
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP(ssid, pass);

    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi to connect...");

    while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
        delay(500);
    }
    // give some feed back that we are connected to router
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(500);

    // connect to thingspeak.com
    if (ThingSpeak.begin(client)) {
    //feed back that we have connected to thingspeak
    Serial.println("connected to ThingSpeak");
    }
    else  Serial.println("not connected to ThingSpeak");
    }

// set up complete
 
 
 void loop() {
  
    if (pms.read(data))
  {
    Serial.println("Data:");

    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    Serial.println();
    //unsigned long data = getValue(response);
    float a = data.PM_AE_UG_1_0;
      float b = data.PM_AE_UG_2_5;
        float c = data.PM_AE_UG_10_0;
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    ThingSpeak.setField(1,a);
    ThingSpeak.setField(2,b);
    ThingSpeak.setField(3,c);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    //WiFi.mode(WIFI_OFF);
    //delay(1000*60*30); // Update ThingSpeak 15 min.
    Serial.println("======================================"); 
     //ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
    //ESP.deepSleep(10 * 1000, WAKE_NO_RFCAL);
  delay(30000);
  }
 }
