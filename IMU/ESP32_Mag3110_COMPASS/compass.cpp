#include "compass.h"


Compass::Compass(TFT_eSPI* tft){
  _tft = tft;
}

void Compass::drawRose(bool calibrated) {

  // width and height of current rotation
  centerX = _tft->width()/2;
  centerY = _tft->height()/2;
  int dxo, dyo, dxi, dyi;
  _tft->drawCircle(centerX, centerY, radius,YELLOW);  // Draw compass circle
  for (float i = 0; i <360; i = i + 22.5) {
    dxo = radius * cos((i-90)*3.14/180);
    dyo = radius * sin((i-90)*3.14/180);
    dxi = dxo * 0.9;
    dyi = dyo * 0.9;
    _tft->drawLine(dxi+centerX,dyi+centerY,dxo+centerX,dyo+centerY,YELLOW);   
  }
  displayString((centerX-5),(centerY-85),"N",RED,2);
  displayString((centerX-5),(centerY+70),"S",RED,2);
  displayString((centerX+80),(centerY-5),"E",RED,2);
  displayString((centerX-85),(centerY-5),"W",RED,2);
  if(!calibrated){
    displayString((centerX-70), (centerY-5), "Uncalibrated", BLUE, 2);       
   }
  else {
    displayString((centerX-70), (centerY-5), "Uncalibrated", BLACK, 2);   
  }
}

void Compass::drawHeading(double heading){
  double arrowHeading = heading;
  //arrowHeading = -arrowHeading; // we want to draw north not the heading
  //if (heading < 0)
  //  arrowHeading -= 1.5708;
  //else  
    arrowHeading -= 1.5708; // adjust for screen orientation;
  dx = (radius * cos(arrowHeading)) + centerX;     // calculate X position 
  dy = (radius * sin(arrowHeading)) + centerY;     // calculate Y position 
  arrow(last_dx,last_dy, centerX, centerY, 20, 20,BLACK);  // Erase last arrow      
  arrow(dx,dy, centerX, centerY, 20, 20,CYAN);             // Draw arrow in new position
  last_dx = dx; 
  last_dy = dy;

  int iHeading = heading*RadToDeg;
  if (iHeading < 0)
    iHeading += 360;

  String sHeading = String(iHeading);
  displayString((centerX+20), (centerY-5), oldHeading, BLACK, 2); 
  displayString((centerX+20), (centerY-5), sHeading, BLUE, 2);
  oldHeading = sHeading;
  
  iHeading = arrowHeading*RadToDeg;
  if (iHeading < 0)
    iHeading += 360;

  String sArrowHeading = String(iHeading);
  displayString((centerX-60), (centerY-5), oldArrowHeading, BLACK, 2); 
  displayString((centerX-60), (centerY-5), sArrowHeading, BLUE, 2);
  oldArrowHeading = sArrowHeading;
   
}

void Compass::arrow(int x2, int y2, int x1, int y1, int alength, int awidth, int color) {
  float distance;
  int dx, dy, x2o,y2o,x3,y3,x4,y4,k;
  distance = sqrt(pow((x1 - x2),2) + pow((y1 - y2), 2));
  dx = x2 + (x1 - x2) * alength / distance;
  dy = y2 + (y1 - y2) * alength / distance;
  k = awidth / alength;
  x2o = x2 - dx;
  y2o = dy - y2;
  x3 = y2o * k + dx;
  y3 = x2o * k + dy;
  //
  x4 = dx - y2o * k;
  y4 = dy - x2o * k;
  _tft->drawLine(x1, y1, x2, y2, color);
  _tft->drawLine(x1, y1, dx, dy, color);
  _tft->drawLine(x3, y3, x4, y4, color);
  _tft->drawLine(x3, y3, x2, y2, color);
  _tft->drawLine(x2, y2, x4, y4, color);
} 

void Compass::displayString(int x, int y, String token, int txt_color, int txt_size) {
  _tft->setCursor(x, y);
  _tft->setTextColor(txt_color);
  _tft->setTextSize(txt_size);
  _tft->print(token);
  _tft->setTextSize(2); // Back to default text size
}
