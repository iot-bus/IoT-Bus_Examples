#include <TFT_eSPI.h> // Hardware-specific library

class Needle {
private:
  int _x; 
  int _y;
  int _dx;
  int _dy;
  int _oldValue;
  float _ltx;
  uint16_t _osx;
  uint16_t _osy;

public:
  Needle(int x, int y, int dx, int dy);
  void draw(TFT_eSPI tft, char* caption, float scaleFactor, int value, int ms_delay);
};
