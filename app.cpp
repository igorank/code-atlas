#include "app.h"
#include "main_frame.h"

bool App::OnInit()
{
    MainFrame* frame = new MainFrame("CodeAtlas", wxDefaultPosition, wxSize(640, 480));
    frame->Show(true);
    return true;
}