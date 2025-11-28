#include "../../app/IDs.h"
#include "VideoPanel.h"

namespace slimenano::ui {

VideoPanel::VideoPanel(wxFrame* pParentFrame) :
    wxPanel(pParentFrame, ID_PANEL_VIDEO_PANEL), m_pParentFrame(pParentFrame) {
}

} // namespace slimenano::ui
