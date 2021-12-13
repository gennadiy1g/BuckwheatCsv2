// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docview.h>
#include <wx/docmdi.h>

#include "main.hpp"
#include "csv_document.hpp"
#include "csv_view.hpp"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    // Create a document manager
    auto *docManager = new wxDocManager;

    // Create a template relating text documents to their views
    new wxDocTemplate(docManager, "Text", "*.txt;*.text", "", "txt;text", "Text Doc", "Text View",
                      wxCLASSINFO(CsvDocument), wxCLASSINFO(CsvView));

    auto frame = new wxDocMDIParentFrame(docManager, NULL, wxID_ANY,
                                         GetAppDisplayName(),
                                         wxDefaultPosition,
                                         wxSize(500, 400));

    auto *menuFile = new wxMenu;
    menuFile->Append(wxID_OPEN);
    menuFile->Append(wxID_CLOSE);
    menuFile->Append(wxID_EXIT);
    docManager->FileHistoryUseMenu(menuFile);

    auto *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    auto *menubar = new wxMenuBar;
    menubar->Append(menuFile, wxGetStockLabel(wxID_FILE));
    menubar->Append(menuHelp, wxGetStockLabel(wxID_HELP));

    frame->SetMenuBar(menubar);
    frame->Centre();
    frame->Show();

    // Enter the application's main loop
    return true;
}
