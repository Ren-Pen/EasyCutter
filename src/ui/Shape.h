#ifndef EASY_CUTTER_SRC_UI_SHAPE_H
#define EASY_CUTTER_SRC_UI_SHAPE_H

#include <cstdint>

namespace slimenano::ui {

struct Rect {
  uint32_t x;
  uint32_t y;
  uint32_t width;
  uint32_t height;
};

} // namespace slimenano::ui

#endif