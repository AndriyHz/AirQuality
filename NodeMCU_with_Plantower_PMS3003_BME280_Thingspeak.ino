///// https://www.hackster.io/adrian-shurmer/environment-monitor-using-nodemcu-esp8266-5c215f
#include "PMS.h"
///// BME280 ////////////////
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
/////////////////////////////
#include <Wire.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#define SLEEP_DELAY_IN_SECONDS 30*1 // every 1 mins 

char *ssid = "HzA home studion";    //  your network SSID (name) 
char *pass = "4LEV3ZSD54";   // your network password
int status = WL_IDLE_STATUS;
WiFiClient  client;

ESP8266WiFiMulti WiFiMulti;

unsigned long myChannelNumber = 462442; // my thing speak account channel number
const char * myWriteAPIKey = "R1K2Y3C972LBAKQ7";// my thing speak apikey

PMS pms(Serial);
PMS::DATA data;
//// BME280 ///////////////
float h, t, p, pin, dp;
char temperatureFString[6];
char dpString[6];
char humidityString[6];
char pressureString[7];
char pressureInchString[6];

Adafruit_BME280 bme; // I2C

///////////////////////////
void setup() {
    Serial.begin(9600); // so we can out put to the screen
    delay(10);
////////////// BME280 ////////////
 Wire.begin(D3, D4); // Make sure you have D3 & D4 hooked up to the BME280
  Wire.setClock(100000);
/////////////////////////////////
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

    //////// BME280 ///////////////////
    Serial.println(F("BME280 test"));

    if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
    //////////////////////////////////
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
    ///////// BME280 //////////
    h = bme.readHumidity();
    t = bme.readTemperature();
   // t = t*1.8+32.0;
   // dp = t-0.36*(100.0-h);
    
    p = bme.readPressure()*0.00750061683;
   // pin = 0.02953*p;
    dtostrf(t, 5, 1, temperatureFString);
    dtostrf(h, 5, 1, humidityString);
    dtostrf(p, 6, 1, pressureString);
   // dtostrf(pin, 5, 2, pressureInchString);
   // dtostrf(dp, 5, 1, dpString);
   delay(500);

    Serial.print("Temperature = ");
    Serial.println(temperatureFString);
    Serial.print("Humidity = ");
    Serial.println(humidityString);
    Serial.print("Pressure = ");
    Serial.println(pressureString);
   // Serial.print("Pressure Inch = ");
   // Serial.println(pressureInchString);
   // Serial.print("Dew Point = ");
   // Serial.println(dpString);
    
   ///////////////  end BME280 //////////////
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
    ThingSpeak.setField(4,t);
    ThingSpeak.setField(5,h);
    ThingSpeak.setField(6,p);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    //WiFi.mode(WIFI_OFF);
    //delay(1000*60*30); // Update ThingSpeak 15 min.
    Serial.println("======================================"); 
     //ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
    //ESP.deepSleep(10 * 1000, WAKE_NO_RFCAL);
  delay(30000);
  }
 }
