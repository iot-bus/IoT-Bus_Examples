#include "IOT_BUS_SIM800.h" 

unsigned short MQTTProtocolNameLength;
unsigned short MQTTClientIDLength;
unsigned short MQTTUsernameLength;
unsigned short MQTTPasswordLength;
unsigned short MQTTTopicLength;

const char MQTTHost[] = "m14.cloudmqtt.com";
const char MQTTPort[] = "17904";
const char MQTTClientID[] = "ABCDEF";
const char MQTTTopic[] = "SampleTopic";
const char MQTTTopic2[] = "SampleTopic2";
const char MQTTProtocolName[] = "MQIsdp";
const char MQTTLVL = 0x03;
const char MQTTFlags = 0xC2;
uint8_t MQTTKeepAlive = 60;
const char MQTTUsername[] = "kfuhqble";
const char MQTTPassword[] = "ouEFAawozlRf";
const char MQTTQOS = 0x00;
uint8_t MQTTPacketID = 0x0001;

unsigned int Counter = 0;
unsigned long datalength, checksum, rLength;
unsigned short topiclength;
unsigned short topiclength2;
unsigned char topic[30];
unsigned char encodedByte;
int X;

int8_t answer;
int resetPin= 33;

char buffer[255];
char ip[16];

char apn[4] = "pgw";
//char apn[10] = "hologram";

char host[32] = "m10.cloudmqtt.com";

char get[255] = "GET https://www.oddwires.com HTTP/1.0\r\nHost: www.oddwires.com\r\n\r\n"; 

char commandBuffer[128];

HardwareSerial Serial2(2);

IOT_BUS_SIM800 SIM800 = IOT_BUS_SIM800(&Serial2, resetPin);

void setup(){
    Serial.begin(115200);
    Serial.println("Starting...");
    if(!SIM800.begin()){
      Serial.println("Failed to start");
      while(1);
    }
}

void loop(){
        Serial.println("Attaching");
        SIM800.sendCheckReply("AT+CGATT=1", "OK", 1000); 
        SIM800.printReply();
        Serial.println("Shutdown existing connections");
        SIM800.sendCheckReply("AT+CIPSHUT", "SHUT OK", 2000); 
        SIM800.printReply();
 
    // Selects Single-connection mode
    Serial.println("ACIPMUX"); 
    if (SIM800.sendCheckReply("AT+CIPMUX=0", "OK", 1000) == 1)
    {
        // Waits for status IP INITIAL
       Serial.println("IP INITIAL"); 
       delay(1000);
       while(SIM800.sendCheckReply2("AT+CIPSTATUS", "STATE: IP INITIAL", 1000 == 0));
        SIM800.printReply();
        // Sets the APN, user name and password
        Serial.println("CSTT"); 
        sprintf(commandBuffer, "AT+CSTT=\"%s\"", apn);
        if (SIM800.sendCheckReply(commandBuffer, "OK",   30000) == 1)
        {
            // Waits for status IP START
            Serial.println("IP START");
            while(SIM800.sendCheckReply2("AT+CIPSTATUS", "STATE: IP START", 500)  == 0 );
            delay(1000);

            // Brings Up Wireless Connection
            Serial.println("CIICR");
            if (SIM800.sendCheckReply("AT+CIICR", "OK", 30000) == 1)
            {
                // Waits for status IP GPRSACT
                 Serial.println("IP GPRSACT");
                while(SIM800.sendCheckReply2("AT+CIPSTATUS", "STATE: IP GPRSACT", 500)  == 0 );
                delay(1000);

                // Gets Local IP Address
                 Serial.println("CIFSR");
                SIM800.sendCheckReply("AT+CIFSR", "ERROR", 1000);
                //{
                    SIM800.getBuffer(ip, sizeof(ip));
                    //strcpy(ip, replybuffer);
                    Serial.println(ip);
                    // Waits for status IP STATUS
                    Serial.println("IP STATUS");
                    while(SIM800.sendCheckReply2("AT+CIPSTATUS", "STATE: IP STATUS", 500)  == 0 );
                    delay(1000);
                    Serial.println("Opening TCP");

                    // Opens a TCP socket
                    sprintf(commandBuffer, "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"", "m10.cloudmqtt.com", "17904");
                    Serial.println(commandBuffer);
                    if (SIM800.sendCheckReply2(commandBuffer, "CONNECT OK", 30000) == 1)
                    {
                        Serial.println("TCP/IP Connected");
                        if(MQTTConnect()){
                           Serial.println("MQTT Connected");
                           if(MQTTsubscribe()){
                              Serial.print("MQTT subscribed");
                           }
                        }

 

                        // Closes the socket
                        Serial.println("\nClosing");
                        SIM800.sendCheckReply("AT+CIPCLOSE", "CLOSE OK", 10000);
                    }
                    else
                    {
                        Serial.println("Error openning the connection");
                    }
            }
            else
            {
                Serial.println("Error bringing up wireless connection");
            }
        }
        else
        {
            Serial.println("Error setting the APN");
        }
    }
    else
    {
        Serial.println("Error setting single connection mode");
    }

    SIM800.sendCheckReply("AT+CIPSHUT", "OK", 10000);
    delay(1000);
}

int  MQTTConnect() {

    SIM800.println("AT+CIPSEND");
    SIM800.getBuffer(buffer, sizeof(buffer), 10000); 
    if(buffer[0] == '>'){
       Serial.println("Found prompt");
 
      SIM800.write(0x10);
      MQTTProtocolNameLength = strlen(MQTTProtocolName);
      MQTTClientIDLength = strlen(MQTTClientID);
      MQTTUsernameLength = strlen(MQTTUsername);
      MQTTPasswordLength = strlen(MQTTPassword);
      datalength = MQTTProtocolNameLength + 2 + 4 + MQTTClientIDLength + 2 + MQTTUsernameLength + 2 + MQTTPasswordLength + 2;
      X = datalength;
      do
      {
        encodedByte = X % 128;
        X = X / 128;
        // if there are more data to encode, set the top bit of this byte
        if ( X > 0 ) {
          encodedByte |= 128;
        }
  
        SIM800.write(encodedByte);
      }
      while ( X > 0 );
      
      SIM800.write(MQTTProtocolNameLength >> 8);
      SIM800.write(MQTTProtocolNameLength & 0xFF);
      SIM800.write((char*)MQTTProtocolName);
  
      SIM800.write(MQTTLVL); // LVL
      SIM800.write(MQTTFlags); // Flags
      SIM800.write(MQTTKeepAlive >> 8);
      SIM800.write(MQTTKeepAlive & 0xFF);
  
  
      SIM800.write(MQTTClientIDLength >> 8);
      SIM800.write(MQTTClientIDLength & 0xFF);
      SIM800.print((char*)MQTTClientID);
  
  
      SIM800.write(MQTTUsernameLength >> 8);
      SIM800.write(MQTTUsernameLength & 0xFF);
      SIM800.print((char*)MQTTUsername);
  
  
      SIM800.write(MQTTPasswordLength >> 8);
      SIM800.write(MQTTPasswordLength & 0xFF);
      SIM800.print((char*)MQTTPassword);
  
      SIM800.write(0x1A);
      Serial.println("Sending check ");
      
      SIM800.getBuffer(buffer, sizeof(buffer), 20000); 
      if(strcmp(buffer,"SEND OK") == 0){

//      if (SIM800.checkReply("SEND OK", 10000)) {
        Serial.println(F("CONNECT PACKET SUCCESS"));
        return 1;
      }
      else{
        Serial.println("Failed to send packet");
        return 0;
      }
    }
    else{
    Serial.println("Did not find prompt");
    return 0;
  }
}


int  MQTTpublish() {
  if (SIM800.sendCheckReply("AT+CIPSEND", ">", 20000)) {

    memset(buffer, 0, sizeof(buffer));

    topiclength = sprintf((char*)topic, MQTTTopic);
    //datalength = sprintf((char*)buffer,"%s%u",topic,Counter);
    datalength = sprintf((char*)buffer, "%s%s", topic, "Counter$$%%^&&&&**&^%$#");

    delay(1000);
    SIM800.write(0x30);
    X = datalength + 2;
    do
    {
      encodedByte = X % 128;
      X = X / 128;
      // if there are more data to encode, set the top bit of this byte
      if ( X > 0 ) {
        encodedByte |= 128;
      }
      SIM800.write(encodedByte);
    }
    while ( X > 0 );

    SIM800.write(topiclength >> 8);
    SIM800.write(topiclength & 0xFF);
    SIM800.print(buffer);
    SIM800.write(0x1A);
    if (SIM800.sendCheckReply("", "SEND OK", 5000)) {
      Serial.println(F("PUBLISH PACKET SENT"));
      return 1;
    }
    else{
        Serial.println("Did not find prompt");
        return 0;
      }
    }
}


boolean MQTTsubscribe() {

  if (SIM800.sendCheckReply("AT+CIPSEND", ">", 1000)) {

    memset(buffer, 0, 255);
    topiclength2 = strlen(MQTTTopic2);
    datalength = 2 + 2 + topiclength2 + 1;
    delay(1000);

    SIM800.write(0x82);
    X = datalength;
    do
    {
      encodedByte = X % 128;
      X = X / 128;
      // if there are more data to encode, set the top bit of this byte
      if ( X > 0 ) {
        encodedByte |= 128;
      }
      SIM800.write(encodedByte);
    }
    while ( X > 0 );
    SIM800.write(MQTTPacketID >> 8);
    SIM800.write(MQTTPacketID & 0xFF);
    SIM800.write(topiclength2 >> 8);
    SIM800.write(topiclength2 & 0xFF);
    SIM800.print((char*)MQTTTopic2);
    SIM800.write(MQTTQOS);

    SIM800.write(0x1A);
    if (SIM800.sendCheckReply("", "SEND OK", 20000)) {
      Serial.println(F("SUBSCRIBE PACKET SENT"));
      return 1;
    }
    else return 0;
  }
}



