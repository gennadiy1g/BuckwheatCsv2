#include "csv_document.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(CsvDocument, wxDocument);

bool CsvDocument::DoOpenDocument(const wxString &filename) {
  Modify(false);
  return true;
}