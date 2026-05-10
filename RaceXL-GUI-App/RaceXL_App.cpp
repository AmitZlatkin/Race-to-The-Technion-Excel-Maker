#include <wx/wx.h>
#include "MainFrame.h"

class RaceXL_App : public wxApp
{
public:
    bool OnInit() override
    {
        if (!wxApp::OnInit())
            return false;
        auto* frame = new MainFrame();
        frame->SetIcon(wxICON(AppIcon));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(RaceXL_App);