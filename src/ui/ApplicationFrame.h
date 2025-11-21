
#ifndef EASY_CUTTER_SRC_UI_APPLICATION_FRAME_H
#define EASY_CUTTER_SRC_UI_APPLICATION_FRAME_H

#include <wx/wx.h>

namespace slimenano::ui {

class ApplicationFrame : public wxFrame {
public:
    ApplicationFrame(const wxString& title);
};

} // namespace slimenano::ui

#endif
