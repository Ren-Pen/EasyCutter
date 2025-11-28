#ifndef EASY_CUTTER_SRC_APP_VIDEO_CONTROLLER_H
#define EASY_CUTTER_SRC_APP_VIDEO_CONTROLLER_H

#include <wx/event.h>
#include "../../ui/panel/VideoPanel.h"

namespace slimenano {

class VideoController : public wxEvtHandler {

public:
    VideoController(ui::VideoPanel* pVideoPanel);

private:
    void OnOpenVideo(wxCommandEvent& event);
    void OnPlayVideo(wxCommandEvent& event);
    void OnStopVideo(wxCommandEvent& event);
    ui::VideoPanel* m_pVideoPanel;
};

} // namespace slimenano

#endif
