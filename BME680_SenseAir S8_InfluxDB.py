import time
import board
from busio import I2C
import adafruit_bme680
import datetime
from influxdb import InfluxDBClient
#S8 CO2
import serial
#End S8
# set db configuration
host = "localhost"
port = 8086
user = "admin"
password = "<password>"
dbname = "logger"

# read sensor data
# Create library object using our Bus I2C port
i2c = I2C(board.SCL, board.SDA)
bme680 = adafruit_bme680.Adafruit_BME680_I2C(i2c, debug=False)
# create influxdb client
client = InfluxDBClient(host, port, user, password, dbname)
#S8
ser = serial.Serial("/dev/ttyS0",baudrate =9600,timeout = 2)
ser.flushInput()
time.sleep(1)

while True:
 ser.flushInput()
 #ser.write("\xFE\x44\x00\x08\x02\x9F\x25")
 #ser.write("0xFE,0x44,0x00,0x08,0x02,0x9F,0x25")
 ser.write(serial.to_bytes([0xFE,0x44,0x00,0x08,0x02,0x9F,0x25]))

 time.sleep(1)

 resp = ser.read(7)
 high = resp[3]
 low = resp[4]
 co2 = (high*256) + low
 
 time.sleep(1)
#End S8
 print("\nCO2: %ppm",  co2)  
 print("Temperature: %0.1f C" % bme680.temperature)
 print("Gas: %d ohm" % bme680.gas)
 print("Humidity: %0.1f %%" % bme680.humidity)
 print("Pressure: %0.3f hPa" % bme680.pressure)
 print("Altitude = %0.2f meters" % bme680.altitude)
 
 time.sleep(.1)



 temp = bme680.temperature
 press = bme680.pressure * 0.750061683
 humi = bme680.humidity
 gas = bme680.gas
 iso = time.ctime()
 co = co2
 
 
 json_body = [
    {
        "measurement": "test",
        "tags": {
            "run": 1,
            "raspid": "raspi1"
        },
        "time": iso,
        "fields": {
            "temp": temp,
            "press": press,
            "humi": humi,
            "gas": gas,
            "co2": co,
        }
    }
]
 print(json_body)

# Write JSON to InfluxDB

 res = client.write_points(json_body)
 
 print(res)
 time.sleep(5)
