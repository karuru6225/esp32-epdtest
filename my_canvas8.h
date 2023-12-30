
#ifndef MYCANVAS8_H
#define MYCANVAS8_H

#include "Arduino.h"
#include <Adafruit_GFX.h>

class MyCanvas8 : public GFXcanvas8
{
public:
  MyCanvas8(uint16_t w, uint16_t h);

  void writeLines(const char *str);
};

#endif