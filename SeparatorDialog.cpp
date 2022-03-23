#include "SeparatorDialog.hpp"

//(*InternalHeaders(SeparatorDialog)
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/valtext.h>

//(*IdInit(SeparatorDialog)
const long SeparatorDialog::ID_RADIOBUTTON1 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON2 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON3 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON4 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON5 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON6 = wxNewId();
const long SeparatorDialog::ID_TEXTCTRL1 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON7 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON8 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON10 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON11 = wxNewId();
const long SeparatorDialog::ID_RADIOBUTTON9 = wxNewId();
const long SeparatorDialog::ID_TEXTCTRL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(SeparatorDialog,wxDialog)
	//(*EventTable(SeparatorDialog)
	//*)
END_EVENT_TABLE()

SeparatorDialog::SeparatorDialog(wxWindow* parent)
{
	//(*Initialize(SeparatorDialog)
	wxBoxSizer* BoxSizerMain;
	wxBoxSizer* mBoxSizerQuoteEscape;
	wxStaticBoxSizer* StaticBoxSizerEscape;
	wxStaticBoxSizer* StaticBoxSizerSeparator;
	wxStaticBoxSizer* mStaticBoxSizerQuote;
	wxStdDialogButtonSizer* StdDialogButtonSizer;

	Create(parent, wxID_ANY, _("Separator, Quote, Escape"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizerSeparator = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Separator character"));
	RadioButtonComma = new wxRadioButton(this, ID_RADIOBUTTON1, _("Comma"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	StaticBoxSizerSeparator->Add(RadioButtonComma, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	RadioButtonTab = new wxRadioButton(this, ID_RADIOBUTTON2, _("Tab"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	StaticBoxSizerSeparator->Add(RadioButtonTab, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	RadioButtonSemicolon = new wxRadioButton(this, ID_RADIOBUTTON3, _("Semicolon"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	StaticBoxSizerSeparator->Add(RadioButtonSemicolon, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	RadioButtonVerticalBar = new wxRadioButton(this, ID_RADIOBUTTON4, _("Vertical bar"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
	StaticBoxSizerSeparator->Add(RadioButtonVerticalBar, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	RadioButtonSpace = new wxRadioButton(this, ID_RADIOBUTTON5, _("Space"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON5"));
	StaticBoxSizerSeparator->Add(RadioButtonSpace, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	RadioButtonOtherSeparator = new wxRadioButton(this, ID_RADIOBUTTON6, _("Other"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON6"));
	StaticBoxSizerSeparator->Add(RadioButtonOtherSeparator, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrlOtherSeparator = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY), _T("ID_TEXTCTRL1"));
	TextCtrlOtherSeparator->SetMaxLength(1);
	TextCtrlOtherSeparator->Disable();
	StaticBoxSizerSeparator->Add(TextCtrlOtherSeparator, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizerMain->Add(StaticBoxSizerSeparator, 0, wxALL|wxEXPAND, 5);
	mBoxSizerQuoteEscape = new wxBoxSizer(wxHORIZONTAL);
	mStaticBoxSizerQuote = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Quote character"));
	RadioButtonDouble = new wxRadioButton(this, ID_RADIOBUTTON7, _("Double"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON7"));
	mStaticBoxSizerQuote->Add(RadioButtonDouble, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	RadioButtonSingle = new wxRadioButton(this, ID_RADIOBUTTON8, _("Single"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON8"));
	mStaticBoxSizerQuote->Add(RadioButtonSingle, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mBoxSizerQuoteEscape->Add(mStaticBoxSizerQuote, 0, wxTOP|wxBOTTOM|wxRIGHT|wxEXPAND, 5);
	mBoxSizerQuoteEscape->Add(-1,-1,1, wxALL|wxEXPAND, 0);
	StaticBoxSizerEscape = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Escape character"));
	RadioButtonNoEscape = new wxRadioButton(this, ID_RADIOBUTTON10, _("No Escape character"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON10"));
	StaticBoxSizerEscape->Add(RadioButtonNoEscape, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	RadioButtonBackslash = new wxRadioButton(this, ID_RADIOBUTTON11, _("Backslash"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON11"));
	StaticBoxSizerEscape->Add(RadioButtonBackslash, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	RadioButtonOtherEscape = new wxRadioButton(this, ID_RADIOBUTTON9, _("Other"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON9"));
	StaticBoxSizerEscape->Add(RadioButtonOtherEscape, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	TextCtrlOtherEscape = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY), _T("ID_TEXTCTRL2"));
	TextCtrlOtherEscape->SetMaxLength(1);
	TextCtrlOtherEscape->Disable();
	StaticBoxSizerEscape->Add(TextCtrlOtherEscape, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mBoxSizerQuoteEscape->Add(StaticBoxSizerEscape, 0, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5);
	BoxSizerMain->Add(mBoxSizerQuoteEscape, 0, wxALL|wxEXPAND, 5);
	StdDialogButtonSizer = new wxStdDialogButtonSizer();
	StdDialogButtonSizer->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer->Realize();
	BoxSizerMain->Add(StdDialogButtonSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	//*)
}

SeparatorDialog::~SeparatorDialog()
{
	//(*Destroy(SeparatorDialog)
	//*)
}

