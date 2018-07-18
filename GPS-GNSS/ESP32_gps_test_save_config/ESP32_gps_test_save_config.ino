//#include <SoftwareSerial.h>
 
//Create software serial object to communicate with GPS
//SoftwareSerial gps(4, 3);
HardwareSerial gps(2);

char saveConfig[] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0xCD};
 
void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(115200);
  while(!Serial);
   
  //Being serial communication with Arduino and GPS Module //Important rate must be 9600
  gps.begin(9600);
  delay(1000);

  gps.println(F("$PUBX,40,RMC,0,0,0,0*47")); //RMC OFF
  //gps.println(F("$PUBX,40,RMC,0,1,0,0*47")); //RMC ON
  delay(100);
  gps.println(F("$PUBX,40,VTG,0,0,0,0*5E")); //VTG OFF
  //gps.println(F("$PUBX,40,VTG,0,1,0,0*5E")); //VTG ON
  delay(100);
  gps.println(F("$PUBX,40,GGA,0,0,0,0*5A")); //CGA OFF
  //gps.println(F("$PUBX,40,GGA,0,1,0,0*5A")); //CGA ON
  delay(100);
  gps.println(F("$PUBX,40,GSA,0,0,0,0*4E")); //GSA OFF
  //gps.println(F("$PUBX,40,GSA,0,1,0,0*4E")); //GSA ON
  delay(100);
  //gps.println(F("$PUBX,40,GSV,0,0,0,0*59")); //GSV OFF
  gps.println(F("$PUBX,40,GSV,0,1,0,0*59")); //GSV ON
  delay(100);
  //gps.println(F("$PUBX,40,GLL,0,0,0,0*5C")); //GLL OFF
  gps.println(F("$PUBX,40,GLL,0,1,0,0*5C")); //GLL ON
  delay(1000);

  gps.println(saveConfig);
   
  Serial.println("Setup Complete!");
}
 
void loop() {

  //Read SIM800 output (if available) and print it in Arduino IDE Serial Monitor
  if(gps.available()){
    Serial.write(gps.read());
  }
  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
  if(Serial.available()){    
    gps.write(Serial.read());
  }
}
