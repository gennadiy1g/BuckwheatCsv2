#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docview.h>
#include <wx/grid.h>
#include <wx/infobar.h>

#include "csv_document.hpp"

class CsvView : public wxView {
public:
  CsvView();
  virtual void OnDraw(wxDC *dc) override{};
  virtual bool OnCreate(wxDocument *doc, long flags) override;
  virtual bool OnClose(bool deleteWindow) override;
  virtual void OnActivateView(bool activate, wxView *activeView, wxView *deactiveView) override;
  virtual void OnUpdate(wxView *sender, wxObject *hint = 0) override;
  void OnThreadEvent(const wxThreadEvent &event);
  void showStatus();
  void clearStatus() const;
  void defaultColSize() const;
  void autoSizeColLabelSize() const;
  CsvGridTable *gridTable() const;
  void goToCol(int col) const;
  void goToRow(int row) const;

private:
  wxInfoBar *mInfoBar{};
  bool mInfoBarShown{false};
  wxGrid *mpGrid{};
  wxGauge *mpGauge{};
  CsvGridTable *mpCsvGridTable{};
  bool mIsActive{false};
  static inline std::unique_ptr<std::locale> mpsThousandsSep{};
  CsvDocument *mpCsvDocument{};

  wxDECLARE_DYNAMIC_CLASS(CsvView);
};

struct thousand_sep_numpunct : std::numpunct<char> {
  virtual char do_thousands_sep() const override { return ','; }    // separate with comma
  virtual std::string do_grouping() const override { return "\3"; } // groups of 3 digit
};
