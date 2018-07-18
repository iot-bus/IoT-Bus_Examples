
#include "Meter.h"
#include <stdio.h> 

Meter::Meter(int x, int y, int dx, int dy, int tickValue = 5, float scaleFactor = 1)
{
  #include <TFT_eSPI.h> // Hardware-specific library

  _x = x;
  _y = y;
  _dx = dx;
  _dy = dy;
  _tickValue = tickValue;
  _needle = new Needle(_x, _y, _dx, _dy);
  _scaleFactor = scaleFactor;
}

// #########################################################################
//  Draw the analogue meter on the screen
// #########################################################################
void Meter::draw(TFT_eSPI tft)
{
  // Meter outline
  _tft = tft;

  tft.fillRect(_x, _y, _dx, _dy, TFT_GREY);
  tft.fillRect(_x+5, _y+3, _dx-9, _dy-7, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);  // Text colour

  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for (int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 15;

    // Coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (100 + tl) + _dx/2;
    uint16_t y0 = sy * (100 + tl) + _y + 140;
    uint16_t x1 = sx * 100 + _dx/2;
    uint16_t y1 = sy * 100 + _y + 140;

    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (100 + tl) + _dx/2;
    int y2 = sy2 * (100 + tl) + _y + 140;
    int x3 = sx2 * 100 + _dx/2;
    int y3 = sy2 * 100 + _y + 140;

    // Yellow zone limits
    //if (i >= -50 && i < 0) {
    //  tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_YELLOW);
    //  tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_YELLOW);
    //}

    // Green zone limits
    if (i >= 0 && i < 25) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREEN);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREEN);
    }

    // Orange zone limits
    if (i >= 25 && i < 50) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_ORANGE);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_ORANGE);
    }

    // Short scale tick length
    if (i % 25 != 0) tl = 8;

    // Recalculate coords incase tick length changed
    x0 = sx * (100 + tl) + _dx/2;
    y0 = sy * (100 + tl) + _y + 140;
    x1 = sx * 100 + _dx/2;
    y1 = sy * 100 + _y + 140;

    // Draw tick
    tft.drawLine(x0, y0, x1, y1, TFT_BLACK);

    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (100 + tl + 10) + _dx/2;
      y0 = sy * (100 + tl + 10) + _y + 140;

      char buffer[] = "0000";
      
      switch (i / 25) {
        case -2: tft.drawCentreString("0", x0, y0 - 12, 2); break;
        case -1: snprintf(buffer, sizeof(buffer), "%d", _tickValue*5); tft.drawCentreString(buffer, x0, y0 - 9, 2); break;
        case 0: snprintf(buffer, sizeof(buffer), "%d", _tickValue*10); tft.drawCentreString(buffer, x0, y0 - 6, 2); break;
        case 1: snprintf(buffer, sizeof(buffer), "%d", _tickValue*15); tft.drawCentreString(buffer, x0, y0 - 9, 2); break;
        case 2: snprintf(buffer, sizeof(buffer), "%d", _tickValue*20); tft.drawCentreString(buffer, x0, y0 - 12, 2); break;
      }
    }

    // Now draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = sx * 100 + _dx/2;
    y0 = sy * 100 + _y + 140;
    // Draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x0, y0, x1, y1, TFT_BLACK);
  }
  if(_units)
    tft.drawString(_units, _x+_dx - 43, _y+_dy - 25, 2); // Units at bottom right
  if(_caption)  
    tft.drawCentreString(_caption, _x+_dx/2, _y+_dy*2/3, 4); // Comment out to avoid font 4
  tft.drawRect(_x+5, _x+3, _x+_dx-9, _y+_dy-7, TFT_BLACK); // Draw bezel line

}

void Meter::drawNeedle(int value, int ms_delay){
  _needle->draw(_tft, _caption, _scaleFactor, value, ms_delay);
}

void Meter::setCaption(char* caption){
  _caption = (char*)malloc(strlen(caption)); 
  strcpy(_caption, caption);
}

void Meter::setUnits(char* units){
  _units = (char*)malloc(strlen(units));
  strcpy(_units, units);
}



