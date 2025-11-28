#include <wx/intl.h>
#include <wx/app.h>
#include <wx/translation.h>
#include "ui/ApplicationFrame.h"

wxLocale g_locale;

class MainApp : public wxApp {
public:
    virtual bool OnInit() override {

        g_locale.Init(wxLANGUAGE_ENGLISH);
        wxLocale::AddCatalogLookupPathPrefix("locale");
        g_locale.AddCatalog("lang");

        auto frame = new slimenano::ui::ApplicationFrame(_("Easy Cutter"));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MainApp);
