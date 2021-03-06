#include <boost/locale/localization_backend.hpp>
#include <numeric>
#include <sstream>

#include <wx/aboutdlg.h>
#include <wx/app.h>
#include <wx/bmpbndl.h>
#include <wx/cmdline.h>
#include <wx/config.h>
#include <wx/debug.h>
#include <wx/docview.h>
#include <wx/filefn.h>
#include <wx/gdicmn.h>
#include <wx/imagpng.h>
#include <wx/msgdlg.h>
#include <wx/numdlg.h>

#include "CsvTable/log.hpp"
#include "CsvTable/utilities.hpp"
#include "FindColumnDialog.hpp"
#include "SeparatorDialog.hpp"
#include "csv_document.hpp"
#include "csv_view.hpp"
#include "main.hpp"

#ifdef __WXGTK__
#include "bitmaps/table_32x32.xpm"

#include "bitmaps/column_wight.png.h" // yes, this is spelling mistake; the original file's name is column_wight.png
#include "bitmaps/folder_table.png.h"
#include "bitmaps/table_gear.png.h"
#include "bitmaps/table_select_column.png.h"
#include "bitmaps/table_select_row.png.h"
#include "bitmaps/text_columns.png.h"

#include "bitmaps/column_wight_2x.png.h" // yes, this is spelling mistake; the original file's name is column_wight.png
#include "bitmaps/folder_table_2x.png.h"
#include "bitmaps/table_gear_2x.png.h"
#include "bitmaps/table_select_column_2x.png.h"
#include "bitmaps/table_select_row_2x.png.h"
#include "bitmaps/text_columns_2x.png.h"
#endif

namespace blocale = boost::locale;

using namespace std::literals::string_literals;

wxIMPLEMENT_APP(App);

bool App::OnInit() {
  if (!wxApp::OnInit()) {
    return false;
  }

  // By some reason, cursor changes to "busy/waiting" after clicking on the URL in the about dialog
  wxSetCursor(*wxSTANDARD_CURSOR); // restore the standard cursor

  initLocalization();
  initLogging();

  // Register PNG image handler to be able to load bitmaps from PNG data
  wxImage::AddHandler(new wxPNGHandler);

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

  for (size_t i = 0; i < mFilesFromCmdLine.size(); ++i) {
    if (wxFileExists(mFilesFromCmdLine[i])) {
      pDocManager->CreateDocument(mFilesFromCmdLine[i], wxDOC_SILENT);
    } else {
      wxMessageBox("The file \'" + mFilesFromCmdLine[i] + "\' does not exist!", "Attention", wxOK, pMainFrame);
    }
  }

  // Enter the application's main loop
  return true;
};

int App::OnExit() {
  const auto pDocManager = wxDocManager::GetDocumentManager();
  pDocManager->FileHistorySave(*wxConfig::Get());
  delete pDocManager;

  // By some reason, cursor changes to "busy/waiting" after clicking on the URL in the about dialog
  wxSetCursor(*wxSTANDARD_CURSOR); // restore the standard cursor

  return wxApp::OnExit();
}

void App::OnInitCmdLine(wxCmdLineParser &parser) {
  parser.AddParam("document-file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_MULTIPLE | wxCMD_LINE_PARAM_OPTIONAL);
  wxApp::OnInitCmdLine(parser);
};

bool App::OnCmdLineParsed(wxCmdLineParser &parser) {
  // save any files given on the command line: we'll open them in OnInit()
  // later, after creating the frame
  for (size_t i = 0; i < parser.GetParamCount(); ++i)
    mFilesFromCmdLine.push_back(parser.GetParam(i));

  return wxApp::OnCmdLineParsed(parser);
};

MainFrame::MainFrame(wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString &title)
    : AuiMainFrame(manager, parent, id, title) {
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
  pMenuView->Append(ID_AUTOSIZE_COL_LABEL_SIZE, "Adjust widths of columns to fit &labels\tALT+L");

  auto pMenuGoTo = new wxMenu;
  pMenuGoTo->Append(ID_GOTO_ROW_DIALOG, "Go to &Row...\tCTRL+G");
  pMenuGoTo->Append(ID_GOTO_COL_DIALOG, "Go to &Column...\tCTRL+SHIFT+G");

  auto pMenuHelp = new wxMenu;
  pMenuHelp->Append(wxID_ABOUT, wxGetStockLabel(wxID_ABOUT));
  pMenuHelp->Append(wxID_DONATE, "Donate!", "buymeacoffee.com");

  auto pMenuBar = new wxMenuBar;
  pMenuBar->Append(pMenuFile, wxGetStockLabel(wxID_FILE));
  pMenuBar->Append(pMenuView, "&View");
  pMenuBar->Append(pMenuGoTo, "&Go to");
  pMenuBar->Append(pMenuHelp, wxGetStockLabel(wxID_HELP));
  SetMenuBar(pMenuBar);

  SetStatusBar(new StatusBar(this));

  SetIcon(wxICON(table_32x32));

  auto toolBar = CreateToolBar();
  toolBar->AddTool(wxID_OPEN, wxGetStockLabel(wxID_OPEN), wxBITMAP_BUNDLE_2(folder_table), wxNullBitmap, wxITEM_NORMAL,
                   "Open File", "Open File");
  toolBar->AddTool(ID_GOTO_ROW_DIALOG, "Row", wxBITMAP_BUNDLE_2(table_select_row), wxNullBitmap, wxITEM_NORMAL,
                   "Go to Row", "Go to Row");
  toolBar->AddTool(ID_GOTO_COL_DIALOG, "Column", wxBITMAP_BUNDLE_2(table_select_column), wxNullBitmap, wxITEM_NORMAL,
                   "Go to Column", "Go to Column");
  toolBar->AddTool(ID_AUTOSIZE_COL_LABEL_SIZE, "Fit widths", wxBITMAP_BUNDLE_2(column_wight), wxNullBitmap,
                   wxITEM_NORMAL, "Adjust widths of columns to fit labels", "Adjust widths of columns to fit labels");
  toolBar->AddTool(ID_DEFAULT_COL_SIZE, "Default widths", wxBITMAP_BUNDLE_2(text_columns), wxNullBitmap, wxITEM_NORMAL,
                   "Set default widths of columns", "Set default widths of columns");
  toolBar->AddTool(ID_SEPARATOR_DIALOG, "Delimiters", wxBITMAP_BUNDLE_2(table_gear), wxNullBitmap, wxITEM_NORMAL,
                   "Separator, Quote, Escape", "Separator, Quote, Escape");
  toolBar->Realize();

  pMenuBar->EnableTop(1, false); // disable View submenu
  pMenuBar->EnableTop(2, false); // disable GoTo submenu
  pMenuBar->Enable(ID_SEPARATOR_DIALOG, false);
  pMenuBar->Enable(ID_DEFAULT_COL_SIZE, false);
  pMenuBar->Enable(ID_AUTOSIZE_COL_LABEL_SIZE, false);
  pMenuBar->Enable(ID_GOTO_ROW_DIALOG, false);
  pMenuBar->Enable(ID_GOTO_COL_DIALOG, false);

  toolBar->EnableTool(ID_GOTO_ROW_DIALOG, false);
  toolBar->EnableTool(ID_GOTO_COL_DIALOG, false);
  toolBar->EnableTool(ID_AUTOSIZE_COL_LABEL_SIZE, false);
  toolBar->EnableTool(ID_DEFAULT_COL_SIZE, false);
  toolBar->EnableTool(ID_SEPARATOR_DIALOG, false);

  Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
  Bind(wxEVT_MENU, &MainFrame::onSeparatorDialog, this, ID_SEPARATOR_DIALOG);
  Bind(wxEVT_MENU, &MainFrame::onGoToRowDialog, this, ID_GOTO_ROW_DIALOG);
  Bind(wxEVT_MENU, &MainFrame::onGoToColumnDialog, this, ID_GOTO_COL_DIALOG);
  Bind(wxEVT_MENU, &MainFrame::onDefaultColSize, this, ID_DEFAULT_COL_SIZE);
  Bind(wxEVT_MENU, &MainFrame::onAutoSizeColLabelSize, this, ID_AUTOSIZE_COL_LABEL_SIZE);
  Bind(wxEVT_MENU, &MainFrame::onAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &MainFrame::onDonate, this, wxID_DONATE);

  SetDropTarget(new DnDFile());

  MFGeometrySerializer appGeometrySerializer;
  RestoreToGeometry(appGeometrySerializer);
};

void MainFrame::OnClose(wxCloseEvent &event) {
  MFGeometrySerializer appGeometrySerializer;
  SaveGeometry(appGeometrySerializer);
  event.Skip(); // the default event handler does call Destroy()
};

bool MainFrame::MFGeometrySerializer::RestoreField(const wxString &name, int *value) {
  const auto pConfig = wxConfigBase::Get();
  const auto oldPath = pConfig->GetPath();
  pConfig->SetPath(ksmPath);
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
    if (!x && !y) {
      value = 1;
    }
  };

  const auto pConfig = wxConfigBase::Get();
  const auto oldPath = pConfig->GetPath();
  pConfig->SetPath(ksmPath);
  const auto result = pConfig->Write(name, value);
  pConfig->SetPath(oldPath);
  return result;
};

void MainFrame::onSeparatorDialog(wxCommandEvent &event) {
  auto pDocument = wxDocManager::GetDocumentManager()->GetCurrentDocument();
  wxASSERT(pDocument);
  auto pCsvDocument = dynamic_cast<CsvDocument *>(pDocument);
  wxASSERT(pCsvDocument);
  SeparatorDialog separatorDialog{wxTheApp->GetTopWindow(), pCsvDocument->documentFile()};
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
  auto numberRows = currentView()->gridTable()->GetNumberRows();
  if (numberRows > 0) {
    auto row = wxGetNumberFromUser("", "Row number:", "Go to Row", 1, 1, numberRows, wxTheApp->GetTopWindow());
    if (row != -1) { // If the user cancels the dialog, the function returns -1
      currentView()->goToRow(row);
    }
  }
};

void MainFrame::onGoToColumnDialog(wxCommandEvent &event) {
  auto pCsvView = currentView();
  FindColumnDialog findColumnDialog{wxTheApp->GetTopWindow(), pCsvView->gridTable()};
  if (findColumnDialog.ShowModal() == wxID_OK) {
    auto selectedCol = findColumnDialog.selectedCol();
    if (selectedCol != -1) { // selectedCol contains first selected column, if any, -1 otherwise
      pCsvView->goToCol(selectedCol);
    }
  }
};

void MainFrame::onDefaultColSize(wxCommandEvent &event) { currentView()->defaultColSize(); };

void MainFrame::onAutoSizeColLabelSize(wxCommandEvent &event) { currentView()->autoSizeColLabelSize(); };

void MainFrame::onAbout(wxCommandEvent &event) {
  wxAboutDialogInfo aboutInfo;
  aboutInfo.SetIcon(wxICON(table_32x32));
  aboutInfo.SetName(wxTheApp->GetAppDisplayName());
  aboutInfo.SetVersion("2.0.2");
  aboutInfo.SetDescription("Viewer for CSV files");
  aboutInfo.SetCopyright("(C) 2019-2022 gennadiy1g");
  aboutInfo.SetWebSite("https://github.com/gennadiy1g/BuckwheatCsv2/releases");

  wxAboutBox(aboutInfo, this);
};

void MainFrame::onDonate(wxCommandEvent &event) {
  int arr[] = {-103, -114, -113, -108, -110, -52, -40, -39, -110, -109, -108, -34, -85,
               -103, -106, -93,  -84,  -79,  -80, -91, -81, -80,  -78,  -77,  -21, -73,
               -84,  -81,  -18,  -73,  -70,  -78, -77, -63, -65,  -69,  -84,  -11, -64};
  std::stringstream ss;
  for (std::size_t i = 0; i < std::size(arr); ++i) {
    ss << static_cast<char>(i + 1 - arr[i]);
  }
  wxLaunchDefaultBrowser(ss.str(), wxBROWSER_NOBUSYCURSOR);
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

wxToolBar *MainFrame::toolBar() {
  auto pTopFrame = dynamic_cast<wxFrame *>(wxTheApp->GetTopWindow());
  wxASSERT(pTopFrame);
  auto pToolBar = pTopFrame->GetToolBar();
  wxASSERT(pToolBar);
  return pToolBar;
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

  MainFrame::toolBar()->EnableTool(ID_GOTO_ROW_DIALOG, enable);
  MainFrame::toolBar()->EnableTool(ID_GOTO_COL_DIALOG, enable);
  MainFrame::toolBar()->EnableTool(ID_AUTOSIZE_COL_LABEL_SIZE, enable);
  MainFrame::toolBar()->EnableTool(ID_DEFAULT_COL_SIZE, enable);
  MainFrame::toolBar()->EnableTool(ID_SEPARATOR_DIALOG, enable);
};

StatusBar::StatusBar(wxWindow *parent) : wxStatusBar(parent) {
  constexpr auto fieldsCount{5};
  int widths[fieldsCount]{-1, -1, -1, -1, -1}; // rows, columns, separator, quote, escape
  SetFieldsCount(fieldsCount, widths);
};

bool DnDFile::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &fileNames) {
  for (size_t i = 0; i < fileNames.GetCount(); ++i) {
    wxDocManager::GetDocumentManager()->CreateDocument(fileNames[i], wxDOC_SILENT);
  }
  return true;
};
