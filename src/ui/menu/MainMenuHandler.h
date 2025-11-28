
#ifndef EASY_CUTTER_SRC_UI_MENU_MAIN_MENU_HANDLER_H
#define EASY_CUTTER_SRC_UI_MENU_MAIN_MENU_HANDLER_H

#include <wx/event.h>
#include <wx/frame.h>

namespace slimenano::ui {

class MainMenuHandler final {

public:
    MainMenuHandler(wxFrame* pFrame);

    void OnExit(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);

private:
    wxFrame* m_pFrame;
};

} // namespace slimenano::ui

#endif
