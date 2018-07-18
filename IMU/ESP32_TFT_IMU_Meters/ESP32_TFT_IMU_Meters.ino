/*
  Example animated analogue meters using a ILI9341 TFT LCD screen

  Needs Font 2 (also Font 4 if using large scale label)

  Make sure all the display driver and pin comnenctions are correct by
  editting the User_Setup.h file in the TFT_eSPI library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/

#include "Meter.h"

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#include "BMP280.h"
#include "Wire.h"

#define P0 1013.25
BMP280 bmp;

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

Meter meter1(0, 0, 239, 106, 5, 1);
Meter meter2(0, 106, 239, 106, 100, 0.05);
Meter meter3(0, 212, 239, 106, 20, 0.25);


#define LOOP_PERIOD 35 // Display updates every 35 ms

uint32_t updateTime = 0;       // time for next update

int value[3] = {0, 0, 0};

void setup(void) {
  Serial.begin(115200); // For debug
  
  // initialize tft
  tft.init();
  tft.setRotation(0);
  
  // Turn on backlight
  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH);
  
  tft.fillScreen(TFT_BLACK);

  // initialize pressure sensor
  if(!bmp.begin()){
    Serial.println("BMP init failed!");
    while(1);
  }
  else Serial.println("BMP init success!");
  
  bmp.setOversampling(4);

  // draw meter 1
  meter1.setCaption("Temp");
  meter1.setUnits("Deg C");
  meter1.draw(tft);
  meter1.drawNeedle(0, 0);

  // draw meter 2
  meter2.setCaption("Press");
  meter2.setUnits("mBar");
  meter2.draw(tft);
  meter2.drawNeedle(0, 0);

  // draw meter 3
  meter3.setCaption("Alt");
  meter3.setUnits("m");
  meter3.draw(tft);
  meter3.drawNeedle(0, 0);

  updateTime = millis(); // Next update time
}


void loop() {
  if (updateTime <= millis()) {
    updateTime = millis() + LOOP_PERIOD;

    double T,P;
    char result = bmp.startMeasurment();
 
    if(result!=0){
      delay(result);
      result = bmp.getTemperatureAndPressure(T,P);
      
      if(result!=0){
      {
        double A = bmp.altitude(P,P0);
        
        Serial.print("T = \t");Serial.print(T,2); Serial.print(" degC\t");
        Serial.print("P = \t");Serial.print(P,2); Serial.print(" mBar\t");
        Serial.print("A = \t");Serial.print(A,2); Serial.println(" m");

        value[0] = T;
        value[1] = P;
        value[2] = A;
    
        //unsigned long t = millis();    

        // draw meters
        meter1.drawNeedle(value[0], 0);
        meter2.drawNeedle(value[1], 0);
        meter3.drawNeedle(value[2], 0);
    
        //Serial.println(millis()-t); // Print time taken for meter update
        }
      }
    }
  }
}




