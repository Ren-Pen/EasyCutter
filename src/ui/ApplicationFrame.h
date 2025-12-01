
#ifndef EASY_CUTTER_SRC_UI_APPLICATION_FRAME_H
#define EASY_CUTTER_SRC_UI_APPLICATION_FRAME_H

#include <wx/frame.h>
#include "menu/MainMenuBar.h"
#include "menu/MainMenuHandler.h"

#include "panel/VideoPanel.h"

namespace slimenano::ui {

class ApplicationFrame : public wxFrame {
public:
    ApplicationFrame(const wxString& title);
    ~ApplicationFrame();

    VideoPanel* GetVideoPanel();

private:
    MainMenuBar* m_pMainMenuBar;
    MainMenuHandler* m_pMainMenuHandler;
    VideoPanel* m_pVideoPanel;
};

} // namespace slimenano::ui

#endif
