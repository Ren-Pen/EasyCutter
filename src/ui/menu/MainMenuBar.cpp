
#include <wx/menuitem.h>
#include <wx/defs.h>
#include <wx/translation.h>
#include <wx/event.h>

#include "../../app/IDs.h"
#include "MainMenuBar.h"

namespace slimenano::ui {

MainMenuBar::MainMenuBar(wxFrame* pFrame) :
    m_pFrame(pFrame) {

    auto menuFile = new wxMenu;

    menuFile->Append(ID_MENU_FILE_OPEN, _("Open"));
    menuFile->AppendSeparator();
    menuFile->Append(ID_MENU_FILE_EXIT, _("Exit"));

    Append(menuFile, _("&File"));
}

} // namespace slimenano::ui
