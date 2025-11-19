#ifndef EASY_CUTTER_SRC_UI_I_GRAPHICS_DEVICE_H
#define EASY_CUTTER_SRC_UI_I_GRAPHICS_DEVICE_H

namespace slimenano::ui {

class IGraphicsDevice {

public:
    virtual ~IGraphicsDevice() = default;

    virtual void drawLine() = 0;


};

} // namespace slimenano::ui

#endif