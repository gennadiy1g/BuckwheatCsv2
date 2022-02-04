#include <cstddef>
#include <wx/docmdi.h>

#include "csv_document.hpp"
#include "csv_view.hpp"
#include "main.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(CsvView, wxView);

bool CsvView::OnCreate(wxDocument *doc, long flags) {
  if (!wxView::OnCreate(doc, flags)) {
    return false;
  }

  auto pChildFrame = new wxDocMDIChildFrame(doc, this, dynamic_cast<wxDocMDIParentFrame *>(wxGetApp().GetTopWindow()),
                                            wxID_ANY, "Child Frame");
  wxASSERT(pChildFrame == GetFrame());

  mpGrid = new wxGrid(pChildFrame, wxID_ANY);
  Bind(wxEVT_THREAD, &CsvView::OnThreadEvent, this);

  pChildFrame->Show();

  return true;
};

bool CsvView::OnClose(bool deleteWindow = true) {
  if (!wxView::OnClose(deleteWindow)) {
    return false;
  }

  Activate(false);

  if (deleteWindow) {
    GetFrame()->Destroy();
    SetFrame(NULL);
  }

  return true;
};

void CsvView::OnThreadEvent(const wxThreadEvent &event) {
  const decltype(mNumLines) numLines = event.GetPayload<std::size_t>();
  [[maybe_unused]] const int percent = event.GetInt();

  assert(numLines >= mNumLines);
  if (numLines > mNumLines) {
    mpGrid->AppendRows(numLines - mNumLines);
    mNumLines = numLines;
  }
};

void CsvView::setGridTable(wxGridTableBase *table) {
  if (!mGridTableIsSet) {
    mpGrid->AssignTable(table);
  }
  mGridTableIsSet = true;
};
