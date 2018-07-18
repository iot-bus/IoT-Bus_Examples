
HardwareSerial Serial2(2);

void setup(){
  Serial.begin(115200);
  Serial2.begin(9600);

  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  delay(100);
  digitalWrite(33, HIGH);
  delay(1200);
  digitalWrite(33, LOW);
//  delay(100);
//  digitalWrite(33, HIGH);
  
}

void loop() // run over and over
{
  if (Serial2.available())
    Serial.write(Serial2.read());
  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
  if(Serial.available()){    
    Serial2.write(Serial.read());
  }
}


