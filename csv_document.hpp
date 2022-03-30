#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/debug.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docview.h>
#include <wx/grid.h>

#include <boost/filesystem.hpp>

#include <memory>
#include <optional>

#include "CsvTable/CsvTable.hpp"
#include "CsvTable/SepChars.hpp"

class CsvDocument : public wxDocument {
public:
  TokenizedFileLines *tokenizedFileLines() {
    wxASSERT(mpTokenizedFileLines);
    return mpTokenizedFileLines.get();
  };

  virtual bool DeleteContents() override;
  virtual bool OnCreate(const wxString &path, long flags) override;

  wchar_t separator() const {
    wxASSERT(mSeparator);
    return mSeparator.value();
  };
  wchar_t quote() const {
    wxASSERT(mQuote);
    return mQuote.value();
  };
  wchar_t escape() const {
    return mEscape;
  };

protected:
  virtual bool DoOpenDocument(const wxString &file) override;

private:
  std::unique_ptr<TokenizedFileLines> mpTokenizedFileLines;
  void OnProgress(std::size_t numLines, int percent);
  FileLines::OnProgress mOnProgress;
  std::optional<wchar_t> mSeparator;
  std::optional<wchar_t> mQuote;
  wchar_t mEscape{kNull};

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

  virtual int GetNumberRows() override;
  virtual int GetNumberCols() override;
  virtual wxString GetValue(int row, int col) override;
  virtual void SetValue(int row, int col, const wxString &value) override{};
  virtual wxString GetColLabelValue(int col) override;

  wxString getValueAux(int row, int col) const;
  int getPercent() const { return mPercent; };
  void setNumberRows(std::size_t numRows, int percent);
  bool hasData() const { return mNumLines; };

private:
  CsvDocument *mpCsvDocument{};
  std::size_t mNumLines{};
  int mPercent{};
};
