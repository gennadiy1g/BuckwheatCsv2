// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docview.h>

#include "main.hpp"
#include "csv_document.hpp"
#include "csv_view.hpp"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    if(!wxApp::OnInit())
        return false;

    // Create a document manager
    auto *docManager = new wxDocManager;

    // Create a template relating text documents to their views
    new wxDocTemplate(docManager, "Text", "*.txt;*.text", "", "txt;text", "Text Doc", "Text View",
                      wxCLASSINFO(CsvDocument), wxCLASSINFO(CsvView));

    // Enter the application's main loop
    return true;
}
