#include <wx/wx.h>
#include "ui/ApplicationFrame.h"

class MainApp : public wxApp {
public:
    virtual bool OnInit() override {
        auto frame = new slimenano::ui::ApplicationFrame(wxT("Easy Cutter"));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MainApp);
