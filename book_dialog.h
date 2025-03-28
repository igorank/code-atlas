#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "book_model.h"

class BookDialog : public wxDialog
{
public:
	BookDialog(wxWindow* parent, const wxString& title);

	BookModel GetBook() const;
	void SetBook(const BookModel& book);

private:
    wxTextCtrl* titleCtrl;
    wxTextCtrl* authorCtrl;
    wxChoice* languageCtrl;
    wxTextCtrl* publisherCtrl;
    wxTextCtrl* isbnCtrl;
    wxSpinCtrl* yearCtrl;
    wxTextCtrl* descriptionCtrl;

    void OnSave(wxCommandEvent& event);

    int currentBookId = -1;
};