#include <boost/filesystem.hpp>

#include "csv_document.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(CsvDocument, wxDocument);

bool CsvDocument::DoOpenDocument(const wxString &filename) {
  Modify(false);

  try {
    mpTokenizedFileLines.reset(new TokenizedFileLines(boost::filesystem::path(filename)));
  } catch (std::runtime_error &) {
    return false;
  }

  return true;
}