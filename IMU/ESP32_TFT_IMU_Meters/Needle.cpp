#include "Needle.h"

Needle::Needle(int x, int y, int dx, int dy)
{
  _x = x;
  _y = y;
  _dx = dx;
  _dy = dy;
  _oldValue = -999;
  _ltx = 0;
  _osx = dx/2;
  _osy = y+dy-6;
}

// #########################################################################
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
// #########################################################################
void Needle::draw(TFT_eSPI tft, char* caption, float scaleFactor, int value, int ms_delay)
{
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  char buf[8]; dtostrf(value, 4, 0, buf);
  tft.drawRightString(buf, _dx/6, _y+_dy-25, 2);

  value *= scaleFactor;

  if (value < -10) value = -10; // Limit value to emulate needle end stops
  if (value > 110) value = 110;


  // Move the needle util new value reached
  while (!(value == _oldValue)) {
    if (_oldValue < value) _oldValue++;
    else _oldValue--;

    if (ms_delay == 0) _oldValue = value; // Update immediately id delay is 0

    float sdeg = map(_oldValue, -10, 110, -150, -30); // Map value to angle
    // Calcualte tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    // Erase old needle image
    tft.drawLine(_dx/2 + 20 * _ltx - 1, _y + 120 - 20, _osx - 1, _osy, TFT_WHITE);
    tft.drawLine(_dx/2 + 20 * _ltx, _y + 120 - 20, _osx, _osy, TFT_WHITE);
    tft.drawLine(_dx/2 + 20 * _ltx + 1, _y + 120 - 20, _osx + 1, _osy, TFT_WHITE);

    // Re-plot text under needle
    tft.setTextColor(TFT_BLACK);
    tft.drawCentreString(caption, _x+_dx/2, _y+_dy*2/3, 4); // // Comment out to avoid font 4

    // Store new needle end coords for next erase
    _ltx = tx;
    _osx = sx * 98 + _dx/2;
    _osy = sy * 98 + _y + 140;

    // Draw the needle in the new postion, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    tft.drawLine(_dx/2 + 20 * _ltx - 1, _y + 120 - 20, _osx - 1, _osy, TFT_RED);
    tft.drawLine(_dx/2 + 20 * _ltx, _y + 120 - 20, _osx, _osy, TFT_MAGENTA);
    tft.drawLine(_dx/2 + 20 * _ltx + 1, _y + 120 - 20, _osx + 1, _osy, TFT_RED);

    // Slow needle down slightly as it approaches new postion
    if (abs(_oldValue - value) < 10) ms_delay += ms_delay / 5;

    // Wait before next update
    delay(ms_delay);
  }
}


