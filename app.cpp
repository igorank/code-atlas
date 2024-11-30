#include "app.h"
#include "main_frame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame* frame = new MainFrame("CodeAtlas", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}