#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docview.h>
#include <wx/grid.h>

#include <memory>

#include "csv_document.hpp"

class CsvView : public wxView {
public:
  CsvView();
  virtual void OnDraw(wxDC *dc) override{};
  virtual bool OnCreate(wxDocument *doc, long flags) override;
  virtual bool OnClose(bool deleteWindow) override;
  virtual void OnActivateView(bool activate, wxView *activeView, wxView *deactiveView) override;
  void OnThreadEvent(const wxThreadEvent &event);
  void showStatus();
  void clearStatus();

private:
  wxGrid *mpGrid{};
  std::unique_ptr<CsvGridTable> mpCsvGridTable;
  bool mIsActive{false};
  static inline std::unique_ptr<std::locale> mpsThousandsSep{};

  wxDECLARE_DYNAMIC_CLASS(CsvView);
};

struct thousand_sep_numpunct : std::numpunct<char> {
  char do_thousands_sep() const { return ','; }    // separate with comma
  std::string do_grouping() const { return "\3"; } // groups of 3 digit
};
