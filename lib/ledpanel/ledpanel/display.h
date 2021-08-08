#ifndef _LEDPANEL_DISPLAY_H_
#define _LEDPANEL_DISPLAY_H_

#include <stdint.h>

namespace ledpanel {

using pos_t = int;

struct Color {
  Color(uint8_t r0, uint8_t g0, uint8_t b0) : r(r0), g(g0), b(b0) {}
  uint8_t r, g, b;
};

class Display {
 public:
  virtual void DrawPixel(pos_t y, pos_t x, const Color &c) = 0;
  virtual pos_t GetHeight() const = 0;
  virtual pos_t GetWidth() const = 0;
};

}  // namespace ledpanel

#endif