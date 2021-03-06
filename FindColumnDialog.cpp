#include <string>

#include <boost/algorithm/string/predicate.hpp>

#include "FindColumnDialog.hpp"

//(*InternalHeaders(FindColumnDialog)
#include <wx/button.h>
#include <wx/debug.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(FindColumnDialog)
const long FindColumnDialog::ID_SEARCHCTRL1 = wxNewId();
const long FindColumnDialog::ID_LISTVIEW1 = wxNewId();
const long FindColumnDialog::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(FindColumnDialog,wxDialog)
	//(*EventTable(FindColumnDialog)
	//*)
END_EVENT_TABLE()

FindColumnDialog::FindColumnDialog(wxWindow* parent, wxGridTableBase* pGridTable)
{
	//(*Initialize(FindColumnDialog)
	wxBoxSizer* BoxSizerMain;
	wxStdDialogButtonSizer* StdDialogButtonSizer;

	Create(parent, wxID_ANY, _("Go to Column"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	BoxSizerMain = new wxBoxSizer(wxVERTICAL);
	SearchCtrl = new wxSearchCtrl(this, ID_SEARCHCTRL1, wxEmptyString, wxDefaultPosition, wxSize(400,29), wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_SEARCHCTRL1"));
	SearchCtrl->ShowCancelButton(true);
	BoxSizerMain->Add(SearchCtrl, 0, wxALL|wxEXPAND, 5);
	ListView = new ColumnsListView(this, ID_LISTVIEW1, wxDefaultPosition, wxSize(400,400), wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VIRTUAL, wxDefaultValidator, _T("ID_LISTVIEW1"));
	BoxSizerMain->Add(ListView, 1, wxALL|wxEXPAND, 5);
	StdDialogButtonSizer = new wxStdDialogButtonSizer();
	StdDialogButtonSizer->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer->Realize();
	BoxSizerMain->Add(StdDialogButtonSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizerMain);
	Timer.SetOwner(this, ID_TIMER1);
	BoxSizerMain->Fit(this);
	BoxSizerMain->SetSizeHints(this);

	Connect(ID_SEARCHCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&FindColumnDialog::OnSearchCtrlText);
	Connect(ID_SEARCHCTRL1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&FindColumnDialog::OnSearchCtrlSearchClicked);
	Connect(ID_SEARCHCTRL1,wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN,(wxObjectEventFunction)&FindColumnDialog::OnSearchCtrlSearchClicked);
	Connect(ID_SEARCHCTRL1,wxEVT_COMMAND_SEARCHCTRL_CANCEL_BTN,(wxObjectEventFunction)&FindColumnDialog::OnSearchCtrlCancelClicked);
	Connect(ID_LISTVIEW1,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&FindColumnDialog::OnListViewItemActivated);
	Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&FindColumnDialog::OnTimerTrigger);
	//*)

  ListView->gridTable(pGridTable);
  ListView->InsertColumn(ListView->GetColumnCount(), "#", wxLIST_FORMAT_RIGHT);
  ListView->InsertColumn(ListView->GetColumnCount(), "Name", wxLIST_FORMAT_LEFT, 300);
  updateListAndButtonOK(wxEmptyString);
}

FindColumnDialog::~FindColumnDialog()
{
	//(*Destroy(FindColumnDialog)
	//*)
}

long ColumnsListView::countItems(const wxString &str) {
  if (str.IsEmpty()) {
    return mpGridTable->GetNumberCols();
  } else {
    // Find all columns with names that contain str
    std::wstring stdStr = str.ToStdWstring();
    mColumnNumbers.clear();
    for (int i = 0; i < mpGridTable->GetNumberCols(); ++i) {
      if (boost::icontains(mpGridTable->GetColLabelValue(i).ToStdWstring(), stdStr)) {
        mColumnNumbers.push_back(i);
      }
    }
    return mColumnNumbers.size();
  }
};

wxString ColumnsListView::OnGetItemText(long item, long column) const {
  if (GetItemCount() == mpGridTable->GetNumberCols()) {
    if (column) {
      return mpGridTable->GetColLabelValue(item);
    } else {
      return std::to_wstring(item + 1);
    }
  } else {
    wxASSERT(static_cast<decltype(mColumnNumbers)::size_type>(item) < mColumnNumbers.size());
    auto columnNumber = mColumnNumbers.at(item);
    if (column) {
      return mpGridTable->GetColLabelValue(columnNumber);
    } else {
      return std::to_wstring(columnNumber + 1);
    }
  }
};

long ColumnsListView::GetFirstSelected() const {
  auto firstSelected = wxListView::GetFirstSelected();
  if (firstSelected == -1) {
    return firstSelected;
  } else {
    if (GetItemCount() == mpGridTable->GetNumberCols()) {
      return firstSelected;
    } else {
      return mColumnNumbers.at(firstSelected);
    }
  }
};

long FindColumnDialog::selectedCol() const { return ListView->GetFirstSelected(); };

void FindColumnDialog::OnSearchCtrlSearchClicked(wxCommandEvent &event) {
  updateListAndButtonOK(SearchCtrl->GetValue());
}

void FindColumnDialog::OnSearchCtrlCancelClicked(wxCommandEvent &event) { updateListAndButtonOK(wxEmptyString); }

void FindColumnDialog::OnTimerTrigger(wxTimerEvent &event) { updateListAndButtonOK(SearchCtrl->GetValue()); }

void FindColumnDialog::updateListAndButtonOK(const wxString &str) {
  auto itemCount = ListView->countItems(str);
  ListView->SetItemCount(itemCount);
  ListView->Refresh();
  if (itemCount > 0) {
    ListView->Select(0);
  }

  auto buttonOK = dynamic_cast<wxButton *>(FindWindowById(wxID_OK));
  wxASSERT(buttonOK);
  buttonOK->Enable(itemCount);
}

void FindColumnDialog::OnSearchCtrlText(wxCommandEvent &event) {
  Timer.Stop();
  Timer.StartOnce(500);
}

void FindColumnDialog::OnListViewItemActivated(wxListEvent &event) { EndModal(wxID_OK); }
