#ifndef FINDCOLUMNDIALOG_H
#define FINDCOLUMNDIALOG_H

//(*Headers(FindColumnDialog)
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/srchctrl.h>
//*)

class wxColumnsListView;

class FindColumnDialog: public wxDialog
{
	public:

		FindColumnDialog(wxWindow* parent);
		virtual ~FindColumnDialog();

		//(*Declarations(FindColumnDialog)
		wxColumnsListView* ListView;
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

class wxColumnsListView : public wxListView {
public:
  wxColumnsListView(wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize, long style = wxLC_REPORT,
                    const wxValidator &validator = wxDefaultValidator, const wxString &name = wxListCtrlNameStr)
      : wxListView(parent, winid, pos, size, style, validator, name){};
  virtual ~wxColumnsListView(){};
};

#endif
