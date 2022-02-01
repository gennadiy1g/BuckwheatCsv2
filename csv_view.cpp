#include <cstddef>
#include <wx/docmdi.h>

#include "csv_document.hpp"
#include "csv_view.hpp"
#include "main.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(CsvView, wxView);

bool CsvView::OnCreate(wxDocument *doc, long flags) {
  if (!wxView::OnCreate(doc, flags))
    return false;

  Bind(wxEVT_THREAD, &CsvView::OnThreadEvent, this);

  auto pChildFrame = new wxDocMDIChildFrame(doc, this, dynamic_cast<wxDocMDIParentFrame *>(wxGetApp().GetTopWindow()),
                                            wxID_ANY, "Child Frame");
  wxASSERT(pChildFrame == GetFrame());

  mpGrid = new wxGrid(pChildFrame, wxID_ANY);
  auto pDocument = GetDocument();
  assert(pDocument);
  auto pCsvDocument = dynamic_cast<CsvDocument *>(pDocument);
  assert(pCsvDocument);
  mpGrid->SetTable(pCsvDocument->getGridTable(), false);

  pChildFrame->Show();
  return true;
};

bool CsvView::OnClose(bool deleteWindow = true) {
  if (!wxView::OnClose(deleteWindow))
    return false;

  Activate(false);

  if (deleteWindow) {
    GetFrame()->Destroy();
    SetFrame(NULL);
  }

  return true;
};

void CsvView::OnThreadEvent(const wxThreadEvent &event) {
  __attribute__((unused)) const std::size_t numLines = event.GetPayload<std::size_t>();
  __attribute__((unused)) const int percent = event.GetInt();
};