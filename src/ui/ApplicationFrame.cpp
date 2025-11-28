
#include "../app/IDs.h"
#include "ApplicationFrame.h"

namespace slimenano::ui {

ApplicationFrame::ApplicationFrame(const wxString& title) :
    wxFrame(nullptr, ID_FRAME_APPLICATON, title, wxDefaultPosition, wxSize(1280, 720)) {
    Centre();

    m_pMainMenuBar = new MainMenuBar{this};
    SetMenuBar(m_pMainMenuBar);

    m_pMainMenuHandler = new MainMenuHandler(this);
}

ApplicationFrame::~ApplicationFrame() {
    delete m_pMainMenuHandler;
}

} // namespace slimenano::ui
