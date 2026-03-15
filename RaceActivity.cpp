#include "RaceActivity.h"

using std::string;

RaceActivity::RaceActivity(const string& name, const string& location, bool makeWorksheet, int rows, bool numberTable, int rowPoints)
    : m_name(name), m_location(location), m_makeWorksheet(makeWorksheet), m_rows(rows), m_numberTable(numberTable), m_pointsPerRow(rowPoints)
{}