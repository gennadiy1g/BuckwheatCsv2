#include <boost/filesystem.hpp>

#include "CsvTable/log.hpp"
#include "CsvTable/utilities.hpp"
#include "csv_document.hpp"
#include "csv_view.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(CsvDocument, wxDocument);

bool CsvDocument::DeleteContents() {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "calling TokenizedFileLines::stopReading()";
  if (mpTokenizedFileLines) {
    mpTokenizedFileLines->stopReading();
  }
  BOOST_LOG_SEV(gLogger, trivial::trace) << "returned from TokenizedFileLines::stopReading()";
  return true; // The return value of this method is currently ignored (docs.wxwidgets.org/3.1.5/classwx_document.html)
};

bool CsvDocument::DoOpenDocument(const wxString &file) {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  Modify(false);

  using namespace std::placeholders; // for _1, _2, _3...
  mOnProgress = std::bind(&CsvDocument::OnProgress, this, _1, _2);
  try {
    bfs::path path(file);
    auto [separator, quote] = detectSeparatorAndQuote(path);
    if (separator) {
      mpTokenizedFileLines.reset(new TokenizedFileLines(path, mOnProgress));
      assert(mpTokenizedFileLines);
      BOOST_LOG_SEV(gLogger, trivial::trace) << "created TokenizedFileLines";
      mpTokenizedFileLines->setTokenFuncParams(L'\0', separator.value(), quote.value_or(L'\"'));
    } else {
      wxMessageBox("Cannot detect the separator character!", "Warning");
    }
  } catch (const std::runtime_error &) {
  }

  return true; // if this method returns false, the application terminates
};

// This method is called on the worker thread
void CsvDocument::OnProgress(std::size_t numLines, int percent) {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "numLines=" << numLines << ", percent=" << percent;

  auto pCsvView = dynamic_cast<CsvView *>(GetFirstView());
  if (pCsvView) {
    wxThreadEvent event;
    event.SetPayload(numLines);
    event.SetInt(percent);
    pCsvView->QueueEvent(event.Clone());
    BOOST_LOG_SEV(gLogger, trivial::trace) << "queued event";
  }
};

int CsvGridTable::GetNumberRows() {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  assert(mNumLines);
  // The 1st line contains columns' names, do not count the 1st line as a data row
  int value = mNumLines - 1;
  BOOST_LOG_SEV(gLogger, trivial::trace) << "return value=" << value;
  return value;
};

int CsvGridTable::GetNumberCols() {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  assert(mNumLines);
  int value{};
  value = mpCsvDocument->getTokenizedFileLines()->numColumns();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "return value=" << value;
  return value;
};

wxString CsvGridTable::GetValue(int row, int col) {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  // the 1st line contains columns' names, the 2nd line is the 1st data row
  assert(mNumLines);
  wxString value{""};
  value = getValueAux(row + 1, col);
  BOOST_LOG_SEV(gLogger, trivial::trace) << "row=" << row << ", col=" << col << "return value=" << value;
  return value;
};

wxString CsvGridTable::GetColLabelValue(int col) {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  assert(mNumLines);
  wxString value{""};
  value = getValueAux(0, col);
  BOOST_LOG_SEV(gLogger, trivial::trace) << "col=" << col << "return value=" << value;
  return value;
};

wxString CsvGridTable::getValueAux(int row, int col) const {
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  wxString value{"."};
  auto tokenizedLine = mpCsvDocument->getTokenizedFileLines()->getTokenizedLine(row);
  if (col < static_cast<int>(tokenizedLine->size())) {
    value = tokenizedLine->at(col);
  }
  BOOST_LOG_SEV(gLogger, trivial::trace) << "row=" << row << ", col=" << col << ", return value=" << value;
  return value;
};

void CsvGridTable::setNumberRows(std::size_t numRows, int percent) {
  assert(numRows);
  mNumLines = numRows;
  mPercent = percent;
};
