#include <algorithm>
#include <sstream>

#include <wx/app.h>
#include <wx/aui/tabmdi.h>
#include <wx/colour.h>
#include <wx/debug.h>
#include <wx/settings.h>
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

  using AuiChildFrame = wxDocChildFrameAny<wxAuiMDIChildFrame, wxAuiMDIParentFrame>;

  auto pChildFrame = new AuiChildFrame(doc, this, wxStaticCast(wxGetApp().GetTopWindow(), wxAuiMDIParentFrame),
                                       wxID_ANY, doc->GetUserReadableName());
  wxASSERT(pChildFrame == GetFrame());

  mInfoBar = new wxInfoBar(pChildFrame);

  mpGrid = new wxGrid(pChildFrame, wxID_ANY);
  mpGrid->EnableEditing(false);
  mpGrid->SetCellHighlightColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT)); // it is white by default

  wxASSERT(doc);
  mpCsvDocument = dynamic_cast<CsvDocument *>(doc);
  wxASSERT(mpCsvDocument);

  mpCsvGridTable = new CsvGridTable(mpCsvDocument);
  BOOST_LOG_SEV(gLogger, trivial::trace) << "created wxGrid & CsvGridTable";

  mpGauge = new wxGauge(pChildFrame, wxID_ANY, 100);

  auto *vSizer = new wxBoxSizer(wxVERTICAL);
  vSizer->Add(mInfoBar, wxSizerFlags().Expand());
  vSizer->Add(mpGrid, wxSizerFlags(1).Expand());
  vSizer->Add(mpGauge, wxSizerFlags(0).Expand());
  pChildFrame->SetSizerAndFit(vSizer);

#ifndef NDEBUG
  wxLog::SetActiveTarget(new wxLogWindow(NULL, "Log"));
#endif

  Bind(wxEVT_THREAD, &CsvView::OnThreadEvent, this);
  pChildFrame->Show();

  MainFrame::toggleViewMenu(true);
  return true;
};

bool CsvView::OnClose(bool deleteWindow) {
  mpGrid->SetTable(mpCsvGridTable, true /* Very important! */, wxGrid::wxGridSelectNone);

  if (!wxView::OnClose(deleteWindow)) {
    return false;
  }

  // Activate(false);

  clearStatus();

  if (deleteWindow) {
    GetFrame()->Destroy();
    SetFrame(NULL);
  }

  MainFrame::toggleViewMenu(false);
  return true;
};

void CsvView::OnActivateView(bool activate, wxView *activeView, wxView *deactiveView) {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "activate=" << activate
                                         << ", GetFilename()=" << GetDocument()->GetFilename();
  if ((mIsActive = activate)) {
    clearStatus();
    showStatus();
  }
};

void CsvView::showStatus() {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();

  auto pStatusBar = MainFrame::statusBar();

  if (mpCsvGridTable->hasData()) {

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
  }

  wxASSERT(mpCsvDocument);
  if (mpCsvDocument->isNumLinesLimitReached() && !mInfoBarShown) {
    mInfoBar->ShowMessage("Limit on number of records has been reached!", wxICON_WARNING);
    mInfoBarShown = true;
  }

  if (pStatusBar->GetStatusText(2) == "") {
    std::wstring statusText{};
    auto separator = mpCsvDocument->separator();
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
    auto quote = mpCsvDocument->quote();
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
    auto escape = mpCsvDocument->escape();
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
  auto pStatusBar = MainFrame::statusBar();

  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();

  for (auto i = 0; i < pStatusBar->GetFieldsCount(); ++i) {
    pStatusBar->SetStatusText("", i);
  }
  BOOST_LOG_SEV(gLogger, trivial::trace) << "status bar cleared";
}

void CsvView::OnUpdate(wxView *sender, wxObject *hint) {
  clearStatus();
  showStatus();
};

// This method is called on the GUI thread
void CsvView::OnThreadEvent(const wxThreadEvent &event) {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  const auto numLines = event.GetPayload<std::size_t>();
  const auto percent = event.GetInt();

  BOOST_LOG_SEV(gLogger, trivial::trace) << "numLines=" << numLines << ", percent=" << percent;

  wxGridSizesInfo colSizes{};
  wxGridCellCoords cellCoords{};
  if (mIsActive) {
    colSizes = mpGrid->GetColSizes();
    cellCoords = mpGrid->GetGridCursorCoords();
    wxLogDebug("GetGridCursorCoords: r=%d, c=%d", cellCoords.GetRow(), cellCoords.GetCol());
  }

  wxASSERT(numLines);
  mpCsvGridTable->setNumberRows(numLines, percent);
  mpGrid->SetTable(mpCsvGridTable, false, wxGrid::wxGridSelectNone);

  if (mIsActive) {
    mpGrid->SetColSizes(colSizes);

    if ((cellCoords != wxGridNoCellCoords) && (cellCoords != mpGrid->GetGridCursorCoords())) {
      mpGrid->GoToCell(cellCoords);
      wxLogDebug("GoToCell: r=%d, c=%d", cellCoords.GetRow(), cellCoords.GetCol());
    }

    if (!mGridRefreshed) {
      mpGrid->ForceRefresh();
      mGridRefreshed = true;
    }
    showStatus();
  }
};

void CsvView::defaultColSize() {
  auto defaultWidth = mpGrid->GetDefaultColSize();
  wxGridUpdateLocker updateLocker(mpGrid);
  for (int i = 0; i < mpGrid->GetNumberCols(); ++i) {
    mpGrid->SetColSize(i, defaultWidth);
  }
};

void CsvView::autoSizeColLabelSize() {
  wxGridUpdateLocker updateLocker(mpGrid);
  for (int i = 0; i < mpGrid->GetNumberCols(); ++i) {
    mpGrid->AutoSizeColLabelSize(i);
  }
};

CsvGridTable *CsvView::gridTable() {
  wxASSERT(mpCsvGridTable);
  return mpCsvGridTable;
};

void CsvView::goToCol(int col) {
  wxASSERT(col < mpGrid->GetNumberCols());
  if (mpGrid->GetNumberRows()) {
    // If grid cursor doesn't have any valid position, row and column are set to -1
    mpGrid->GoToCell(std::max(mpGrid->GetGridCursorRow(), 0), col);
    mpGrid->SetFocus();
  }

  auto sizeBefore = mpGrid->GetColSize(col);
  wxGridUpdateLocker updateLocker(mpGrid);
  mpGrid->AutoSizeColLabelSize(col);
  auto sizeAfter = mpGrid->GetColSize(col);
  if (sizeBefore > sizeAfter) {
    mpGrid->SetColSize(col, sizeBefore);
  }
};

void CsvView::goToRow(int row) {
  wxASSERT(row >= 1 && row <= mpGrid->GetNumberRows());
  --row;
  // If grid cursor doesn't have any valid position, row and column are set to -1
  mpGrid->GoToCell(row, std::max(mpGrid->GetGridCursorCol(), 0));
  mpGrid->SetFocus();
};
