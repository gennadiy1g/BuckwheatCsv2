#ifndef FINDCOLUMNDIALOG_H
#define FINDCOLUMNDIALOG_H

//(*Headers(FindColumnDialog)
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/srchctrl.h>
//*)

class FindColumnDialog: public wxDialog
{
	public:

		FindColumnDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);
		virtual ~FindColumnDialog();

		//(*Declarations(FindColumnDialog)
		wxListView* ListView;
		wxSearchCtrl* SearchCtrl;
		//*)

	protected:

		//(*Identifiers(FindColumnDialog)
		static const long ID_SEARCHCTRL1;
		static const long ID_LISTVIEW1;
		//*)

	private:

		//(*Handlers(FindColumnDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
