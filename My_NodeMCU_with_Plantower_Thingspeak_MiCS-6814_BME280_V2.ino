///// https://www.hackster.io/adrian-shurmer/environment-monitor-using-nodemcu-esp8266-5c215f
#include "PMS.h"
///// MiCS-6814  ////////////////
#include <Wire.h>
#include "MutichannelGasSensor.h"
/////////////////////////////

#include <Wire.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#define SLEEP_DELAY_IN_SECONDS 30*1 // every 1 mins 

///////////////////// BME280 ///////////////////////////////
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
float h, t, p, pin, dp;
char temperatureFString[6];
char dpString[6];
char humidityString[6];
char pressureString[7];
char pressureInchString[6];
Adafruit_BME280 bme; // I2C
///////////////////////////// end BME280 ///////////////////

char *ssid = "HzA home studion";    //  your network SSID (name) 
char *pass = "4LEV3ZSD54";   // your network password
int status = WL_IDLE_STATUS;
WiFiClient  client;

ESP8266WiFiMulti WiFiMulti;

unsigned long myChannelNumber = 507656; // my thing speak account channel number
const char * myWriteAPIKey = "NJXWVRG0E8SWR2VZ";// my thing speak apikey

PMS pms(Serial);
PMS::DATA data;


void setup() {
  Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial1.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)

 ///////////////////// BME280 ///////////////////////////////
 //Serial.begin(9600);
  delay(10);
  Wire.begin(D3, D4); // Make sure you have D3 & D4 hooked up to the BME280
  Wire.setClock(100000);
 ///////////////////// end BME280 ///////////////////////////////
    
 /////////////////////////////// ///// MiCS-6814  //////////////// ///////////////
    //Serial.begin(115200);  // start serial for output
    Serial.println("power on!");
    gas.begin(0x04);//the default I2C address of the slave is 0x04
    gas.powerOn();
    Serial.print("Firmware Version = ");
    Serial.println(gas.getVersion());
 //////////////////////////// end MiCS-6814  ///////////////
 
 //////////////////////////////// Plantower ///////////////
    pms.passiveMode();    // Switch to passive mode
 //////////////////////////////// end Plantower ///////////
    
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
//////////////////////////////// ///// MiCS-6814  //////////////// ///////////////
float c;
float d;
float e;
    c = gas.measure_NO2();
    Serial.print("The concentration of NO2 is ");
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" ppm");  
    
    d = gas.measure_CO();
    Serial.print("The concentration of CO is ");
    if(d>=0) Serial.print(d);
    else Serial.print("invalid");
    Serial.println(" ppm");
    
    e = gas.measure_NH3();
    Serial.print("The concentration of NH3 is ");
    if(e>=0) Serial.print(e);
    else Serial.print("invalid");
    Serial.println(" ppm");
   
////////////////////////// end MiCS-6814 ///////////////

//////////////////////////////// Plantower ///////////////
  Serial.println("Wake up, wait 30 seconds for stable readings...");
  pms.wakeUp();
  delay(30000);

  Serial.println("Send request read...");
  pms.requestRead();

  Serial.println("Wait max. 10 seconds for read...");
  if (pms.read(data, 10000))
///////////////////////////////////////////////////////  
  {

   /////////////////////  BME280 ///////////////////////////////
    bme.begin();
    h = bme.readHumidity();
    t = bme.readTemperature();
    //t = t*1.8+32.0;
    dp = t-0.36*(100.0-h);
    
    p = bme.readPressure()/100.0F;
    pin = 0.02953*p;
    
    delay(10000);

    Serial.print("Temperature = ");
    Serial.println(t);
    Serial.print("Humidity = ");
    Serial.println(h);
    Serial.print("Pressure = ");
    Serial.println(p);
    // Serial.print("Pressure Inch = ");
    // Serial.println(pressureInchString);
    // Serial.print("Dew Point = ");
    // Serial.println(dpString);
        delay(100);
      
////////////////////////// end BME280 ///////////////  

    Serial.println("Data:");
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);
    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);
    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);
    Serial.println();
    
        //unsigned long data = getValue(response);
    //float a = data.PM_AE_UG_1_0;
    //  float b = data.PM_AE_UG_2_5;
    //    float c = data.PM_AE_UG_10_0;
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    ThingSpeak.setField(1,data.PM_AE_UG_1_0);
    ThingSpeak.setField(2,data.PM_AE_UG_2_5);
    ThingSpeak.setField(3,data.PM_AE_UG_10_0);
    ThingSpeak.setField(7,t);
    ThingSpeak.setField(8,h);
    
    ////// MiCS-6814 /////////////////////////
      ThingSpeak.setField(4,c);
      ThingSpeak.setField(5,d);
      ThingSpeak.setField(6,e);
    ///////////////////////////////////////////
    
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    //WiFi.mode(WIFI_OFF);
    //delay(1000*60*30); // Update ThingSpeak 15 min.
    Serial.println("======================================"); 
     //ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
    //ESP.deepSleep(10 * 1000, WAKE_NO_RFCAL);
  delay(4000);
  }
   else
  {
    Serial.println("No data.");
  }
     
  Serial.println("Going to sleep for 60 seconds.");
  pms.sleep();
  delay(60000);
 }
