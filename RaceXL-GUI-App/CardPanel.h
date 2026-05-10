#pragma once

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include "RaceActivity.h"

class CardPanel : public wxPanel
{
public:
    CardPanel(wxWindow* parent, const RaceActivity& activity, size_t index);

private:
    RaceActivity m_activity;
    size_t m_index;

    void DrawTag(wxDC& dc, int& x, int y, const wxString& text, const wxColour& bg, const wxColour& fg);

    void DrawButton(wxDC& dc, int x, int y, int w, const wxString& label);

    void OnPaint(wxPaintEvent&);
};

