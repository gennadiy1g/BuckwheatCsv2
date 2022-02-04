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

class CsvDocument : public wxDocument {
public:
  TokenizedFileLines *getTokenizedFileLines() {
    assert(mpTokenizedFileLines);
    return mpTokenizedFileLines.get();
  };

protected:
  virtual bool DoOpenDocument(const wxString &file) override;

private:
  std::unique_ptr<TokenizedFileLines> mpTokenizedFileLines;
  void OnProgress(std::size_t numLines, int percent);
  FileLines::OnProgress mOnProgress;

  wxDECLARE_DYNAMIC_CLASS(CsvDocument);
};

class CsvGridTable : public wxGridTableBase {
public:
  CsvGridTable(CsvDocument *pCsvDocument) : wxGridTableBase(), mpCsvDocument(pCsvDocument){};
  virtual ~CsvGridTable() = default;

  CsvGridTable(const CsvGridTable &src) = delete;
  CsvGridTable &operator=(const CsvGridTable &rhs) = delete;

  CsvGridTable(CsvGridTable &&src) = delete;
  CsvGridTable &operator=(CsvGridTable &&rhs) = delete;

  virtual int GetNumberRows() override { return mNumLines; };
  virtual int GetNumberCols() override;
  virtual wxString GetValue(int row, int col) override;
  virtual void SetValue(int row, int col, const wxString &value) override{};
  virtual wxString GetColLabelValue(int col) override;
  virtual bool AppendRows(size_t numRows = 1) override;

  wxString getValueAux(int row, int col);

private:
  CsvDocument *mpCsvDocument{nullptr};
  std::size_t mNumLines{0};
  bool mFirstLineCounted{false};
};
