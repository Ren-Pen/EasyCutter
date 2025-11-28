#include <wx/msgdlg.h>
#include "MainMenuHandler.h"

#include "../IDs.h"

namespace slimenano::ui {

MainMenuHandler::MainMenuHandler(wxFrame* pFrame) :
    m_pFrame(pFrame) {

    m_pFrame->Bind(wxEVT_MENU, &MainMenuHandler::OnOpen, this, ID_MENU_FILE_OPEN);
    m_pFrame->Bind(wxEVT_MENU, &MainMenuHandler::OnExit, this, ID_MENU_FILE_EXIT);
}

void MainMenuHandler::OnOpen([[maybe_unused]] wxCommandEvent& event) {
    wxMessageBox(_("Hello world!"), "About Hello World", wxOK | wxICON_INFORMATION);
}

void MainMenuHandler::OnExit([[maybe_unused]] wxCommandEvent& event) {
    m_pFrame->Close(true);
}

} // namespace slimenano::ui
