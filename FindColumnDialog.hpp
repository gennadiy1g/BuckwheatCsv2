#ifndef FINDCOLUMNDIALOG_H
#define FINDCOLUMNDIALOG_H

//(*Headers(FindColumnDialog)
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/srchctrl.h>
#include <wx/timer.h>
//*)

#include <vector>

#include <wx/grid.h>
#include <wx/string.h>

class ColumnsListView;

class FindColumnDialog: public wxDialog
{
	public:

		FindColumnDialog(wxWindow* parent, wxGridTableBase* pGridTable);
		virtual ~FindColumnDialog();

		//(*Declarations(FindColumnDialog)
		ColumnsListView* ListView;
		wxSearchCtrl* SearchCtrl;
		wxTimer Timer;
		//*)

		int selectedCol() const;

    protected:

		//(*Identifiers(FindColumnDialog)
		static const long ID_SEARCHCTRL1;
		static const long ID_LISTVIEW1;
		static const long ID_TIMER1;
		//*)

	private:

		//(*Handlers(FindColumnDialog)
		void OnSearchCtrlSearchClicked(wxCommandEvent& event);
		void OnSearchCtrlCancelClicked(wxCommandEvent& event);
		void OnTimerTrigger(wxTimerEvent& event);
		void OnSearchCtrlText(wxCommandEvent& event);
		void OnListViewItemActivated(wxListEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

class ColumnsListView : public wxListView {
public:
  ColumnsListView(wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize, long style = wxLC_REPORT,
                    const wxValidator &validator = wxDefaultValidator, const wxString &name = wxListCtrlNameStr)
      : wxListView(parent, winid, pos, size, style, validator, name){};
  virtual ~ColumnsListView(){};
  void gridTable(wxGridTableBase *pGridTable) { mpGridTable = pGridTable; };
  // The same return type as parameter type in void wxListCtrl::SetItemCount(long count)
  long countItems(const wxString &str);
  long GetFirstSelected() const;

protected:
  virtual wxString OnGetItemText(long item, long column) const override;
  std::vector<int> mColumnNumbers;

private:
  wxGridTableBase *mpGridTable{};
};

#endif
