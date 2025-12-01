
#include "../pch.h" // IWYU pragma: keep

#include "IDs.h"
#include "ApplicationFrame.h"

namespace slimenano::ui {

ApplicationFrame::ApplicationFrame(const wxString& title) :
    wxFrame(nullptr, ID_FRAME_APPLICATON, title, wxDefaultPosition, wxSize(1280, 720)) {
    Centre();

    m_pMainMenuBar = new MainMenuBar{this};
    SetMenuBar(m_pMainMenuBar);

    m_pMainMenuHandler = new MainMenuHandler(this);

    m_pVideoPanel = new VideoPanel(this);

    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_pVideoPanel, 1, wxEXPAND | wxALL, 0);
    SetSizer(sizer);
    Layout();
}

ApplicationFrame::~ApplicationFrame() {
    delete m_pMainMenuHandler;
}

VideoPanel* ApplicationFrame::GetVideoPanel() {
    return this->m_pVideoPanel;
}

} // namespace slimenano::ui
