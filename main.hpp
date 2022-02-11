#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docmdi.h>

// Application class
class App : public wxApp {
public:
  // function called at the application initialization
  virtual bool OnInit() override;

  // processing which needs to be done as the application is about to exit
  virtual int OnExit() override;
};

// Create a forward declaration of the wxGetApp() function implemented by
// wxIMPLEMENT_APP()
wxDECLARE_APP(App);

class MainFrame : public wxDocMDIParentFrame {
public:
  MainFrame(wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString &title,
            const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE, const wxString &name = wxFrameNameStr);
  virtual ~MainFrame() = default;
  void OnClose(wxCloseEvent &event);

private:
  class MFGeometrySerializer : public wxTopLevelWindow::GeometrySerializer {
  private:
    static inline const wxString path{"/Geometry"};
    virtual bool RestoreField(const wxString &name, int *value) override;
    virtual bool SaveField(const wxString &name, int value) const override;
  };
};
