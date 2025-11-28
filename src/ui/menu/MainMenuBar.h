#ifndef EASY_CUTTER_SRC_UI_MENU_MAIN_MENU_BAR_H
#define EASY_CUTTER_SRC_UI_MENU_MAIN_MENU_BAR_H

#include <wx/menu.h>
#include <wx/frame.h>

namespace slimenano::ui {

class MainMenuBar final : public wxMenuBar {

public:
    MainMenuBar(wxFrame* pFrame);

private:
    wxFrame* m_pFrame;
};

} // namespace slimenano::ui

#endif
