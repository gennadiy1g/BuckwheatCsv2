#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docview.h>
#include <wx/grid.h>

#include <boost/filesystem.hpp>

#include <memory>

#include "CsvTable/CsvTable.hpp"

class CsvTable : public wxGridTableBase {
public:
  CsvTable(TokenizedFileLines *pTokenizedFileLines);
  virtual ~CsvTable() = default;

  CsvTable(const CsvTable &src) = delete;
  CsvTable &operator=(const CsvTable &rhs) = delete;

  CsvTable(CsvTable &&src) = delete;
  CsvTable &operator=(CsvTable &&rhs) = delete;

  virtual int GetNumberRows() override;
  virtual int GetNumberCols() override;
  virtual wxString GetValue(int row, int col) override;
  virtual void SetValue(int row, int col, const wxString &value) override;
};

class CsvDocument : public wxDocument {
public:
  CsvDocument() : wxDocument(){};

protected:
  virtual bool DoOpenDocument(const wxString &file) override;

private:
  std::unique_ptr<TokenizedFileLines> mpTokenizedFileLines;
  std::unique_ptr<CsvTable> mpCsvTable;
  void OnProgress(std::size_t numLines, int percent);
  FileLines::OnProgress mOnProgress;

  wxDECLARE_DYNAMIC_CLASS(CsvDocument);
};
