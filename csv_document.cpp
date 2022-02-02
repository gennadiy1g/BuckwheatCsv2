#include <boost/filesystem.hpp>

#include "csv_document.hpp"
#include "csv_view.hpp"

CsvGridTable::CsvGridTable(TokenizedFileLines *pTokenizedFileLines)
    : wxGridTableBase(), mpTokenizedFileLines(pTokenizedFileLines){};

int CsvGridTable::GetNumberCols() { return 0; };
wxString CsvGridTable::GetValue(int row, int col) { return L""; };
wxString CsvGridTable::GetColLabelValue(int col) { return L""; };

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
