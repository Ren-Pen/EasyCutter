
#ifndef EASY_CUTTER_SRC_UI_APPLICATION_FRAME_H
#define EASY_CUTTER_SRC_UI_APPLICATION_FRAME_H

#include <wx/frame.h>
#include "menu/MainMenuBar.h"
#include "menu/MainMenuHandler.h"

namespace slimenano::ui {

class ApplicationFrame : public wxFrame {
public:
    ApplicationFrame(const wxString& title);
    ~ApplicationFrame();

private:
    MainMenuBar* m_pMainMenuBar;
    MainMenuHandler* m_pMainMenuHandler;
};

} // namespace slimenano::ui

#endif
