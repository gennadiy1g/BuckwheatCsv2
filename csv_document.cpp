#include <boost/filesystem.hpp>

#include "csv_document.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(CsvDocument, wxDocument);

bool CsvDocument::DoOpenDocument(const wxString &filename) {
  Modify(false);

  using namespace std::placeholders; // for _1, _2, _3...
  mOnProgress = std::bind(&CsvDocument::OnProgress, this, _1, _2);
  try {
    mpTokenizedFileLines.reset(new TokenizedFileLines(boost::filesystem::path(filename), mOnProgress));
  } catch (std::runtime_error &) {
    return false;
  }

  return true;
}

void CsvDocument::OnProgress(std::size_t numLines, int percent) {
  return; //
};
