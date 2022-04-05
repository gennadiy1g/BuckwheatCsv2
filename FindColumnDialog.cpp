#include "FindColumnDialog.h"

//(*InternalHeaders(FindColumnDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(FindColumnDialog)
//*)

BEGIN_EVENT_TABLE(FindColumnDialog,wxDialog)
	//(*EventTable(FindColumnDialog)
	//*)
END_EVENT_TABLE()

FindColumnDialog::FindColumnDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(FindColumnDialog)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
}

FindColumnDialog::~FindColumnDialog()
{
	//(*Destroy(FindColumnDialog)
	//*)
}

