#include "main_frame.h"

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame(NULL, wxID_ANY, title, pos, size), db("catalog.db"), sortColumn(0), sortAscending(true)
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
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, "&Exit");
    
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "&About\tCtrl-A", "Show information about this program");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &MainFrame::OnAddBook, this, wxID_ADD);
    Bind(wxEVT_MENU, &MainFrame::OnEditBook, this, wxID_EDIT);
    Bind(wxEVT_MENU, &MainFrame::OnDeleteBook, this, wxID_DELETE);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, [&](wxCommandEvent&) { Close(); }, wxID_EXIT);

    bookList = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(800, 400));
    bookList->AppendTextColumn("ID", wxDATAVIEW_CELL_INERT, 0, wxALIGN_LEFT, wxDATAVIEW_COL_HIDDEN); // Hidden column
    bookList->AppendTextColumn("Title", wxDATAVIEW_CELL_INERT, wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    bookList->AppendTextColumn("Author", wxDATAVIEW_CELL_INERT, wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    bookList->AppendTextColumn("Language", wxDATAVIEW_CELL_INERT, wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    bookList->AppendTextColumn("Year", wxDATAVIEW_CELL_INERT, wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    bookList->AppendTextColumn("Description", wxDATAVIEW_CELL_INERT, wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);

    bookList->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnKeyDown, this);
    bookList->Bind(wxEVT_DATAVIEW_COLUMN_HEADER_CLICK, &MainFrame::OnColumnHeaderClick, this);

    LoadBooks();
}

void MainFrame::LoadBooks() {
    bookList->DeleteAllItems();

    auto books = db.GetBooks();
    std::sort(books.begin(), books.end(), [&](const BookModel& a, const BookModel& b) {
        switch (sortColumn) {
        case 0: return sortAscending ? (a.id < b.id) : (a.id > b.id);
        case 1: return sortAscending ? a.title < b.title : a.title > b.title;
        case 2: return sortAscending ? a.author < b.author : a.author > b.author;
        case 3: return sortAscending ? a.language < b.language : a.language > b.language;
        case 4: return sortAscending ? a.year < b.year : a.year > b.year;
        case 5: {
            std::string descA = a.description.substr(0, 100);
            std::string descB = b.description.substr(0, 100);
            return sortAscending ? descA < descB : descA > descB;
        }
        default: return true;
        }
        });


    for (const auto& book : books) {
        wxVector<wxVariant> data;
        data.push_back(wxVariant(wxString::Format("%d", book.id))); // ID in hidden column
        data.push_back(wxVariant(wxString(book.title)));
        data.push_back(wxVariant(wxString(book.author)));
        data.push_back(wxVariant(wxString(book.language)));
        data.push_back(wxVariant(wxString::Format("%d", book.year)));
        data.push_back(wxVariant(wxString(truncateText(book.description, 100))));

        bookList->AppendItem(data);
    }
}

void MainFrame::OnColumnHeaderClick(wxDataViewEvent& event) {
    int column = event.GetColumn();
    if (column == sortColumn) {
        sortAscending = !sortAscending; // if we click on the same column, we change the sort order
    }
    else {
        // otherwise, select a new column and set the sorting in ascending order
        sortColumn = column;
        sortAscending = true;
    }
    LoadBooks();
}

void MainFrame::OnAddBook(wxCommandEvent& event) {
    BookDialog dlg(this, "Add Book");
    if (dlg.ShowModal() == wxID_OK) {
        db.AddBook(dlg.GetBook());
        LoadBooks();
    }
}

void MainFrame::OnEditBook(wxCommandEvent& event) {
    wxDataViewItem selected = bookList->GetSelection();
    if (!selected.IsOk()) {
        wxMessageBox("Please select a book to edit.");
        return;
    }

    wxVariant idVariant;
    bookList->GetValue(idVariant, bookList->ItemToRow(selected), 0);
    int bookId = wxAtoi(idVariant.GetString());

    auto books = db.GetBooks();
    auto it = std::find_if(books.begin(), books.end(), [bookId](const BookModel& book) {
        return book.id == bookId;
        });

    if (it == books.end()) {
        wxMessageBox("Book not found.");
        return;
    }

    BookModel book = *it;
    BookDialog dlg(this, "Edit Book");
    dlg.SetBook(book);
    if (dlg.ShowModal() == wxID_OK) {
        db.UpdateBook(dlg.GetBook());
        LoadBooks();
    }
}


void MainFrame::OnAbout(wxCommandEvent& event) {
    AboutDialog dlg(this);
    dlg.ShowModal();
}

void MainFrame::OnDeleteBook(wxCommandEvent& event) {
    wxDataViewItem selected = bookList->GetSelection();
    if (!selected.IsOk()) {
        wxMessageBox("Please select a book to delete.");
        return;
    }

    wxVariant idVariant;
    bookList->GetValue(idVariant, bookList->ItemToRow(selected), 0);
    int bookId = wxAtoi(idVariant.GetString());

    if (wxMessageBox("Are you sure you want to delete this book?", "Confirm Deletion", wxYES_NO | wxICON_QUESTION) == wxYES) {
        db.DeleteBook(bookId);
        LoadBooks();
    }
}


void MainFrame::OnKeyDown(wxKeyEvent& event) {
    if (event.GetKeyCode() == WXK_DELETE) {
        wxDataViewItem selected = bookList->GetSelection();
        if (!selected.IsOk()) {
            wxMessageBox("Please select a book to delete.");
            return;
        }

        wxVariant idVariant;
        bookList->GetValue(idVariant, bookList->ItemToRow(selected), 0); // get ID from hidden column
        int bookId = wxAtoi(idVariant.GetString());

        if (wxMessageBox("Are you sure you want to delete this book?", "Confirm", wxYES_NO | wxICON_WARNING) == wxYES) {
            db.DeleteBook(bookId);
            LoadBooks();
        }
    }
    else {
        event.Skip();
    }
}