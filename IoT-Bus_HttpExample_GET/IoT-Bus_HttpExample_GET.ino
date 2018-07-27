#include <ArduinoJson.h>
#include <Http.h>

unsigned long lastRunTime = 0;
unsigned long waitForRunTime = 0;

#define RST_PIN 33
HTTP http(9600, RST_PIN);

const char contentType[] = "application/json";
const char header[] = "Authorization : bearer c.oQojUMUIPbSPBh7n0SHZJ5MPrE0rqb8mA3K57qfkHmJpWmodpIaeSQGQ9kuKmB5h0dIDTgl6Hj2hquYDDwotbVEYTL8ryRyW0pEGq2WU4fat6KnzGXvS1Ugbw17bbk86tlKM3ymTLHu9nTsJ";

// the setup routine runs once when you press reset:
void setup() {
  
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Starting!");
  doGet();
}

// the loop routine runs over and over again forever:
void loop() {
  
}

void print(const __FlashStringHelper *message, int code = -1){
  if (code != -1){
    Serial.print(message);
    Serial.println(code);
  }
  else {
    Serial.println(message);
  }
}

void doGet(){
  http.powerDownNow();
  http.preInit();
  char response[1024];
  Result result;

  result = http.disconnect(); // will get error on connect if currently connected
  print(F("Configure bearer: "), http.configureBearer("pwg"));
  result = http.connect();
  print(F("HTTP connect: "), result);

  http.setContentType(contentType);
  //http.setHeader(header);

  result = http.get("https://developer-api.nest.com/?auth=c.oQojUMUIPbSPBh7n0SHZJ5MPrE0rqb8mA3K57qfkHmJpWmodpIaeSQGQ9kuKmB5h0dIDTgl6Hj2hquYDDwotbVEYTL8ryRyW0pEGq2WU4fat6KnzGXvS1Ugbw17bbk86tlKM3ymTLHu9nTsJ", response);
  print(F("HTTP GET: "), result);
  if (result == SUCCESS) {
    Serial.println(response);
    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response);
    lastRunTime = millis();
    waitForRunTime = root["waitForRunTime"];
    
    print(F("Last run time: "), lastRunTime);
    print(F("Next post in: "), waitForRunTime);
  }
  
  print(F("HTTP disconnect: "), http.disconnect());
}
