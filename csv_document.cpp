#include <boost/filesystem.hpp>
#include <wx/string.h>

#include "CsvTable/log.hpp"
#include "csv_document.hpp"
#include "csv_view.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(CsvDocument, wxDocument);

bool CsvDocument::DoOpenDocument(const wxString &file) {
  Modify(false);

  using namespace std::placeholders; // for _1, _2, _3...
  mOnProgress = std::bind(&CsvDocument::OnProgress, this, _1, _2);
  try {
    mpTokenizedFileLines.reset(new TokenizedFileLines(boost::filesystem::path(file), mOnProgress));
    assert(mpTokenizedFileLines);
    BOOST_LOG_FUNCTION();
    auto &gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, trivial::trace) << "created TokenizedFileLines";
  } catch (std::runtime_error &) {
    return false;
  }

  return true;
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
  assert(mNumLines);
  // The 1st line contains columns' names, do not count the 1st line as a data row
  int value = mNumLines - 1;
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "return value=" << value;
  return value;
};

int CsvGridTable::GetNumberCols() {
  assert(mNumLines);
  int value{0};
  value = mpCsvDocument->getTokenizedFileLines()->numColumns();
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "return value=" << value;
  return value;
};

wxString CsvGridTable::GetValue(int row, int col) {
  // the 1st line contains columns' names, the 2nd line is the 1st data row
  assert(mNumLines);
  wxString value{""};
  value = getValueAux(row + 1, col);
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "row=" << row << ", col=" << col << "return value=" << value;
  return value;
};

wxString CsvGridTable::GetColLabelValue(int col) {
  assert(mNumLines);
  wxString value{""};
  value = getValueAux(0, col);
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "col=" << col << "return value=" << value;
  return value;
};

wxString CsvGridTable::getValueAux(int row, int col) {
  wxString value{"."};
  auto tokenizedLine = mpCsvDocument->getTokenizedFileLines()->getTokenizedLine(row);
  if (col < static_cast<int>(tokenizedLine->size())) {
    value = tokenizedLine->at(col);
  }
  BOOST_LOG_FUNCTION();
  auto &gLogger = GlobalLogger::get();
  BOOST_LOG_SEV(gLogger, trivial::trace) << "row=" << row << ", col=" << col << ", return value=" << value;
  return value;
};

void CsvGridTable::setNumberRows(std::size_t numRows) {
  assert(numRows);
  mNumLines = numRows;
};
