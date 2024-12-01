#pragma once
#include <wx/wx.h>
#include <wx/dataview.h>
#include "database.h"
#include "book_dialog.h"
#include "about_dialog.h"

class MainFrame : public wxFrame
{
private:
	wxDataViewListCtrl* bookList;
	Database db;
	
	void LoadBooks();

	void OnAddBook(wxCommandEvent& event);
	void OnEditBook(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnDeleteBook(wxCommandEvent& event);
	
	void OnKeyDown(wxKeyEvent& event);

public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
};

