#pragma once
#include <wx/wx.h>

class App : public wxApp
{
public:
	virtual bool OnInit();
};
wxIMPLEMENT_APP(App);
