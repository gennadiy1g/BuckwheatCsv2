#include "FindColumnDialog.h"

//(*InternalHeaders(FindColumnDialog)
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(FindColumnDialog)
const long FindColumnDialog::ID_SEARCHCTRL1 = wxNewId();
const long FindColumnDialog::ID_LISTVIEW1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(FindColumnDialog,wxDialog)
	//(*EventTable(FindColumnDialog)
	//*)
END_EVENT_TABLE()

FindColumnDialog::FindColumnDialog(wxWindow* parent, wxGridTableBase* gridTable)
{
	//(*Initialize(FindColumnDialog)
	wxBoxSizer* BoxSizerMain;
	wxStdDialogButtonSizer* StdDialogButtonSizer;

	Create(parent, wxID_ANY, _("Find Column"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	SearchCtrl = new wxSearchCtrl(this, ID_SEARCHCTRL1, wxEmptyString, wxDefaultPosition, wxSize(400,29), 0, wxDefaultValidator, _T("ID_SEARCHCTRL1"));
	SearchCtrl->ShowCancelButton(true);
	BoxSizerMain->Add(SearchCtrl, 0, wxALL|wxEXPAND, 5);
	ListView = new wxColumnsListView(this, ID_LISTVIEW1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VIRTUAL, wxDefaultValidator, _T("ID_LISTVIEW1"));
	BoxSizerMain->Add(ListView, 1, wxALL|wxEXPAND, 5);
	StdDialogButtonSizer = new wxStdDialogButtonSizer();
	StdDialogButtonSizer->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer->Realize();
	BoxSizerMain->Add(StdDialogButtonSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	//*)

	mpGridTable = gridTable;
}

FindColumnDialog::~FindColumnDialog()
{
	//(*Destroy(FindColumnDialog)
	//*)
}

