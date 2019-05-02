#include <Wire.h>
#include "cactus_io_BME280_I2C.h"
 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
 
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
 
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
 
#define BTN_UP 9
 
int statusBtnUp = 0; //Status des Tasters
int index = 1; // Zählvariable
 
BME280_I2C bme(0x76);  // I2C using address 0x76
 
void setup() {
  Serial.begin(9600);
 
  //Setup für den BME280 Sensor
  if (!bme.begin()) {
    Serial.println("Es konnte kein BME280 Sensor gefunden werden!");
    Serial.println("Bitte überprüfen Sie die Verkabelung!");
    while (1);
  }
  bme.setTempCal(-1);
  
  //Setup für das Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(1000);
  display.clearDisplay();
 
  //Setup für den Button
  pinMode(BTN_UP,INPUT);
}
 
void loop() {
    bme.readSensor();
 
    statusBtnUp = digitalRead(BTN_UP);
    Serial.println(statusBtnUp);
    
    if(statusBtnUp == HIGH && index <3){ 
      index = index+1;
    } else if (statusBtnUp == HIGH && index ==3){
      index = 1;
    }
 
   switch(index){
     case 1: zeigeTemperatur(); break; 
     case 2: zeigeLuftfeuchtigkeit(); break;
     case 3: zeigeLuftdruck(); break;
   }    
}
 
void zeigeTemperatur(){
  String tempC = String(bme.getTemperature_C())+"C";
  displayText("Temperatur", tempC);
}
 
void zeigeLuftfeuchtigkeit(){
  String humidity = String(bme.getHumidity())+"%";
  displayText("Luftfeuchtigkeit", humidity);
}
 
void zeigeLuftdruck(){
   String pressure = String(bme.getPressure_MB())+" mPa";
   displayText("Luftdruck", pressure);
}
 
void displayText(String row1, String row2){
  delay(1000);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(row1);
  display.setCursor(0,10);
  display.println(row2);
  display.display();
  display.clearDisplay();
}
