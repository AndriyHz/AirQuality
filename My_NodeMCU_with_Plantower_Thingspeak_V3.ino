#include "PMS.h"
#include <Wire.h>
/////// Wi-Fi //////////////
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
WiFiClient client;
String thingSpeakAddress= "http://api.thingspeak.com/update?";
String writeAPIKey;
String tsfield1Name;
String request_string;
HTTPClient http;
const int postingInterval = 20 * 1000; // post data every 20 seconds
///////////////////////////


PMS pms(Serial);
PMS::DATA data;

void setup()
{
  Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial1.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
////// Wi-Fi ///////////////////
WiFi.disconnect();
  delay(3000);
WiFi.begin("HzA home studion","4LEV3ZSD54");
while (WiFi.status() != WL_CONNECTED) {
   delay(500);
 }
////////////////////////////////  
}

void loop()
{
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
    
 ////// Wi-Fi ///////////////
 if (client.connect("api.thingspeak.com",80)) {
/// read sensor ///////
 
//////////////////////
////// Wi-Fi /////////////// 
      writeAPIKey = "key=R1K2Y3C972LBAKQ7";
      tsfield1Name = "&field1=pmat10&field2=pmat25&field3=pmat100";
      request_string = thingSpeakAddress;
      request_string += "key=";
      request_string += "R1K2Y3C972LBAKQ7";
      request_string += "&";
      request_string += "field1";
      request_string += "=";
      request_string += data.PM_AE_UG_1_0;
 request_string += "&";
      request_string += "field2";
      request_string += "=";
      request_string += data.PM_AE_UG_2_5;
 request_string += "&";
      request_string += "field3";
      request_string += "=";
      request_string += data.PM_AE_UG_10_0;
      http.begin(request_string);
      http.GET();
      http.end();
    ///////////////////////////////

    
 }
 client.stop();

  // wait and then post again
  delay(postingInterval);
}
}
