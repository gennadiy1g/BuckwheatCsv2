#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docview.h>

#include <memory>

#include "CsvTable/CsvTable.hpp"

class CsvDocument : public wxDocument {
public:
  CsvDocument() : wxDocument(){};

protected:
  virtual bool DoOpenDocument(const wxString &filename) override;

private:
  std::unique_ptr<TokenizedFileLines> mpTokenizedFileLines;
  void OnProgress(std::size_t numLines, int percent);
  FileLines::OnProgress mOnProgress;

  wxDECLARE_DYNAMIC_CLASS(CsvDocument);
};
