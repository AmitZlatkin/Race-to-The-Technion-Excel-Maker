#pragma once

#include <wx/wx.h>

struct RaceActivity
{
    wxString name;
    wxString location;
    bool makeWorksheet = true;
    bool numberTable = true;
    int rows = 3;
    int pointsPerRow = 10;
};