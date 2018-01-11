/*
 * 
 * 
 */
//#include <LiquidCrystal.h>
#include <stdio.h>
#define N 23
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);          //Wybór linii, do ktorych podłączony jest wyświetlacz
//char linia1[16], linia2[16];
unsigned char bufor [N];
int PM25 = 20, PM10 = 20, PM1 = 20;
int znachenjaPM1(unsigned char *thebuf);
int znachenjaPM25(unsigned char *thebuf);
int znachenjaPM10(unsigned char *thebuf);
char umova(unsigned char *thebuf, char leng);

void setup(){
   Serial.begin(9600);
   //lcd.begin(16,2);          
   //lcd.setCursor(0,0);      
}
 
void loop(){
  if(Serial.find(0x42))    
    Serial.readBytes(bufor,N);

     if(bufor[0] == 0x4d){
      if(umova(bufor,N)){  
        PM25=znachenjaPM25(bufor);
        PM10=znachenjaPM10(bufor);
        PM1=znachenjaPM1(bufor);  
     }
    }
   //lcd.clear();
   //lcd.setCursor(0,0); 
   //sprintf(linia1,"PM2.5=%d ug/m3",PM25);
   //lcd.print(linia1);
   //lcd.setCursor(0,1);
   //sprintf(linia2,"PM10=%d ug/m3",PM10);
   //lcd.print(linia2);
   
   Serial.print("PM1.0:  ");  //send PM1.0 data to bluetooth
      Serial.print(PM1);
      Serial.println("  ug/m3");
  Serial.print("PM2.5: ");  //send PM1.0 data to bluetooth
      Serial.print(PM25);
      Serial.println("  ug/m3");
   Serial.print("PM10:  ");  //send PM1.0 data to bluetooth
      Serial.print(PM10);
      Serial.println("  ug/m3");
      Serial.println();     
      
       
}

int znachenjaPM1(unsigned char *buf) // generacja wartosci PM1.0
{
  int PM1v;
  PM1v=((buf[9]<<8) + buf[10]);  
  return PM1v;
}
int znachenjaPM25(unsigned char *buf) // generacja wartosci PM2,5
{
  int PM25v;
  PM25v=((buf[11]<<8) + buf[12]);  
  return PM25v;
}

int znachenjaPM10(unsigned char *buf) // generacja wartosci PM10
{
  int PM10v;
  PM10v=((buf[13]<<8) + buf[14]); 
  return PM10v;
}

bool umova(unsigned char *buf, int dlugosc) // sprawdzenie poprawności lancucha
{  
  bool flaga=0;
  int suma=0;

  for(int i=0; i<(dlugosc-2); i++){
  suma+=buf[i];
  }
 suma=suma + 0x42;
 
  if(suma == ((buf[dlugosc-2]<<8)+buf[dlugosc-1]))  
  {
    suma = 0;
    flaga = 1;
  }
  return flaga;
}


