// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

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
    auto *docManager = new wxDocManager;

    // Create a template relating text documents to their views
    new wxDocTemplate(docManager, "Delimited text files", "*.csv;*.txt;*.tab", "", "csv;txt;tab", "Delimited Text Doc",
                      "Delimited Text View", wxCLASSINFO(CsvDocument), wxCLASSINFO(CsvView));
    new wxDocTemplate(docManager, "All files", "*.*", "", "", "Delimited Text Doc", "Delimited Text Doc",
                      wxCLASSINFO(CsvDocument), wxCLASSINFO(CsvView));

    // TODO (gennadiy#1#): Save position and size of the main frame at the exit and restore them here.
    auto frame = new wxDocMDIParentFrame(docManager, NULL, wxID_ANY, GetAppDisplayName(), wxDefaultPosition,
                                         wxSize(wxWindow::FromDIP(wxSize(1000, 700), nullptr)));

    auto *menuFile = new wxMenu;
    menuFile->Append(wxID_OPEN);
    menuFile->Append(wxID_CLOSE);
    menuFile->Append(wxID_EXIT);
    docManager->FileHistoryUseMenu(menuFile);
    docManager->FileHistoryLoad(*wxConfig::Get());

    auto *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    auto *menubar = new wxMenuBar;
    menubar->Append(menuFile, wxGetStockLabel(wxID_FILE));
    menubar->Append(menuHelp, wxGetStockLabel(wxID_HELP));

    frame->SetMenuBar(menubar);
    frame->CreateStatusBar();
    frame->SetStatusText("");
    frame->SetIcon(wxICON(table));
    frame->Centre();
    frame->Show();

    // Enter the application's main loop
    return true;
};
