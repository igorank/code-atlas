#include "main_frame.h"

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame(NULL, wxID_ANY, title, pos, size), db("catalog.db")
{
    wxIcon appIcon;
    if (!appIcon.LoadFile("icon.ico", wxBITMAP_TYPE_ICO)) {
        wxMessageBox("Failed to load icon!", "Error", wxOK | wxICON_ERROR);
    }
    SetIcon(appIcon);

	wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_ADD, "&Add Book");
    menuFile->Append(wxID_EDIT, "&Edit Book");
    menuFile->Append(wxID_DELETE, "&Delete Book");
    menuFile->Append(wxID_EXIT, "&Exit");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &MainFrame::OnAddBook, this, wxID_ADD);
    Bind(wxEVT_MENU, &MainFrame::OnEditBook, this, wxID_EDIT);
    Bind(wxEVT_MENU, &MainFrame::OnDeleteBook, this, wxID_DELETE);
    Bind(wxEVT_MENU, [&](wxCommandEvent&) { Close(); }, wxID_EXIT);

    bookList = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(800, 400));
    bookList->AppendTextColumn("Title", wxDATAVIEW_CELL_INERT, wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    bookList->AppendTextColumn("Author", wxDATAVIEW_CELL_INERT, wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    bookList->AppendTextColumn("Language", wxDATAVIEW_CELL_INERT, wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    bookList->AppendTextColumn("Year", wxDATAVIEW_CELL_INERT, wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    bookList->AppendTextColumn("Description", wxDATAVIEW_CELL_INERT, wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);

    LoadBooks();
}

void MainFrame::LoadBooks() {
    bookList->DeleteAllItems();

    for (const auto& book : db.GetBooks()) {
        wxVector<wxVariant> data;
        data.push_back(wxVariant(wxString(book.title)));
        data.push_back(wxVariant(wxString(book.author)));
        data.push_back(wxVariant(wxString(book.language)));
        data.push_back(wxVariant(wxString::Format("%d", book.year)));
        data.push_back(wxVariant(wxString(book.description)));

        bookList->AppendItem(data);
    }
}


void MainFrame::OnAddBook(wxCommandEvent& event) {
    BookDialog dlg(this, "Add Book");
    if (dlg.ShowModal() == wxID_OK) {
        db.AddBook(dlg.GetBook());
        LoadBooks();
    }
}

void MainFrame::OnEditBook(wxCommandEvent& event) {
    int selectedRow = bookList->GetSelectedRow();
    if (selectedRow == wxNOT_FOUND) {
        wxMessageBox("Please select a book to edit.");
        return;
    }

    BookModel book = db.GetBooks()[selectedRow];
    BookDialog dlg(this, "Edit Book");
    dlg.SetBook(book);
    if (dlg.ShowModal() == wxID_OK) {
        db.UpdateBook(dlg.GetBook());
        LoadBooks();
    }
}

void MainFrame::OnDeleteBook(wxCommandEvent& event) {
    int selectedRow = bookList->GetSelectedRow();
    if (selectedRow == wxNOT_FOUND) {
        wxMessageBox("Please select a book to delete.");
        return;
    }

    int bookId = db.GetBooks()[selectedRow].id;
    db.DeleteBook(bookId);
    LoadBooks();
}