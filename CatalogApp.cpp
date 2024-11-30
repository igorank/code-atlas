#include "CatalogApp.h"
#include "Frame.h"

wxIMPLEMENT_APP(CatalogApp);

bool CatalogApp::OnInit()
{
    Frame* frame = new Frame("CodeAtlas", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}