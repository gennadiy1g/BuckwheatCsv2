#include <wx/docview.h>
#include <wx/docmdi.h>
#include <wx/config.h>

#include "main.hpp"
#include "csv_document.hpp"
#include "csv_view.hpp"
#ifdef __WXGTK__
#include "table.xpm"
#endif

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    SetVendorName("gennadiy1g");
    SetAppName("buckwheatcsv");
    SetAppDisplayName("Buckwheat CSV");

    // Create a document manager
    auto pDocManager = new wxDocManager;

    // Create a template relating text documents to their views
    new wxDocTemplate(pDocManager, "Delimited text files", "*.csv;*.txt;*.tab", "", "csv;txt;tab", "Delimited Text Doc",
                      "Delimited Text View", wxCLASSINFO(CsvDocument), wxCLASSINFO(CsvView));
    new wxDocTemplate(pDocManager, "All files", "*.*", "", "", "Delimited Text Doc", "Delimited Text Doc",
                      wxCLASSINFO(CsvDocument), wxCLASSINFO(CsvView));

    auto menuFile = new wxMenu;
    menuFile->Append(wxID_OPEN);
    menuFile->Append(wxID_CLOSE);
    menuFile->Append(wxID_EXIT);
    pDocManager->FileHistoryUseMenu(menuFile);
    pDocManager->FileHistoryLoad(*wxConfig::Get());

    auto pMenuHelp = new wxMenu;
    pMenuHelp->Append(wxID_ABOUT);

    auto pMenuBar = new wxMenuBar;
    pMenuBar->Append(menuFile, wxGetStockLabel(wxID_FILE));
    pMenuBar->Append(pMenuHelp, wxGetStockLabel(wxID_HELP));

    auto pMainFrame = new MainFrame(pDocManager, NULL, wxID_ANY, GetAppDisplayName());
    pMainFrame->SetMenuBar(pMenuBar);
    pMainFrame->CreateStatusBar();
    pMainFrame->SetStatusText("");
    pMainFrame->SetIcon(wxICON(table));
    pMainFrame->Show();

    // Enter the application's main loop
    return true;
};

MainFrame::MainFrame(wxDocManager* manager, wxFrame* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                     const wxSize&  size, long style, const wxString& name )
    : wxDocMDIParentFrame(manager, parent, id, title, pos, size, style, name)
{
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
    AppGeometrySerializer appGeometrySerializer;
    RestoreToGeometry(appGeometrySerializer);
};

void MainFrame::OnClose(wxCloseEvent& event)
{
    AppGeometrySerializer appGeometrySerializer;
    SaveGeometry(appGeometrySerializer);
    event.Skip(); // the default event handler does call Destroy()
};

bool AppGeometrySerializer::RestoreField(const wxString &name, int *value)
{
    auto pConfig = wxConfigBase::Get();
    auto oldPath = pConfig->GetPath();
    pConfig->SetPath(path);
    auto result = pConfig->Read(name, value);
    pConfig->SetPath(oldPath);

    /* By some strange reason, if x and y are 0, the main frame is centered on the screen.
     By changing x from 0 to 1, we achieve that the main frame's geometry is almost restored
     (i.e., moved 1 pixel to the right).  */
    if (result && (name == "x") && (*value == 0)) {
        *value = 1;
    }
    return result;
};
bool AppGeometrySerializer::SaveField(const wxString &name, int value) const
{
    auto pConfig = wxConfigBase::Get();
    auto oldPath = pConfig->GetPath();
    pConfig->SetPath(path);
    auto result = pConfig->Write(name, value);
    pConfig->SetPath(oldPath);
    return result;
};
