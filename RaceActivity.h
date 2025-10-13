#ifndef RACE_ACTIVITY_H
#define RACE_ACTIVITY_H

#include <string>
using std::string;

/**
 * RaceActivity: A struct to hold information
 * 
 * @param m_name The name of the activity
 * @param m_location The location of the activity
 * @param m_makeWorksheet Whether to create a worksheet for this activity
 * @param m_rows The number of rows (participants) in the activity
 * @param m_numberTable Whether the activity is a number table (true) or a checkbox table (false)
 * @param m_pointsPerRow The points per row (used if not a number table)
 * 
 * @note The struct includes constructors for easy initialization
 */
struct RaceActivity
{
    string m_name;
    string m_location;
    bool m_makeWorksheet;
    int m_rows;
    bool m_numberTable;
    int m_pointsPerRow;

    /**
     * Default constructor
     */
    RaceActivity() = default;

    /**
     * Parameterized constructor
     * 
     * @param name The name of the activity
     * @param location The location of the activity
     * @param makeWorksheet Whether to create a worksheet for this activity
     * @param rows The number of rows (participants) in the activity
     * @param numberTable Whether the activity is a number table (true) or a checkbox table (false)
     * @param rowPoints The points per row (used if not a number table)
     */
    RaceActivity(const string& name, const string& location, bool makeWorksheet, int rows, bool numberTable, int rowPoints);
};

#endif // RACE_ACTIVITY_H