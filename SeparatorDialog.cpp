#include "SeparatorDialog.hpp"

//(*InternalHeaders(SeparatorDialog)
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(SeparatorDialog)
//*)

BEGIN_EVENT_TABLE(SeparatorDialog,wxDialog)
	//(*EventTable(SeparatorDialog)
	//*)
END_EVENT_TABLE()

SeparatorDialog::SeparatorDialog(wxWindow* parent)
{
	//(*Initialize(SeparatorDialog)
	wxBoxSizer* mBoxSizerMain;
	wxBoxSizer* mBoxSizerQuoteEscape;
	wxStaticBoxSizer* mStaticBoxSizerEscape;
	wxStaticBoxSizer* mStaticBoxSizerQuote;
	wxStaticBoxSizer* mStaticBoxSizerSeparator;
	wxStdDialogButtonSizer* mStdDialogButtonSizer;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	mBoxSizerMain = new wxBoxSizer(wxVERTICAL);
	mStaticBoxSizerSeparator = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Separator character"));
	mBoxSizerMain->Add(mStaticBoxSizerSeparator, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mBoxSizerQuoteEscape = new wxBoxSizer(wxHORIZONTAL);
	mStaticBoxSizerQuote = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Quote character"));
	mBoxSizerQuoteEscape->Add(mStaticBoxSizerQuote, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mStaticBoxSizerEscape = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Escape character"));
	mBoxSizerQuoteEscape->Add(mStaticBoxSizerEscape, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mBoxSizerMain->Add(mBoxSizerQuoteEscape, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mStdDialogButtonSizer = new wxStdDialogButtonSizer();
	mStdDialogButtonSizer->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	mStdDialogButtonSizer->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	mStdDialogButtonSizer->Realize();
	mBoxSizerMain->Add(mStdDialogButtonSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(mBoxSizerMain);
	mBoxSizerMain->Fit(this);
	mBoxSizerMain->SetSizeHints(this);
	//*)
}

SeparatorDialog::~SeparatorDialog()
{
	//(*Destroy(SeparatorDialog)
	//*)
}

