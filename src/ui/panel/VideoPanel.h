#ifndef EASY_CUTTER_SRC_UI_PANEL_VIDEO_PANEL_H
#define EASY_CUTTER_SRC_UI_PANEL_VIDEO_PANEL_H

#include <wx/frame.h>
#include <wx/panel.h>

namespace slimenano::ui {

class VideoPanel : public wxPanel {
public:
    VideoPanel(wxFrame* pParentFrame);

private:
    wxFrame* m_pParentFrame;
};

} // namespace slimenano::ui

#endif
