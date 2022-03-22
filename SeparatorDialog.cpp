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
	wxBoxSizer* mBoxSizerMain;
	wxBoxSizer* mBoxSizerQuoteEscape;
	wxStaticBoxSizer* mStaticBoxSizerEscape;
	wxStaticBoxSizer* mStaticBoxSizerQuote;
	wxStaticBoxSizer* mStaticBoxSizerSeparator;
	wxStdDialogButtonSizer* mStdDialogButtonSizer;

	Create(parent, wxID_ANY, _("Separator, Quote, Escape"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	mBoxSizerMain = new wxBoxSizer(wxVERTICAL);
	mStaticBoxSizerSeparator = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Separator character"));
	mRadioButtonComma = new wxRadioButton(this, ID_RADIOBUTTON1, _("Comma"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
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
	mTextCtrlOtherSeparator = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY), _T("ID_TEXTCTRL1"));
	mTextCtrlOtherSeparator->SetMaxLength(1);
	mTextCtrlOtherSeparator->Disable();
	mStaticBoxSizerSeparator->Add(mTextCtrlOtherSeparator, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mBoxSizerMain->Add(mStaticBoxSizerSeparator, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mBoxSizerQuoteEscape = new wxBoxSizer(wxHORIZONTAL);
	mStaticBoxSizerQuote = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Quote character"));
	mRadioButtonDouble = new wxRadioButton(this, ID_RADIOBUTTON7, _("Double"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON7"));
	mStaticBoxSizerQuote->Add(mRadioButtonDouble, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mRadioButtonSingle = new wxRadioButton(this, ID_RADIOBUTTON8, _("Single"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON8"));
	mStaticBoxSizerQuote->Add(mRadioButtonSingle, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mBoxSizerQuoteEscape->Add(mStaticBoxSizerQuote, 1, wxTOP|wxBOTTOM|wxRIGHT|wxEXPAND, 5);
	mStaticBoxSizerEscape = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Escape character"));
	mRadioButtonNoEscape = new wxRadioButton(this, ID_RADIOBUTTON10, _("No Escape character"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTON10"));
	mStaticBoxSizerEscape->Add(mRadioButtonNoEscape, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mRadioButtonOtherEscape = new wxRadioButton(this, ID_RADIOBUTTON9, _("Other"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON9"));
	mStaticBoxSizerEscape->Add(mRadioButtonOtherEscape, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mTextCtrlOtherEscape = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY), _T("ID_TEXTCTRL2"));
	mTextCtrlOtherEscape->SetMaxLength(1);
	mTextCtrlOtherEscape->Disable();
	mStaticBoxSizerEscape->Add(mTextCtrlOtherEscape, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mBoxSizerQuoteEscape->Add(mStaticBoxSizerEscape, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	mBoxSizerMain->Add(mBoxSizerQuoteEscape, 0, wxALL|wxEXPAND, 5);
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

