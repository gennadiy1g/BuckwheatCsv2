#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docview.h>

class CsvDocument : public wxDocument
{
public:
    CsvDocument() : wxDocument() {};
protected:
    wxDECLARE_DYNAMIC_CLASS(CsvDocument);
};
