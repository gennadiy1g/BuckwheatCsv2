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
    clearStatus();
    BOOST_LOG_SEV(gLogger, trivial::trace) << "mpCsvGridTable->hasData()=" << mpCsvGridTable->hasData();
    if (mpCsvGridTable->hasData()) {
      showStatus();
    }
  }
};

void CsvView::showStatus() {
  assert(mpCsvGridTable->hasData());

  auto pTopFrame = dynamic_cast<wxFrame *>(wxTheApp->GetTopWindow());
  assert(pTopFrame);
  auto pStatusBar = dynamic_cast<StatusBar *>(pTopFrame->GetStatusBar());
  assert(pStatusBar);

  std::stringstream ss;
  assert(mpsThousandsSep);
  ss.imbue(*mpsThousandsSep);

  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "mpCsvGridTable->GetNumberRows()=" << mpCsvGridTable->GetNumberRows();

  ss << mpCsvGridTable->GetNumberRows() << " data records";
  if (mpCsvGridTable->getPercent() < 100) {
    ss << " (" << mpCsvGridTable->getPercent() << "%)";
    pStatusBar->getGauge()->SetValue(mpCsvGridTable->getPercent());
    if (!pStatusBar->getGauge()->IsShown()) {
      pStatusBar->getGauge()->Show(true);
    }
  } else {
    pStatusBar->getGauge()->Show(false);
  }
  pStatusBar->SetStatusText(ss.str(), 0);

  if (pStatusBar->GetStatusText(1) == "") {
    BOOST_LOG_SEV(gLogger, trivial::trace) << "mpCsvGridTable->GetNumberCols()=" << mpCsvGridTable->GetNumberCols();
    ss.str("");
    ss << mpCsvGridTable->GetNumberCols() << " columns";
    pStatusBar->SetStatusText(ss.str(), 1);
  }

  if (pStatusBar->GetStatusText(2) == "") {
    ss.str("");
    ss << "separator: comma";
    pStatusBar->SetStatusText(ss.str(), 2);
  }

  if (pStatusBar->GetStatusText(3) == "") {
    ss.str("");
    ss << "quote: double";
    pStatusBar->SetStatusText(ss.str(), 3);
  }
};

void CsvView::clearStatus() {
  auto pTopFrame = dynamic_cast<wxFrame *>(wxTheApp->GetTopWindow());
  assert(pTopFrame);
  auto pStatusBar = pTopFrame->GetStatusBar();
  assert(pStatusBar);

  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "pStatusBar->GetStatusText()=" << pStatusBar->GetStatusText();

  for (auto i = 0; i < pStatusBar->GetFieldsCount() - 1; ++i) { /* the last field is for the gauge */
    pStatusBar->SetStatusText("", i);
  }
}

// This method is called on the GUI thread
void CsvView::OnThreadEvent(const wxThreadEvent &event) {
  const std::size_t numLines = event.GetPayload<std::size_t>();
  int percent = event.GetInt();

  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "numLines=" << numLines << ", percent=" << percent;

  assert(numLines);
  mpCsvGridTable->setNumberRows(numLines);
  mpCsvGridTable->setPercent(percent);
  mpGrid->SetTable(mpCsvGridTable.get(), false);

  /* If the file is so small that its cells fit entirely inside of the grid (in other words, the last cell is
     visible), the grid appears empty, and ForceRefresh() is necessary to display the cells. */
  if ((numLines > 1) && (mpGrid->IsVisible(numLines - 2, mpCsvGridTable->GetNumberCols() - 1, false))) {
    BOOST_LOG_SEV(gLogger, trivial::trace) << "mpGrid->ForceRefresh()";
    mpGrid->ForceRefresh();
  }

  if (mIsActive) {
    showStatus();
  }
};
