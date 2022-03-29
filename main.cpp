#include <boost/locale/localization_backend.hpp>
#include <numeric>

#include <wx/config.h>
#include <wx/debug.h>
#include <wx/docview.h>

#include "CsvTable/log.hpp"
#include "CsvTable/utilities.hpp"
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

StatusBar *App::statusBar() {
  auto pTopFrame = dynamic_cast<wxFrame *>(wxTheApp->GetTopWindow());
  wxASSERT(pTopFrame);
  auto pStatusBar = dynamic_cast<StatusBar *>(pTopFrame->GetStatusBar());
  wxASSERT(pStatusBar);
  return pStatusBar;
};

wxMenuBar *App::menuBar() {
  auto pTopFrame = dynamic_cast<wxFrame *>(wxTheApp->GetTopWindow());
  wxASSERT(pTopFrame);
  auto pMenuBar = pTopFrame->GetMenuBar();
  wxASSERT(pMenuBar);
  return pMenuBar;
};

void App::toggleViewMenu() {
  App::menuBar()->EnableTop(1, wxDocManager::GetDocumentManager()->GetDocumentsVector().size());
};

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
  pMenuView->Append(ID_SEPARATOR_DIALOG, "Separator/Quote/Escape character...");

  auto pMenuHelp = new wxMenu;
  pMenuHelp->Append(wxID_ABOUT, wxGetStockLabel(wxID_ABOUT));

  auto pMenuBar = new wxMenuBar;
  pMenuBar->Append(pMenuFile, wxGetStockLabel(wxID_FILE));
  pMenuBar->Append(pMenuView, "View");
  pMenuBar->Append(pMenuHelp, wxGetStockLabel(wxID_HELP));

  SetMenuBar(pMenuBar);
  pMenuBar->EnableTop(1, false); // disable View submenu
  SetIcon(wxICON(table));

  Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
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

StatusBar::StatusBar(wxWindow *parent) : wxStatusBar(parent) {
  constexpr auto fieldsCount{5};
  int widths[fieldsCount]{-1, -1, -1, -1, -1}; // rows, columns, separator, quote, escape
  SetFieldsCount(fieldsCount, widths);
};
