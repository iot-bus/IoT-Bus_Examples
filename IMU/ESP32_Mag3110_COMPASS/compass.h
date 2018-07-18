#include <TFT_eSPI.h> // Hardware-specific library

#include <SparkFun_MAG3110.h> // Magnetometer

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

/* multiplicative conversion constants */
#define DegToRad 0.017453292F
#define RadToDeg 57.295779F

class Compass {

private:

  char* _caption; 
  TFT_eSPI* _tft;
  int centerX  = 160;
  int centerY  = 120;
  const int radius = 100;   
  int last_dx, last_dy, dx, dy;
  String oldHeading;
  String oldArrowHeading;    
  
public:
  float declination;

  Compass(TFT_eSPI* tft);
  void drawRose(bool calibrated);
  void drawHeading(double heading);
  void setCaption(char* caption);
  void displayString(int x, int y, String token, int txt_color, int txt_size);
  void arrow(int x2, int y2, int x1, int y1, int alength, int awidth, int color);
  ~Compass() {  }

};
