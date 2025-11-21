
#include "ApplicationFrame.h"

namespace slimenano::ui {

ApplicationFrame::ApplicationFrame(const wxString& title) :
    wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1280, 720)) {
    Centre();
    Maximize(true);
}

} // namespace slimenano::ui
