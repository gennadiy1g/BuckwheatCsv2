#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/aui/tabmdi.h>
#include <wx/gauge.h>

#include "csv_view.hpp"

enum EventID {
  ID_SEPARATOR_DIALOG = wxID_HIGHEST + 1,
  ID_DEFAULT_COL_SIZE,
  ID_AUTOSIZE_COL_LABEL_SIZE,
  ID_GOTO_ROW_DIALOG,
  ID_GOTO_COL_DIALOG
};

class StatusBar;

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

static_assert(wxUSE_AUI);

class MainFrame : public wxDocParentFrameAny<wxAuiMDIParentFrame> {
public:
  MainFrame(wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString &title,
            const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE, const wxString &name = wxFrameNameStr);
  virtual ~MainFrame() = default;
  void OnClose(wxCloseEvent &event);

  static StatusBar *statusBar();
  static wxMenuBar *menuBar();
  static wxToolBar *toolBar();

  // Disable the View submenu if no documents are open, otherwise enable the View submenu
  static void toggleViewMenu(bool onViewCreate);
  static CsvView *currentView();

private:
  void onSeparatorDialog(wxCommandEvent &event);
  void onGoToRowDialog(wxCommandEvent &event);
  void onGoToColumnDialog(wxCommandEvent &event);
  void onDefaultColSize(wxCommandEvent &event);
  void onAutoSizeColLabelSize(wxCommandEvent &event);

  class MFGeometrySerializer : public wxTopLevelWindow::GeometrySerializer {
  private:
    static inline const wxString path{"/Geometry"};
    virtual bool RestoreField(const wxString &name, int *value) override;
    virtual bool SaveField(const wxString &name, int value) const override;
  };
};

class StatusBar : public wxStatusBar {
public:
  StatusBar(wxWindow *parent);
  virtual ~StatusBar() = default;
};
