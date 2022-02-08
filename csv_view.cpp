#include <cstddef>
#include <wx/docmdi.h>

#include "CsvTable/log.hpp"
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

  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "created wxGrid";

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

// This method is called on the GUI thread
void CsvView::OnThreadEvent(const wxThreadEvent &event) {
  const decltype(mNumLines) numLines = event.GetPayload<std::size_t>();
  assert(numLines);
  [[maybe_unused]] const int percent = event.GetInt();

  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "numLines=" << numLines << ", percent=" << percent;

  if (!mpCsvGridTable) {
    auto pDocument = GetDocument();
    assert(pDocument);
    auto pCsvDocument = dynamic_cast<CsvDocument *>(pDocument);
    assert(pCsvDocument);
    mpCsvGridTable = new CsvGridTable(pCsvDocument);
    assert(mpCsvGridTable);
    mpCsvGridTable->setNumberRows(numLines);
  } else {
    mpCsvGridTable->setNumberRows(numLines);
  }

  mpGrid->SetTable(mpCsvGridTable, false);
  BOOST_LOG_SEV(gLogger, trivial::trace) << "called wxGrid::SetTable";
  // mpGrid->ForceRefresh();
};
