# FOR ARDUINO UNO
## PM2.5-PMS3003
PM2-myWork.ino - 
* Idea from http://mikrokontroler.pl/2017/06/14/projekt-czujnik-pylu-pms3003-i-arduino/ but added PM1.0 value

PM2-myWork_V1.ino -
* Idea from http://www.pakmingwan.com/creating-a-pollution-sensor-based-on-plantower-particulate-sensors-pms3003/



# FOR TEENSY 3.2  with SenseAir S8 and BME280
PM2_with_S8_BME280-For-Teensy3.ino
* Idea from https://github.com/alanishungry/AirSensorPackage/tree/master/Read_and_print_data_from_PMS3003_alan

# FOR NodeMcu  with PMC3003
1. NodeMCU_with_Plantower_PMS3003.ino 
  1.1. NodeMCU_with_Plantower_PMS3003_BME280_Thingspeak.ino 
  1.2. My_NodeMCU_with_Plantower_Thingspeak_BME280_advance.ino

> Plantower PMS 1003&Plantower PMS 3003: RX - TX; TX - RX;

* The most of the BME280 sensor modules available on eBay or Aliexpress,  have I2C address to be 0x76. So, if you didn’t get any response from the sensor using the default address set in the Adafruit_BME280.h file, you might need to change that to 0x76.

## FOR NodeMcu V3  with PMC3003 and Grove - Multichannel Gas Sensor - BME280
1. My_NodeMCU_with_Plantower_Thingspeak_MiCS-6814_advance.ino
2. My_NodeMCU_with_Plantower_Thingspeak_MiCS-6814_BME280_advance_V.ino

Multichannel Gas Sensor MiCS-6814 Connection Points.
> Grove - Multichannel Gas Sensor: D1 - SCL; D2 - SDA;

* For example, for most of the BME280 sensor modules available on eBay or Aliexpress have their I2C address to be 0x76. If you dont get a response from the sensor using the default address set in the Adafruit_BME280.h file, you might need to change it to 0x76.

BME280 & ESP8266 Connection Points. 
> ESP8266 3.3V to BME280 Vin, ESP8266 GND to BME280 GND, ESP8266 D4 to BME280 SCL, ESP8266 D3 to BME280 SDA. 


