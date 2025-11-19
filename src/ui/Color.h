#ifndef EASY_CUTTER_SRC_UI_COLOR_H
#define EASY_CUTTER_SRC_UI_COLOR_H

#include <cstdint>

namespace slimenano::ui {

struct Color {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t a = 255;

  static inline Color parse(const char *buf, size_t bufSize) {
    if (!buf || bufSize <= 0) {
      return {0, 0, 0, 255};
    }
    if (buf[0] == '#') {
      auto hexColor = Color{0, 0, 0, 255};
      for (size_t i = 1; i < bufSize; i++) {
        
      }
      return hexColor;
    }
  }
};

} // namespace slimenano::ui

#endif