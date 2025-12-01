#include "../../pch.h" // IWYU pragma: keep

#include "MainMenuHandler.h"

#include "../ApplicationFrame.h"
#include "../IDs.h"
#include "../../app/Events.h"

namespace slimenano::ui {

MainMenuHandler::MainMenuHandler(wxFrame* pFrame) :
    m_pFrame(pFrame) {

    m_pFrame->Bind(wxEVT_MENU, &MainMenuHandler::OnOpen, this, ID_MENU_FILE_OPEN);
    m_pFrame->Bind(wxEVT_MENU, &MainMenuHandler::OnExit, this, ID_MENU_FILE_EXIT);
}

void MainMenuHandler::OnOpen([[maybe_unused]] wxCommandEvent& event) {
    auto fileDialog = wxFileDialog{
        this->m_pFrame,
        _("Open a media file"),
        "",
        "",
        _("Media files") + "|*.mp4;*.avi|" + _("All files") + "|*.*",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    };

    if (fileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    auto path = fileDialog.GetPath();
    wxCommandEvent openVideoEvent(EVT_OPEN_VIDEO);
    openVideoEvent.SetString(path);
    wxPostEvent(static_cast<ApplicationFrame*>(this->m_pFrame)->GetVideoPanel(), openVideoEvent);
}

void MainMenuHandler::OnExit([[maybe_unused]] wxCommandEvent& event) {
    m_pFrame->Close(true);
}

} // namespace slimenano::ui
