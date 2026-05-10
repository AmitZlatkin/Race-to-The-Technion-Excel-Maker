#include "CardPanel.h"

CardPanel::CardPanel(wxWindow* parent, const RaceActivity& activity, size_t index)
    : wxPanel(parent), m_activity(activity), m_index(index)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetMinSize(wxSize(-1, 180));
    Bind(wxEVT_PAINT, &CardPanel::OnPaint, this);
}

void CardPanel::DrawTag(wxDC& dc, int& x, int y, const wxString& text, const wxColour& bg, const wxColour& fg)
{
    wxSize ext = dc.GetTextExtent(text);
    int w = ext.x + 22;
    int h = 26;
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(bg));
    dc.DrawRoundedRectangle(x, y, w, h, 13);
    dc.SetTextForeground(fg);
    dc.DrawText(text, x + 11, y + 5);
    x += w + 8;
}

void CardPanel::DrawButton(wxDC& dc, int x, int y, int w, const wxString& label)
{
    dc.SetPen(wxPen(wxColour(214, 206, 194)));
    dc.SetBrush(wxBrush(wxColour(250, 248, 243)));
    dc.DrawRoundedRectangle(x, y, w, 30, 15);
    dc.SetTextForeground(wxColour(58, 50, 41));
    dc.DrawText(label, x + 12, y + 6);
}

void CardPanel::OnPaint(wxPaintEvent&)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxSize sz = GetClientSize();
    dc.SetPen(wxPen(wxColour(220, 212, 201)));
    dc.SetBrush(wxBrush(wxColour(255, 253, 249)));
    dc.DrawRoundedRectangle(0, 0, sz.x - 1, sz.y - 1, 18);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(wxColour(215, 233, 231)));
    dc.DrawRoundedRectangle(20, 18, 38, 38, 12);
    dc.SetTextForeground(wxColour(10, 106, 114));
    dc.SetFont(wxFontInfo(12).Bold());
    dc.DrawLabel(wxString::Format("%d", static_cast<int>(m_index + 1)), wxRect(20, 18, 38, 38), wxALIGN_CENTER);

    dc.SetTextForeground(wxColour(28, 24, 20));
    dc.SetFont(wxFontInfo(14).Bold());
    dc.DrawText(m_activity.name, 72, 20);
    dc.SetFont(wxFontInfo(10));
    dc.SetTextForeground(wxColour(112, 102, 91));
    dc.DrawText(m_activity.location, 72, 46);

    dc.SetFont(wxFontInfo(9).Bold());
    int tagX = 20;
    DrawTag(dc, tagX, 76, m_activity.makeWorksheet ? "Worksheet" : "No worksheet", wxColour(238, 231, 218), wxColour(98, 90, 78));
    DrawTag(dc, tagX, 76, m_activity.numberTable ? "Numeric" : "Checkbox", wxColour(215, 233, 231), wxColour(10, 106, 114));
    DrawTag(dc, tagX, 76, wxString::Format("Rows: %d", m_activity.rows), wxColour(244, 236, 226), wxColour(128, 82, 34));
    if (!m_activity.numberTable)
        DrawTag(dc, tagX, 76, wxString::Format("Pts/row: %d", m_activity.pointsPerRow), wxColour(229, 239, 221), wxColour(67, 102, 44));

    dc.SetTextForeground(wxColour(92, 84, 75));
    dc.SetFont(wxFontInfo(10));
    dc.DrawText("Edit the activity visually, then reorder, duplicate, or delete it.", 20, 118);

    int y = sz.y - 46;
    DrawButton(dc, 20, y, 82, "Edit");
    DrawButton(dc, 110, y, 96, "Duplicate");
    DrawButton(dc, 214, y, 86, "Delete");
    DrawButton(dc, sz.x - 196, y, 78, "Move up");
    DrawButton(dc, sz.x - 108, y, 88, "Move down");
}