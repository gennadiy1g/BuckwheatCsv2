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
  CsvView() : wxView(){};
  virtual void OnDraw(wxDC *dc) override{};
  virtual bool OnCreate(wxDocument *doc, long flags) override;
  virtual bool OnClose(bool deleteWindow) override;
  virtual void OnActivateView(bool activate, wxView *activeView, wxView *deactiveView) override;
  void OnThreadEvent(const wxThreadEvent &event);

private:
  wxGrid *mpGrid{nullptr};
  std::unique_ptr<CsvGridTable> mpCsvGridTable;
  wxDECLARE_DYNAMIC_CLASS(CsvView);
};
