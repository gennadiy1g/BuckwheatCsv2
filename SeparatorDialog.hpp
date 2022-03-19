#ifndef SEPARATORDIALOG_H
#define SEPARATORDIALOG_H

//(*Headers(SeparatorDialog)
#include <wx/dialog.h>
//*)

class SeparatorDialog: public wxDialog
{
	public:

		SeparatorDialog(wxWindow* parent);
		virtual ~SeparatorDialog();

		//(*Declarations(SeparatorDialog)
		//*)

	protected:

		//(*Identifiers(SeparatorDialog)
		//*)

	private:

		//(*Handlers(SeparatorDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
