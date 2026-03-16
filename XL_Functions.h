
#ifndef XL_FUNCTIONS_H
#define XL_FUNCTIONS_H

#include "libXL/OpenXLSX.hpp" // for the namespace 'OpenXLSX'
#include <string>             // for 'std::string'
#include "RaceActivity.h"     // for 'RaceActivity'

using std::string;
using namespace OpenXLSX;


static const XLColor dark_green = XLColor(53, 104, 84);
static const XLColor light_grey = XLColor(246, 248, 249);
static const XLColor white = XLColor(255, 255, 255);
static const XLColor black = XLColor(0, 0, 0);


/**
 * @brief A class to handle the editing of the Excel spreadsheet
 */
class XL_Functions {
public:

    /**
     * Remove all sheets except the first one, and rename it to "ניקוד"
     * 
     * @param wb the workbook to clean
     * @param outputFilename the name of the output file (for logging purposes)
     */
    static void cleanWorkbook(XLWorkbook& wb, const string& outputFilename);


    /**
     * Initialize the top row of a worksheet with team names
     * 
     * @param wks the worksheet to initialize
     */
    static void initWorksheet(const XLWorksheet& wks, int teams, int columnShift = 0);


    /**
     * Initialize the scores worksheet with team names and total formulas
     * 
     * @param doc the document containing the worksheet
     * @param wks the worksheet to initialize
     * @param teams the number of teams
     * @param activities the number of activities
     */
    static void initScoresWorksheet(XLDocument& doc, XLWorksheet& wks, int teams, int activities);


    /**
     * Initialize an activity worksheet with checkboxes
     * 
     * @param wks the worksheet to initialize
     * @param teams the number of teams
     * @param rows the number of rows (participants) in the activity
     * @param numberTable whether the activity is a number table (true) or a checkbox table (false)
     */
    static void initActivityWorksheet(const XLWorksheet& wks, int teams, int rows, bool numberTable);


    /**
     * Set the total formulas in the scores worksheet
     * 
     * @param scores_wks reference to the scores worksheet, formulas will be set here
     * @param teams the number of teams
     * @param row_num the row number to set the formulas in
     */
    static void setTotalFormulas(XLWorksheet& scores_wks, int teams, int row_num);


    /**
     * Set the delta time formulas in the scores worksheet
     * 
     * @param doc reference to the document containing the styles
     * @param wks reference to the scores worksheet, formulas will be set here
     * @param teams the number of teams
     */
    static void setDeltaTimeFormulas(XLDocument& doc, XLWorksheet& wks, int teams);


    /**
     * Set the formulas for an activity in the scores worksheet
     * 
     * @param scores_wks reference to the scores worksheet, formulas will be set here
     * @param teams the number of teams
     * @param formula_row_num the row number to set the formulas in
     * @param activity the activity to set the formulas for
     */
    static void setActivityFormulas(XLWorksheet& scores_wks, int teams, int formula_row_num, const RaceActivity& activity);


    /**
     * Get a range of cells from a worksheet
     * 
     * @param wks the worksheet to get the range from
     * @param topLeft the top left cell of the range
     * @param bottomRight the bottom right cell of the range
     * 
     * @return the cell range
     */
    static XLCellRange getRange(const XLWorksheet& wks, const XLCell& topLeft, const XLCell& bottomRight);


    /**
     * Style a row of cells with the given parameters
     * 
     * @param doc the document containing the styles
     * @param row the row of cells to style
     * @param fontColor the font color to apply
     * @param makeBold whether to make the font bold
     * @param fillColor the fill color to apply
     * @param horizontalBorders whether to apply horizontal borders (top and bottom)
     */
    static void styleRow(XLDocument& doc, XLCellRange& row, const XLColor& fontColor, bool makeBold, const XLColor& fillColor, bool horizontalBorders);
};

#endif // XL_FUNCTIONS_H


