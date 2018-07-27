#include <arduino.h>
#include <hardwareserial.h>

class IOT_BUS_SIM800 {

  private:
    uint8_t _resetPin;
    HardwareSerial* _SIM800;

    char replybuffer[255];
   
    void flush();
  
  public: 
    IOT_BUS_SIM800(HardwareSerial* SIM800, uint8_t resetPin);
    boolean begin(void);
    boolean powerOn(void);
    boolean sendCheckReply(char *send, char *reply, uint16_t timeout);
    boolean sendCheckReply2(char *send, char *reply, uint16_t timeout);
    uint8_t getReply(char *send, uint16_t timeout);
    uint8_t getReply2(char *send, uint16_t timeout);
    uint8_t readline(uint16_t timeout, boolean multiline = false);
    uint8_t getBuffer(char* buffer, int length, uint16_t timeout, boolean multiline = false);

    boolean available(void);
    char read(void);
    void println(char* str);
    void printReply(void);
    void getBuffer(char* buffer, int length);


};

