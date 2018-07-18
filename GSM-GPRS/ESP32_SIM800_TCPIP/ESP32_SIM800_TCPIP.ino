#include "IOT_BUS_SIM800.h" 

int8_t answer;
int resetPin= 33;

char buffer[255];
char ip[16];

char apn[4] = "pwg";
//char apn[10] = "hologram";

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
                    sprintf(commandBuffer, "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"", "www.oddwires.com", "80");
                    Serial.println(commandBuffer);
                    if (SIM800.sendCheckReply2(commandBuffer, "CONNECT OK", 30000) == 1)
                    {
                        Serial.println("Connected");

                        // Sends some data to the TCP socket
                        sprintf(commandBuffer,"AT+CIPSEND=%i", strlen(get));
                        Serial.println(commandBuffer);
                        SIM800.println(commandBuffer);
                        SIM800.getBuffer(buffer, sizeof(buffer), 10000); 
                        if(buffer[0] == '>'){
                          sprintf(commandBuffer, "%s\x1A", get);
                          Serial.println("Sending data");
                          Serial.println(commandBuffer);
                          if(SIM800.sendCheckReply(commandBuffer, "SEND OK", 10000)){
                            Serial.println("Requesting data");
                            while(!SIM800.available());
                            while(SIM800.available()){
                              Serial.write(SIM800.read());
                            }
                            while(!SIM800.available());
                            while(SIM800.available()){
                              Serial.write(SIM800.read());
                            }
                            while(!SIM800.available());
                            while(SIM800.available()){
                              Serial.write(SIM800.read());
                            }

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


