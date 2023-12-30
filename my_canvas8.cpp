#include "my_canvas8.h"

MyCanvas8::MyCanvas8(uint16_t w, uint16_t h) : GFXcanvas8(w, h) {}

void MyCanvas8::writeLines(const char *str) {
  int16_t cur_x = getCursorX();
  int16_t cur_y;
  uint8_t c; // Current character
  while ((c = *str++)) {
    write(c);
    if (c == '\n') {
      cur_y = getCursorY();
      setCursor(cur_x, cur_y);
    }
  }
}