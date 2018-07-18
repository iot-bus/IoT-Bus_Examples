#include "IOT_BUS_SIM800.h"    
    
    IOT_BUS_SIM800::IOT_BUS_SIM800(HardwareSerial* SIM800, uint8_t resetPin){
      _resetPin = resetPin;
      pinMode(_resetPin, OUTPUT);
      _SIM800 = SIM800;
    };

boolean  IOT_BUS_SIM800::begin(){
      _SIM800->begin(9600);
      Serial.println("Starting...");
      if(!powerOn()){
        Serial.println("Failed to power on");
        return false;
      }
      // sets the PIN code
      //sendCheckReply("AT+CPIN=****", "OK", 2000);
  
      Serial.println("Connecting to the network...");
      Serial.println("Registering");
      if( sendCheckReply("AT+CREG?", "+CREG: 0,1", 1000)){
          Serial.println(replybuffer);
          sendCheckReply("AT+CREG=1", "OK", 1000);
          sendCheckReply("AT+CREG?", "+CREG: 0,1", 1000);
          Serial.println(replybuffer);
      }
      else{
        Serial.println(replybuffer);
      }
      return true;
    }


boolean IOT_BUS_SIM800::powerOn(){

    uint8_t answer=0;

    // checks if the module is started

    // give it a chance to wake up
    for(int i=0; i<3; i++)
    {
      sendCheckReply("AT", "OK", 500);
    }

    answer = sendCheckReply("AT", "OK", 500);

    // turn off echo
    
    for(int i=0; i<3; i++)
    {
      answer = sendCheckReply("ATE0", "OK", 500);
    }
    
    if (answer == 0)
    {
        Serial.println("power cycle");
        // power on pulse
        digitalWrite(_resetPin,HIGH);
        delay(100);
        digitalWrite(_resetPin,LOW);
        delay(1200);
        digitalWrite(_resetPin,HIGH);
        // waits for an answer from the module
        int i=0;
        while(answer == 0 && i <20){     // Send AT every two seconds and wait for the answer
            answer = sendCheckReply("AT", "AT", 2000); // ECHO will be on at this point
            answer = sendCheckReply("ATE0", "OK", 500); // now turn off echo
            i++;
        }
    }
    else{
      return answer;
    }

}

boolean IOT_BUS_SIM800::sendCheckReply(char *send, char *reply, uint16_t timeout) {
  if (! getReply(send, timeout) ){
    Serial.println("No reply");
    return false;
  }
/*
  for (uint8_t i=0; i<strlen(replybuffer); i++) {
  DEBUG_PRINT(replybuffer[i], HEX); DEBUG_PRINT(" ");
  }
  DEBUG_PRINTLN();
  for (uint8_t i=0; i<strlen(reply); i++) {
    DEBUG_PRINT(reply[i], HEX); DEBUG_PRINT(" ");
  }
  DEBUG_PRINTLN();
  */
  Serial.println(replybuffer);
  return (strcmp(replybuffer, reply) == 0);
}

boolean IOT_BUS_SIM800::sendCheckReply2(char *send, char *reply, uint16_t timeout) {
  if (! getReply2(send, timeout) ){
    Serial.println("No reply");
    return false;
  }
/*
  for (uint8_t i=0; i<strlen(replybuffer); i++) {
  DEBUG_PRINT(replybuffer[i], HEX); DEBUG_PRINT(" ");
  }
  DEBUG_PRINTLN();
  for (uint8_t i=0; i<strlen(reply); i++) {
    DEBUG_PRINT(reply[i], HEX); DEBUG_PRINT(" ");
  }
  DEBUG_PRINTLN();
  */
  Serial.println(replybuffer);
  return (strcmp(replybuffer, reply) == 0);
}

uint8_t IOT_BUS_SIM800::getBuffer(char* buffer, int length, uint16_t timeout, boolean multiline){
   uint8_t retcode = readline(timeout, multiline);
   strncpy(buffer, replybuffer, length); 
   return retcode;
}

uint8_t IOT_BUS_SIM800::readline(uint16_t timeout, boolean multiline) {
  uint16_t replyidx = 0;

  while (timeout--) {
    if (replyidx >= 254) {
      //DEBUG_PRINTLN(F("SPACE"));
      break;
    }

    while(_SIM800->available()) {
      char c =  _SIM800->read();
      if (c == '\r') continue;
      if (c == 0xA) {
        if (replyidx == 0)   // the first 0x0A is ignored
          continue;

        if (!multiline) {
          timeout = 0;         // the second 0x0A is the end of the line
          break;
        }
      }
      replybuffer[replyidx] = c;
      //DEBUG_PRINT(c, HEX); DEBUG_PRINT("#"); DEBUG_PRINTLN(c);
      replyidx++;
    }

    if (timeout == 0) {
      //DEBUG_PRINTLN(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  replybuffer[replyidx] = 0;  // null term
  return replyidx;
}

uint8_t IOT_BUS_SIM800::getReply(char *send, uint16_t timeout) {
  flush();


 // DEBUG_PRINT(F("\t---> ")); DEBUG_PRINTLN(send);


  _SIM800->println(send);

  uint8_t l = readline(timeout);

 // DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return l;
}

uint8_t IOT_BUS_SIM800::getReply2(char *send, uint16_t timeout) {
  flush();


 // DEBUG_PRINT(F("\t---> ")); DEBUG_PRINTLN(send);


  _SIM800->println(send);

  readline(timeout); // eat OK
  uint8_t l = readline(timeout);

 // DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return l;
}

void IOT_BUS_SIM800::flush() {
    // Read all available serial input to flush pending data.
    uint16_t timeoutloop = 0;
    while (timeoutloop++ < 40) {
        while(_SIM800->available()) {
            _SIM800->read();
            timeoutloop = 0;  // If char was received reset the timer
        }
        delay(1);
    }
}

boolean IOT_BUS_SIM800::available(){
    return _SIM800->available();
}

char IOT_BUS_SIM800::read(){
    return _SIM800->read();
}

void IOT_BUS_SIM800::println(char* str){
    _SIM800->println(str);
}

void IOT_BUS_SIM800::printReply(void){
  Serial.println(replybuffer);
     
}

void IOT_BUS_SIM800::getBuffer(char* buffer, int length){
  strncpy(buffer, replybuffer, length);
}

