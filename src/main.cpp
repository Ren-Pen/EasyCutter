#include <fmt/core.h>
#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        fmt::print("Hello World!\n");

        wxFrame* frame = new wxFrame(
            nullptr,
            wxID_ANY,
            wxT("Title"),
            wxDefaultPosition,
            wxSize(250, 150)
        );

        frame->Centre();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);

