#ifndef FINDCOLUMNDIALOG_H
#define FINDCOLUMNDIALOG_H

//(*Headers(FindColumnDialog)
#include <wx/dialog.h>
//*)

class FindColumnDialog: public wxDialog
{
	public:

		FindColumnDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);
		virtual ~FindColumnDialog();

		//(*Declarations(FindColumnDialog)
		//*)

	protected:

		//(*Identifiers(FindColumnDialog)
		//*)

	private:

		//(*Handlers(FindColumnDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
