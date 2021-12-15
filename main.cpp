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
    auto *pDocManager = new wxDocManager;

    // Create a template relating text documents to their views
    new wxDocTemplate(pDocManager, "Delimited text files", "*.csv;*.txt;*.tab", "", "csv;txt;tab", "Delimited Text Doc",
                      "Delimited Text View", wxCLASSINFO(CsvDocument), wxCLASSINFO(CsvView));
    new wxDocTemplate(pDocManager, "All files", "*.*", "", "", "Delimited Text Doc", "Delimited Text Doc",
                      wxCLASSINFO(CsvDocument), wxCLASSINFO(CsvView));

    auto *menuFile = new wxMenu;
    menuFile->Append(wxID_OPEN);
    menuFile->Append(wxID_CLOSE);
    menuFile->Append(wxID_EXIT);
    pDocManager->FileHistoryUseMenu(menuFile);
    pDocManager->FileHistoryLoad(*wxConfig::Get());

    auto *pMenuHelp = new wxMenu;
    pMenuHelp->Append(wxID_ABOUT);

    auto *pMenuBar = new wxMenuBar;
    pMenuBar->Append(menuFile, wxGetStockLabel(wxID_FILE));
    pMenuBar->Append(pMenuHelp, wxGetStockLabel(wxID_HELP));

    // TODO (gennadiy#1#): Save position and size of the main frame at the exit and restore them here.
    auto mainFrame = new MainFrame(pDocManager, NULL, wxID_ANY, GetAppDisplayName(), wxDefaultPosition,
                                   wxSize(wxWindow::FromDIP(wxSize(1000, 700), nullptr)));
    mainFrame->SetMenuBar(pMenuBar);
    mainFrame->CreateStatusBar();
    mainFrame->SetStatusText("");
    mainFrame->SetIcon(wxICON(table));
    mainFrame->Centre();
    mainFrame->Show();

    // Enter the application's main loop
    return true;
};

MainFrame::MainFrame(wxDocManager* manager, wxFrame* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                     const wxSize&  size, long style, const wxString& name )
    : wxDocMDIParentFrame(manager, parent, id, title, pos, size, style, name)
{
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    event.Skip(); // the default event handler does call Destroy()
}
