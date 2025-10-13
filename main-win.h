#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>

#include "libXL/OpenXLSX.hpp"
#include "RaceActivity.h"

using std::string;
using std::cout;
using std::endl;
using stringsPair = std::pair<string, string>;
using namespace OpenXLSX;

static const string defaultOutputFilename = "race-to-the-technion-scores.xlsx";

static const XLColor dark_green = XLColor(53, 104, 84);
static const XLColor light_grey = XLColor(246, 248, 249);
static const XLColor white = XLColor(255, 255, 255);
static const XLColor black = XLColor(0, 0, 0);

/**
 * printLine: print a line to the console
 * @param str the string to print
 * @param end the line ending character (default: '\n')
 */
void printLine(const string& str = "", char end = '\n') {
    cout << str << end;
}


/**
 * cleanWorkbook: remove all sheets except the first one, and rename it to "ניקוד"
 * @param wb the workbook to clean
 * @param outputFilename the name of the output file (for logging purposes)
 */
void cleanWorkbook(XLWorkbook& wb, const string& outputFilename);


/**
 * initWorksheet: initialize the top row of a worksheet with team names
 * @param wks the worksheet to initialize
 */
void initWorksheet(const XLWorksheet& wks, int teams, int columnShift = 0);


/**
 * initScoresWorksheet: initialize the scores worksheet with team names and total formulas
 * @param doc the document containing the worksheet
 * @param wks the worksheet to initialize
 * @param teams the number of teams
 * @param activities the number of activities
 */
void initScoresWorksheet(XLDocument& doc, XLWorksheet& wks, int teams, int activities);


/**
 * initActivityWorksheet: initialize an activity worksheet with checkboxes
 * @param wks the worksheet to initialize
 * @param teams the number of teams
 * @param rows the number of rows (participants) in the activity
 * @param numberTable whether the activity is a number table (true) or a checkbox table (false)
 */
void initActivityWorksheet(const XLWorksheet& wks, int teams, int rows, bool numberTable);


/**
 * setTotalFormulas: set the total formulas in the scores worksheet
 * @param scores_wks reference to the scores worksheet, formulas will be set here
 * @param teams the number of teams
 * @param row_num the row number to set the formulas in
 */
void setTotalFormulas(XLWorksheet& scores_wks, int teams, int row_num);


/**
 * setActivityFormulas: set the formulas for an activity in the scores worksheet
 * @param scores_wks reference to the scores worksheet, formulas will be set here
 * @param teams the number of teams
 * @param formula_row_num the row number to set the formulas in
 * @param activity the activity to set the formulas for
 */
void setActivityFormulas(XLWorksheet& scores_wks, int teams, int formula_row_num, const RaceActivity& activity);


/**
 * getRange: get a range of cells from a worksheet
 * @param wks the worksheet to get the range from
 * @param topLeft the top left cell of the range
 * @param bottomRight the bottom right cell of the range
 * @return the cell range
 */
XLCellRange getRange(const XLWorksheet& wks, const XLCell& topLeft, const XLCell& bottomRight);


/**
 * styleRow: style a row of cells with the given parameters
 * @param doc the document containing the styles
 * @param row the row of cells to style
 * @param fontColor the font color to apply
 * @param makeBold whether to make the font bold
 * @param fillColor the fill color to apply
 * @param horizontalBorders whether to apply horizontal borders (top and bottom)
 */
void styleRow(XLDocument& doc, XLCellRange& row, const XLColor& fontColor, bool makeBold, const XLColor& fillColor, bool horizontalBorders);


/**
 * splitString: split a string by a delimiter
 * @param str the string to split
 * @return a vector of strings, split by whitespace
 */
std::vector<string> splitString(const string& str);


/**
 * readUserInput: read user input from command line or prompt for default configuration
 * 
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 * @return a pair containing the json configuration string and the output filename
 */
stringsPair readUserInput(int argc, std::vector<string> argv);


/**
 * checkArguments: check the command line arguments for validity and set flags accordingly
 * 
 * possible flags:
 * 
 * - <file>.json : path to the json configuration file. If not provided, user will be prompted to use default configuration
 * 
 * - -d, --default : use default configuration without prompting
 * 
 * - -n, --name : specify the name of the output file
 * 
 * - -h, --help : print help message and exit
 * 
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 * @param flag_default reference to a boolean that will be set to true if the default flag is set
 * 
 * @return a pair containing the output filename (or default if not provided) and the json filename (or empty string if not provided)
 */
stringsPair checkArguments(int argc, std::vector<string> argv, bool& flag_default);


/**
 * findFlag: find the index of a flag in the command line arguments
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 * @param flag_v1 the first version of the flag to search for
 * @param flag_v2 the second version of the flag to search for
 * @return the index of the flag in argv if found, -1 otherwise
 */
int findFlag(int argc, std::vector<string> argv, const string& flag_v1, const string& flag_v2);


/**
 * @brief print the help message to the console
 * @param progName the name of the program (usually argv[0])
 */
void printHelpMessage(const string& progName);



// old styling functions
/*
void applyFont(XLDocument& doc, XLCellRange& cellRange, const XLColor& fontColor, bool makeBold);
void applyFill(XLDocument& doc, XLCellRange& cellRange, const XLColor& fillColor);
void applyBorder(XLDocument& doc, XLCellRange& cellRange, const XLLineStyle& lineStyle, const XLColor& borderColor);
*/

#endif // MAIN_H