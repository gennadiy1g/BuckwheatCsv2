// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "main.hpp"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    if(!wxApp::OnInit())
        return false;

    // create a new frame and set it as the top most application window
    SetTopWindow(new wxFrame(NULL, -1, wxT(""), wxDefaultPosition, wxSize(100, 50)));

    // create new button and assign it to the main frame
    new wxButton(GetTopWindow(), wxID_EXIT, wxT("Click!"));

    // connect button click event with event handler
    Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(App::OnClick));

    // show main frame
    GetTopWindow()->Show();

    // enter the application's main loop
    return true;
}
