#include "SeparatorDialog.hpp"

//(*InternalHeaders(SeparatorDialog)
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
	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	//*)
}

SeparatorDialog::~SeparatorDialog()
{
	//(*Destroy(SeparatorDialog)
	//*)
}

