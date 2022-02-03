#include <boost/filesystem.hpp>

#include "csv_document.hpp"
#include "csv_view.hpp"

CsvGridTable::CsvGridTable(TokenizedFileLines *pTokenizedFileLines)
    : wxGridTableBase(), mpTokenizedFileLines(pTokenizedFileLines){};

int CsvGridTable::GetNumberCols() {
  if (mNumLines) {
    return mpTokenizedFileLines->numColumns();
  } else {
    return 0;
  }
};

wxString CsvGridTable::GetValue(int row, int col) {
  // the 1st line contains columns' names, the 2nd line is the 1st data row
  auto tokenizedLine = mpTokenizedFileLines->getTokenizedLine(row + 1);
  if (col < static_cast<int>(tokenizedLine->size())) {
    return tokenizedLine->at(col);
  } else {
    return L".";
  }
};

wxString CsvGridTable::GetColLabelValue(int col) {
  if (mNumLines) {
    auto tokenizedLine = mpTokenizedFileLines->getTokenizedLine(0);
    if (col < static_cast<int>(tokenizedLine->size())) {
      return tokenizedLine->at(col);
    } else {
      return L".";
    }
  } else {
    return L"";
  }
};

bool CsvGridTable::AppendRows(size_t numRows) {
  assert(numRows);
  if (mNumLines) {
    mNumLines += numRows;
  } else {
    // the 1st line contains columns' names, do not count the 1st line as a data row
    mNumLines += numRows - 1;
  }
  return true;
}

wxIMPLEMENT_DYNAMIC_CLASS(CsvDocument, wxDocument);

bool CsvDocument::DoOpenDocument(const wxString &file) {
  Modify(false);

  using namespace std::placeholders; // for _1, _2, _3...
  mOnProgress = std::bind(&CsvDocument::OnProgress, this, _1, _2);
  try {
    mpTokenizedFileLines.reset(new TokenizedFileLines(boost::filesystem::path(file), mOnProgress));
    assert(mpTokenizedFileLines);
  } catch (std::runtime_error &) {
    return false;
  }

  mpCsvGridTable.reset(new CsvGridTable(mpTokenizedFileLines.get()));
  return true;
};

void CsvDocument::OnProgress(std::size_t numLines, int percent) {
  auto pCsvView = dynamic_cast<CsvView *>(GetFirstView());
  assert(pCsvView);
  wxThreadEvent event;
  event.SetPayload(numLines);
  event.SetInt(percent);
  pCsvView->QueueEvent(event.Clone());
};
