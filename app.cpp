#include "app.h"
#include "main_frame.h"

bool App::OnInit()
{
    wxInitAllImageHandlers();

    wxIcon appIcon("icon.ico", wxBITMAP_TYPE_ICO);

    MainFrame* frame = new MainFrame("CodeAtlas", wxDefaultPosition, wxSize(800, 600));
    frame->SetIcons(wxIconBundle(appIcon));
    frame->Show(true);
    return true;
}