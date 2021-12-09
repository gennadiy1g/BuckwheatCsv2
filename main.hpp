#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// application class
class App : public wxApp
{
public:
    // function called at the application initialization
    virtual bool OnInit() override;

    // event handler for button click
    void OnClick(wxCommandEvent& event)
    {
        GetTopWindow()->Close();
    }
};
