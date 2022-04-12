#include <boost/locale/localization_backend.hpp>
#include <numeric>

#include <wx/app.h>
#include <wx/config.h>
#include <wx/debug.h>
#include <wx/docview.h>
#include <wx/numdlg.h>

#include "CsvTable/log.hpp"
#include "CsvTable/utilities.hpp"
#include "FindColumnDialog.h"
#include "SeparatorDialog.hpp"
#include "csv_document.hpp"
#include "csv_view.hpp"
#include "main.hpp"

#ifdef __WXGTK__
#include "table.xpm"
#endif

namespace blocale = boost::locale;

using namespace std::literals::string_literals;

wxIMPLEMENT_APP(App);

bool App::OnInit() {
  if (!wxApp::OnInit()) {
    return false;
  }

  initLocalization();
  initLogging();

  auto backends = blocale::localization_backend_manager::global().get_all_backends();
  std::string backendsList =
      std::accumulate(backends.cbegin(), backends.cend(), ""s,
                      [](const std::string &a, const std::string &b) { return a + (a == "" ? "" : ", ") + b; });
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::debug) << " Localization backends: " << backendsList << '.';

  SetVendorName("gennadiy1g");
  SetAppName("buckwheatcsv");
  SetAppDisplayName("Buckwheat CSV");

  // Create a document manager
  auto pDocManager = new wxDocManager;

  // Create a template relating text documents to their views
  new wxDocTemplate(pDocManager, "Delimited text files", "*.csv;*.txt;*.tab", "", "csv;txt;tab", "Delimited Text Doc",
                    "Delimited Text View", wxCLASSINFO(CsvDocument), wxCLASSINFO(CsvView));
  new wxDocTemplate(pDocManager, "All files", "*.*", "", "", "Delimited Text Doc", "Delimited Text View",
                    wxCLASSINFO(CsvDocument), wxCLASSINFO(CsvView));

  auto pMainFrame = new MainFrame(pDocManager, NULL, wxID_ANY, GetAppDisplayName());
  pMainFrame->Show();

  // Enter the application's main loop
  return true;
};

int App::OnExit() {
  const auto pDocManager = wxDocManager::GetDocumentManager();
  pDocManager->FileHistorySave(*wxConfig::Get());
  delete pDocManager;

  return wxApp::OnExit();
}

MainFrame::MainFrame(wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString &title, const wxPoint &pos,
                     const wxSize &size, long style, const wxString &name)
    : wxDocMDIParentFrame(manager, parent, id, title, pos, size, style, name) {
  auto pMenuFile = new wxMenu;
  pMenuFile->Append(wxID_OPEN, wxGetStockLabel(wxID_OPEN));
  pMenuFile->Append(wxID_CLOSE, wxGetStockLabel(wxID_CLOSE));
  pMenuFile->Append(wxID_EXIT, wxGetStockLabel(wxID_EXIT));

  const auto pDocManager = wxDocManager::GetDocumentManager();
  pDocManager->FileHistoryUseMenu(pMenuFile);
  pDocManager->FileHistoryLoad(*wxConfig::Get());

  auto pMenuView = new wxMenu;
  pMenuView->Append(ID_SEPARATOR_DIALOG, "&Separator, Quote, Escape...\tALT+S");
  pMenuView->AppendSeparator();
  pMenuView->Append(ID_DEFAULT_COL_SIZE, "Set &default widths of columns\tALT+D");
  pMenuView->Append(ID_AUTOSIZE_COL_LABEL_SIZE, "Adjust widths of columns to fit &labels\tALT+L",
                    "Automatically adjust widths of the columns to fit their labels. ");

  auto pMenuGoTo = new wxMenu;
  pMenuGoTo->Append(ID_GOTO_ROW_DIALOG, "Go to &Row...\tALT+R");
  pMenuGoTo->Append(ID_GOTO_COL_DIALOG, "Go to &Column...\tALT+C");

  auto pMenuHelp = new wxMenu;
  pMenuHelp->Append(wxID_ABOUT, wxGetStockLabel(wxID_ABOUT));

  auto pMenuBar = new wxMenuBar;
  pMenuBar->Append(pMenuFile, wxGetStockLabel(wxID_FILE));
  pMenuBar->Append(pMenuView, "&View");
  pMenuBar->Append(pMenuGoTo, "&Go to");
  pMenuBar->Append(pMenuHelp, wxGetStockLabel(wxID_HELP));

  SetMenuBar(pMenuBar);
  pMenuBar->EnableTop(1, false); // disable View submenu
  pMenuBar->EnableTop(2, false); // disable GoTo submenu
  pMenuBar->Enable(ID_SEPARATOR_DIALOG, false);
  pMenuBar->Enable(ID_DEFAULT_COL_SIZE, false);
  pMenuBar->Enable(ID_AUTOSIZE_COL_LABEL_SIZE, false);
  pMenuBar->Enable(ID_GOTO_ROW_DIALOG, false);
  pMenuBar->Enable(ID_GOTO_COL_DIALOG, false);
  SetIcon(wxICON(table));

  Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
  Bind(wxEVT_MENU, &MainFrame::onSeparatorDialog, this, ID_SEPARATOR_DIALOG);
  Bind(wxEVT_MENU, &MainFrame::onGoToRowDialog, this, ID_GOTO_ROW_DIALOG);
  Bind(wxEVT_MENU, &MainFrame::onGoToColumnDialog, this, ID_GOTO_COL_DIALOG);
  Bind(wxEVT_MENU, &MainFrame::onDefaultColSize, this, ID_DEFAULT_COL_SIZE);
  Bind(wxEVT_MENU, &MainFrame::onAutoSizeColLabelSize, this, ID_AUTOSIZE_COL_LABEL_SIZE);
  MFGeometrySerializer appGeometrySerializer;
  RestoreToGeometry(appGeometrySerializer);

  SetStatusBar(new StatusBar(this));
};

void MainFrame::OnClose(wxCloseEvent &event) {
  MFGeometrySerializer appGeometrySerializer;
  SaveGeometry(appGeometrySerializer);
  event.Skip(); // the default event handler does call Destroy()
};

bool MainFrame::MFGeometrySerializer::RestoreField(const wxString &name, int *value) {
  const auto pConfig = wxConfigBase::Get();
  const auto oldPath = pConfig->GetPath();
  pConfig->SetPath(path);
  const auto result = pConfig->Read(name, value);
  pConfig->SetPath(oldPath);
  return result;
};

bool MainFrame::MFGeometrySerializer::SaveField(const wxString &name, int value) const {
  /* By some strange reason, if x and y are 0, the main frame is centered on the
   screen instead of being restored. By changing x from 0 to 1, we achieve that
   the main frame's geometry is almost restored (i.e., moved 1 pixel to the
   right). */
  if (name == "x") {
    int x{-1}, y{-1};
    wxTheApp->GetTopWindow()->GetPosition(&x, &y);
    wxASSERT(value == x);
    if (!value && !y) {
      value = 1;
    }
  };

  const auto pConfig = wxConfigBase::Get();
  const auto oldPath = pConfig->GetPath();
  pConfig->SetPath(path);
  const auto result = pConfig->Write(name, value);
  pConfig->SetPath(oldPath);
  return result;
};

void MainFrame::onSeparatorDialog(wxCommandEvent &event) {
  auto pDocument = wxDocManager::GetDocumentManager()->GetCurrentDocument();
  wxASSERT(pDocument);
  auto pCsvDocument = dynamic_cast<CsvDocument *>(pDocument);
  wxASSERT(pCsvDocument);
  SeparatorDialog separatorDialog{wxTheApp->GetTopWindow()};
  separatorDialog.separator(pCsvDocument->separator());
  separatorDialog.quote(pCsvDocument->quote());
  separatorDialog.escape(pCsvDocument->escape());
  if (separatorDialog.ShowModal() == wxID_OK) {
    pCsvDocument->escapeSeparatorQuote(separatorDialog.escape(), separatorDialog.separator(), separatorDialog.quote());
  }
};

CsvView *MainFrame::currentView() {
  auto pView = wxDocManager::GetDocumentManager()->GetCurrentView();
  wxASSERT(pView);
  auto pCsvView = dynamic_cast<CsvView *>(pView);
  wxASSERT(pCsvView);
  return pCsvView;
};

void MainFrame::onGoToRowDialog(wxCommandEvent &event) {
  auto row = wxGetNumberFromUser("", "Row number:", "Go to Row", 1, 1, currentView()->gridTable()->GetNumberRows(),
                                 wxTheApp->GetTopWindow());
  currentView()->selectRow(row);
};

void MainFrame::onGoToColumnDialog(wxCommandEvent &event) {
  auto pCsvView = currentView();
  FindColumnDialog findColumnDialog{wxTheApp->GetTopWindow(), pCsvView->gridTable()};
  if (findColumnDialog.ShowModal() == wxID_OK) {
    auto selectedCol = findColumnDialog.selectedCol();
    if (selectedCol >= 0) { // selectedCol contains first selected column, if any, -1 otherwise
      pCsvView->selectCol(selectedCol);
    }
  }
};

void MainFrame::onDefaultColSize(wxCommandEvent &event) {
  auto pCsvView = currentView();
  pCsvView->defaultColSize();
};

void MainFrame::onAutoSizeColLabelSize(wxCommandEvent &event) {
  auto pCsvView = currentView();
  pCsvView->autoSizeColLabelSize();
};

StatusBar *MainFrame::statusBar() {
  auto pTopFrame = dynamic_cast<wxFrame *>(wxTheApp->GetTopWindow());
  wxASSERT(pTopFrame);
  auto pStatusBar = dynamic_cast<StatusBar *>(pTopFrame->GetStatusBar());
  wxASSERT(pStatusBar);
  return pStatusBar;
};

wxMenuBar *MainFrame::menuBar() {
  auto pTopFrame = dynamic_cast<wxFrame *>(wxTheApp->GetTopWindow());
  wxASSERT(pTopFrame);
  auto pMenuBar = pTopFrame->GetMenuBar();
  wxASSERT(pMenuBar);
  return pMenuBar;
};

void MainFrame::toggleViewMenu(bool onViewCreate) {
  auto enable = onViewCreate || wxDocManager::GetDocumentManager()->GetDocumentsVector().size() > 1;
  MainFrame::menuBar()->EnableTop(1, enable); // View submenu
  MainFrame::menuBar()->EnableTop(2, enable); // GoTo submenu
  MainFrame::menuBar()->Enable(ID_SEPARATOR_DIALOG, enable);
  MainFrame::menuBar()->Enable(ID_DEFAULT_COL_SIZE, enable);
  MainFrame::menuBar()->Enable(ID_AUTOSIZE_COL_LABEL_SIZE, enable);
  MainFrame::menuBar()->Enable(ID_GOTO_ROW_DIALOG, enable);
  MainFrame::menuBar()->Enable(ID_GOTO_COL_DIALOG, enable);
};

StatusBar::StatusBar(wxWindow *parent) : wxStatusBar(parent) {
  constexpr auto fieldsCount{5};
  int widths[fieldsCount]{-1, -1, -1, -1, -1}; // rows, columns, separator, quote, escape
  SetFieldsCount(fieldsCount, widths);
};
