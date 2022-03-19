#include "SeparatorDialog.hpp"

//(*InternalHeaders(SeparatorDialog)
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(SeparatorDialog)
const long SeparatorDialog::ID_RADIOBUTTON1 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON2 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON3 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON4 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON5 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON6 = wxNewId();
const long SeparatorDialog::ID_TEXTCTRL1 = wxNewId();
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
	mRadioButtonComma = new wxRadioButton(this, ID_RADIOBUTTON1, _("Comma"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	mStaticBoxSizerSeparator->Add(mRadioButtonComma, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mRadioButtonTab = new wxRadioButton(this, ID_RADIOBUTTON2, _("Tab"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	mStaticBoxSizerSeparator->Add(mRadioButtonTab, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mRadioButtonSemicolon = new wxRadioButton(this, ID_RADIOBUTTON3, _("Semicolon"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	mStaticBoxSizerSeparator->Add(mRadioButtonSemicolon, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mRadioButtonVerticalBar = new wxRadioButton(this, ID_RADIOBUTTON4, _("Vertical bar"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
	mStaticBoxSizerSeparator->Add(mRadioButtonVerticalBar, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mRadioButtonSpace = new wxRadioButton(this, ID_RADIOBUTTON5, _("Space"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON5"));
	mStaticBoxSizerSeparator->Add(mRadioButtonSpace, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mRadioButtonOther = new wxRadioButton(this, ID_RADIOBUTTON6, _("Other"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON6"));
	mStaticBoxSizerSeparator->Add(mRadioButtonOther, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mTextCtrlOtherSeparator = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	mTextCtrlOtherSeparator->SetMaxLength(1);
	mTextCtrlOtherSeparator->Disable();
	mStaticBoxSizerSeparator->Add(mTextCtrlOtherSeparator, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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

