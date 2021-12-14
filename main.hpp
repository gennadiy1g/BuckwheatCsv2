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
};

class MainFrame : public wxDocMDIParentFrame
{
public:
    MainFrame(wxDocManager* manager, wxFrame* parent, wxWindowID id, const wxString& title,
              const wxPoint& pos = wxDefaultPosition, const wxSize&  size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE,
              const wxString& name = wxFrameNameStr);
    virtual ~MainFrame() = default;
    void OnClose(wxCloseEvent& event);
private:
};
