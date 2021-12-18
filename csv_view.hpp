#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/docview.h>

class CsvView : public wxView {
public:
    CsvView() : wxView() {};
    virtual void OnDraw(wxDC *dc) override {};
private:
    wxDECLARE_DYNAMIC_CLASS(CsvView);
};
