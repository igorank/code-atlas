#pragma once
#include <wx/wx.h>
#include <wx/dataview.h>
#include "database.h"
#include "book_dialog.h"
#include "about_dialog.h"
#include "utils.h"

class MainFrame : public wxFrame
{
private:
	wxDataViewListCtrl* bookList;
	Database db;
	
	int sortColumn;	// current column to sort
	bool sortAscending;	// sort order (true for ascending)

	void LoadBooks();

	void OnAddBook(wxCommandEvent& event);
	void OnEditBook(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnDeleteBook(wxCommandEvent& event);
	
	void OnKeyDown(wxKeyEvent& event);
	void OnColumnHeaderClick(wxDataViewEvent& event);

public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
};

