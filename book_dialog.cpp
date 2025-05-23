#include "book_dialog.h"

BookDialog::BookDialog(wxWindow* parent, const wxString& title)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(400, 600)) {

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Title"), 0, wxALL, 5);
    titleCtrl = new wxTextCtrl(this, wxID_ANY);
    mainSizer->Add(titleCtrl, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Author"), 0, wxALL, 5);
    authorCtrl = new wxTextCtrl(this, wxID_ANY);
    mainSizer->Add(authorCtrl, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Programming Language"), 0, wxALL, 5);
    languageCtrl = new wxChoice(this, wxID_ANY);
    languageCtrl->Append("C++");
    languageCtrl->Append("Python");
    languageCtrl->Append("JavaScript");
    languageCtrl->Append("Go");
    languageCtrl->Append("Java");
    languageCtrl->Append("Ruby");
    languageCtrl->Append("Swift");
    languageCtrl->Append("PHP");
    languageCtrl->Append("C#");
    languageCtrl->Append("Kotlin");
    languageCtrl->Append("Rust");
    mainSizer->Add(languageCtrl, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Publisher"), 0, wxALL, 5);
    publisherCtrl = new wxTextCtrl(this, wxID_ANY);
    mainSizer->Add(publisherCtrl, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "ISBN"), 0, wxALL, 5);
    isbnCtrl = new wxTextCtrl(this, wxID_ANY);
    mainSizer->Add(isbnCtrl, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Year"), 0, wxALL, 5);
    yearCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1900, 2025);
    mainSizer->Add(yearCtrl, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Description"), 0, wxALL, 5);
    descriptionCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    mainSizer->Add(descriptionCtrl, 1, wxEXPAND | wxALL, 5);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSizer->Add(new wxButton(this, wxID_OK, "Save"), 0, wxALL, 5);
    btnSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);

    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER);

    SetSizer(mainSizer);

    Bind(wxEVT_BUTTON, &BookDialog::OnSave, this, wxID_OK);
}

void BookDialog::OnSave(wxCommandEvent& event) {
    if (titleCtrl->GetValue().IsEmpty()) {
        wxMessageBox("Please enter a title.", "Validation Error", wxOK | wxICON_ERROR);
        return;
    }
    if (authorCtrl->GetValue().IsEmpty()) {
        wxMessageBox("Please enter an author.", "Validation Error", wxOK | wxICON_ERROR);
        return;
    }
    if (languageCtrl->GetSelection() == wxNOT_FOUND) {
        wxMessageBox("Please select a programming language.", "Validation Error", wxOK | wxICON_ERROR);
        return;
    }
    if (isbnCtrl->GetValue().IsEmpty()) {
        wxMessageBox("Please enter an ISBN.", "Validation Error", wxOK | wxICON_ERROR);
        return;
    }

    wxString sanitizedDescription = descriptionCtrl->GetValue();
    sanitizedDescription.Replace("\n", "");
    descriptionCtrl->SetValue(sanitizedDescription);

    EndModal(wxID_OK);
}

BookModel BookDialog::GetBook() const {
    return {
        currentBookId,
        titleCtrl->GetValue().ToStdString(),
        authorCtrl->GetValue().ToStdString(),
        languageCtrl->GetStringSelection().ToStdString(),
        publisherCtrl->GetValue().ToStdString(),
        isbnCtrl->GetValue().ToStdString(),
        yearCtrl->GetValue(),
        descriptionCtrl->GetValue().ToStdString()
    };
}

void BookDialog::SetBook(const BookModel& book) {
    titleCtrl->SetValue(book.title);
    authorCtrl->SetValue(book.author);
    languageCtrl->SetStringSelection(book.language);
    publisherCtrl->SetValue(book.publisher);
    isbnCtrl->SetValue(book.isbn);
    yearCtrl->SetValue(book.year);
    descriptionCtrl->SetValue(book.description);
    currentBookId = book.id;
}