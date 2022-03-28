#include <sstream>

#include <wx/app.h>
#include <wx/debug.h>
#include <wx/docmdi.h>
#include <wx/sizer.h>

#include "CsvTable/log.hpp"
#include "CsvTable/utilities.hpp"
#include "csv_document.hpp"
#include "csv_view.hpp"
#include "main.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(CsvView, wxView);

CsvView::CsvView() : wxView() {
  if (!mpsThousandsSep) {
    // "C" locale is essential here!
    CsvView::mpsThousandsSep.reset(new std::locale(std::locale("C"), new thousand_sep_numpunct()));
  }
};

bool CsvView::OnCreate(wxDocument *doc, long flags) {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  if (!wxView::OnCreate(doc, flags)) {
    return false;
  }

  auto pChildFrame = new wxDocMDIChildFrame(doc, this, dynamic_cast<wxDocMDIParentFrame *>(wxGetApp().GetTopWindow()),
                                            wxID_ANY, "Child Frame");
  wxASSERT(pChildFrame == GetFrame());

  mpGrid = new wxGrid(pChildFrame, wxID_ANY);
  wxASSERT(doc);
  auto pCsvDocument = dynamic_cast<CsvDocument *>(doc);
  wxASSERT(pCsvDocument);

  mpCsvGridTable.reset(new CsvGridTable(pCsvDocument));
  wxASSERT(mpCsvGridTable);
  BOOST_LOG_SEV(gLogger, trivial::trace) << "created wxGrid & CsvGridTable";

  mpGauge = new wxGauge(pChildFrame, wxID_ANY, 100);

  auto *vSizer = new wxBoxSizer(wxVERTICAL);
  vSizer->Add(mpGrid, wxSizerFlags(1).Expand());
  vSizer->Add(mpGauge, wxSizerFlags(0).Expand());
  pChildFrame->SetSizerAndFit(vSizer);

  Bind(wxEVT_THREAD, &CsvView::OnThreadEvent, this);
  pChildFrame->Show();

  return true;
};

bool CsvView::OnClose(bool deleteWindow = true) {
  if (!wxView::OnClose(deleteWindow)) {
    return false;
  }

  clearStatus();

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
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  wxASSERT(mpCsvGridTable->hasData());
  auto pStatusBar = App::statusBar();

  std::stringstream ss;
  wxASSERT(mpsThousandsSep);
  ss.imbue(*mpsThousandsSep);

  BOOST_LOG_SEV(gLogger, trivial::trace) << "mpCsvGridTable->GetNumberRows()=" << mpCsvGridTable->GetNumberRows();

  ss << mpCsvGridTable->GetNumberRows() << " data records";
  const auto percent = mpCsvGridTable->getPercent();
  mpGauge->SetValue(percent);
  if (percent < 100) {
    ss << " (" << percent << "%)";
  } else {
    GetFrame()->GetSizer()->Hide(mpGauge);
    GetFrame()->GetSizer()->Layout();
  }
  pStatusBar->SetStatusText(ss.str(), 0);

  if (pStatusBar->GetStatusText(1) == "") {
    BOOST_LOG_SEV(gLogger, trivial::trace) << "mpCsvGridTable->GetNumberCols()=" << mpCsvGridTable->GetNumberCols();
    ss.str("");
    ss << mpCsvGridTable->GetNumberCols() << " columns";
    pStatusBar->SetStatusText(ss.str(), 1);
  }

  CsvDocument *pCsvDocument{};

  if (pStatusBar->GetStatusText(2) == "") {
    std::wstring statusText{};
    if (!pCsvDocument) {
      pCsvDocument = dynamic_cast<CsvDocument *>(GetDocument());
      wxASSERT(pCsvDocument);
    }
    auto separator = pCsvDocument->separator();
    switch (separator) {
    case kTab:
      statusText = L"Tab";
      break;
    case kPipe:
      statusText = L"Vertical bar";
      break;
    case kSemicolon:
      statusText = L"Semicolon";
      break;
    case kComma:
      statusText = L"Comma";
      break;
    case kSpace:
      statusText = L"Space";
      break;
    default:
      statusText = separator;
      break;
    };
    pStatusBar->SetStatusText(L"separator: " + statusText, 2);
  }

  if (pStatusBar->GetStatusText(3) == "") {
    std::wstring statusText{};
    if (!pCsvDocument) {
      pCsvDocument = dynamic_cast<CsvDocument *>(GetDocument());
      wxASSERT(pCsvDocument);
    }
    auto quote = pCsvDocument->quote();
    switch (quote) {
    case kDoubleQuote:
      statusText = L"Double";
      break;
    case kSingleQuote:
      statusText = L"Single";
      break;
    default:
      wxASSERT(false);
      break;
    }
    pStatusBar->SetStatusText(L"quote: " + statusText, 3);
  }

  if (pStatusBar->GetStatusText(4) == "") {
    std::wstring statusText{};
    if (!pCsvDocument) {
      pCsvDocument = dynamic_cast<CsvDocument *>(GetDocument());
      wxASSERT(pCsvDocument);
    }
    auto escape = pCsvDocument->escape();
    switch (escape) {
    case kBackslash:
      statusText = L"Backslash";
      break;
    case kNull:
      statusText = L"None";
      break;
    default:
      statusText = escape;
      break;
    };
    pStatusBar->SetStatusText(L"escape: " + statusText, 4);
  }
};

void CsvView::clearStatus() {
  auto pStatusBar = App::statusBar();

  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();

  for (auto i = 0; i < pStatusBar->GetFieldsCount(); ++i) {
    pStatusBar->SetStatusText("", i);
  }
  BOOST_LOG_SEV(gLogger, trivial::trace) << "status bar cleared";
}

// This method is called on the GUI thread
void CsvView::OnThreadEvent(const wxThreadEvent &event) {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  const auto numLines = event.GetPayload<std::size_t>();
  const auto percent = event.GetInt();

  BOOST_LOG_SEV(gLogger, trivial::trace) << "numLines=" << numLines << ", percent=" << percent;

  wxASSERT(numLines);
  mpCsvGridTable->setNumberRows(numLines, percent);
  mpGrid->SetTable(mpCsvGridTable.get(), false);

  if (mIsActive) {
    /* If the file is so small that its cells fit entirely inside of the grid (in other words, the last cell is
       visible), the grid appears empty, and ForceRefresh() is necessary to display the cells. */
    if ((numLines > 1) && (mpGrid->IsVisible(numLines - 2, mpCsvGridTable->GetNumberCols() - 1, false))) {
      BOOST_LOG_SEV(gLogger, trivial::trace) << "mpGrid->ForceRefresh()";
      mpGrid->ForceRefresh();
    }

    showStatus();
  }
};
