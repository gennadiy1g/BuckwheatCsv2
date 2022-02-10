#include <cstddef>
#include <sstream>

#include <wx/app.h>
#include <wx/docmdi.h>

#include "CsvTable/log.hpp"
#include "csv_document.hpp"
#include "csv_view.hpp"
#include "main.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(CsvView, wxView);

CsvView::CsvView() : wxView() {
  if (!mpsThousandsSep) {
    // CsvView::msThousandsSep = std::make_unique<std::locale>(std::locale("C"), new thousand_sep_numpunct());
    CsvView::mpsThousandsSep.reset(new std::locale(std::locale("C"), new thousand_sep_numpunct()));
  }
};

bool CsvView::OnCreate(wxDocument *doc, long flags) {
  if (!wxView::OnCreate(doc, flags)) {
    return false;
  }

  auto pChildFrame = new wxDocMDIChildFrame(doc, this, dynamic_cast<wxDocMDIParentFrame *>(wxGetApp().GetTopWindow()),
                                            wxID_ANY, "Child Frame");
  wxASSERT(pChildFrame == GetFrame());
  mpGrid = new wxGrid(pChildFrame, wxID_ANY);
  auto pDocument = GetDocument();
  assert(pDocument);
  auto pCsvDocument = dynamic_cast<CsvDocument *>(pDocument);
  assert(pCsvDocument);
  mpCsvGridTable.reset(new CsvGridTable(pCsvDocument));
  assert(mpCsvGridTable);

  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "created wxGrid & CsvGridTable";

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

void CsvView::OnActivateView(bool activate, wxView *activeView, wxView *deactiveView) {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "activate=" << activate
                                         << ", GetFilename()=" << GetDocument()->GetFilename();
  if ((mIsActive = activate)) {
    showStatus();
  }
};

void CsvView::showStatus() {
  auto pTopFrame = dynamic_cast<wxFrame *>(wxTheApp->GetTopWindow());
  assert(pTopFrame);
  auto pStatusBar = pTopFrame->GetStatusBar();
  assert(pStatusBar);
  std::stringstream ss;
  if (mpCsvGridTable->getNumLines()) {
    assert(mpsThousandsSep);
    ss.imbue(*mpsThousandsSep);
    ss << mpCsvGridTable->GetNumberRows() << " data records";
    if (mpCsvGridTable->getPercent() < 100) {
      ss << " (" << mpCsvGridTable->getPercent() << "%)";
    }
  }
  pStatusBar->SetStatusText(ss.str());
};

// This method is called on the GUI thread
void CsvView::OnThreadEvent(const wxThreadEvent &event) {
  const std::size_t numLines = event.GetPayload<std::size_t>();
  [[maybe_unused]] const int percent = event.GetInt();

  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "numLines=" << numLines << ", percent=" << percent;

  assert(numLines);
  mpCsvGridTable->setNumberRows(numLines);
  mpCsvGridTable->setPercent(percent);
  BOOST_LOG_SEV(gLogger, trivial::trace) << "calling wxGrid::SetTable";
  mpGrid->SetTable(mpCsvGridTable.get(), false);

  if ((mIsActive)) {
    showStatus();
  }
};
