#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docview.h>

class CsvDocument : public wxDocument {
public:
  CsvDocument() : wxDocument(){};

protected:
  virtual bool DoOpenDocument(const wxString &filename) override;
  wxDECLARE_DYNAMIC_CLASS(CsvDocument);
};
