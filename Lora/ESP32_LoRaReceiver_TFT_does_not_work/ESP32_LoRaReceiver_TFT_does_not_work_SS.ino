#include <SPI.h>
#include <LoRa.h>

#define DIO0 35
#define SS   5
#define RESET 17

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 27
#define TFT_CS 5

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 0

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {

  LoRa.setPins(SS, RESET, DIO0);
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

//  if (!LoRa.begin(915E6)) {
//    Serial.println("Starting LoRa failed!");
//    while (1);
//  }

    // Turn on backlight
  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH);
 
  tft.begin();

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  
  Serial.println(F("Benchmark                Time (microseconds)"));
  delay(10);
  Serial.print(F("Screen fill              "));
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  tft.setTextColor(ILI9341_WHITE); 
  tft.setTextSize(2);
  tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);
   tft.println(1234.56);

  Serial.println(F("Done!"));

}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}



