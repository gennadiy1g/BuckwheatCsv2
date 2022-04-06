#include "FindColumnDialog.h"

//(*InternalHeaders(FindColumnDialog)
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

FindColumnDialog::FindColumnDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(FindColumnDialog)
	wxBoxSizer* BoxSizerMain;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	SearchCtrl = new wxSearchCtrl(this, ID_SEARCHCTRL1, wxEmptyString, wxDefaultPosition, wxSize(400,29), 0, wxDefaultValidator, _T("ID_SEARCHCTRL1"));
	SearchCtrl->ShowCancelButton(true);
	BoxSizerMain->Add(SearchCtrl, 0, wxALL|wxEXPAND, 5);
	ListView = new wxListView(this, ID_LISTVIEW1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VIRTUAL, wxDefaultValidator, _T("ID_LISTVIEW1"));
	BoxSizerMain->Add(ListView, 1, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizerMain);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);
	//*)
}

FindColumnDialog::~FindColumnDialog()
{
	//(*Destroy(FindColumnDialog)
	//*)
}

