#include "about_dialog.h"

AboutDialog::AboutDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "About CodeAtlas", wxDefaultPosition, wxSize(400, 300)) {

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* appName = new wxStaticText(this, wxID_ANY, "CodeAtlas 1.0");
    appName->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(appName, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    wxStaticText* description = new wxStaticText(this, wxID_ANY,
        "This program is a book catalog for modern programming languages.\n"
        "It helps organize and manage information about books.");
    mainSizer->Add(description, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 15);

    mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 10);

    wxStaticText* developerInfo = new wxStaticText(this, wxID_ANY,
        "Developer: Ihor Anikin\nEmail: anikin.ihor@lll.kpi.ua");
    mainSizer->Add(developerInfo, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->AddStretchSpacer(1);
    wxButton* okButton = new wxButton(this, wxID_OK, "OK");
    buttonSizer->Add(okButton, 0, wxALL, 10);

    mainSizer->Add(buttonSizer, 0, wxEXPAND);

    SetSizerAndFit(mainSizer);

    Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { EndModal(wxID_OK); }, wxID_OK);
}