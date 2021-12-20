#include <wx/docmdi.h>

#include "csv_view.hpp"
#include "main.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(CsvView, wxView);

bool CsvView::OnCreate(wxDocument *doc, long flags) {
  if (!wxView::OnCreate(doc, flags))
    return false;

  auto pChildFrame = new wxDocMDIChildFrame(
      doc, this, dynamic_cast<wxDocMDIParentFrame *>(wxGetApp().GetTopWindow()),
      wxID_ANY, "Child Frame");
  wxASSERT(pChildFrame == GetFrame());
  mpGrid = new wxGrid(pChildFrame, wxID_ANY);
  mpGrid->CreateGrid(100, 10);
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
}