#include <TFT_eSPI.h> // Hardware-specific library
#include "Needle.h"

#define TFT_GREY 0x5AEB

class Meter {
private:
  int _x; 
  int _y;
  int _dx;
  int _dy;
  int _tickValue;
  float _scaleFactor;

  char* _caption;
  char* _units;
  
  Needle* _needle;
  TFT_eSPI _tft;
  
public:
  Meter(int x, int y, int dx, int dy, int tickValue, float scaleFactor);
  void draw(TFT_eSPI tft);
  void drawNeedle(int value, int ms_delay);
  void setCaption(char* caption);
  void setUnits(char* units);
  ~Meter() { delete _needle; delete _caption; delete _units; }

};
