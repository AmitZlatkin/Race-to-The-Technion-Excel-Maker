
#ifndef RACE_XL_H
#define RACE_XL_H

#include "UtilsFunctions.h" // for 'stringsPair'


/**
 * @brief A class for the core functionality of creating the Excel Spreadsheet for the race.
 */
class RaceXL
{
public:

    /**
    * Default constructor
    */
    RaceXL() = default;

    
    /**
     * Read the user input from the command line, and, if needend, prompt for the default configuration
     * 
     * @param argc the number of command line arguments
     * @param argv the command line arguments
     */
    void readShellInput(int argc, char** argv);
    
    
    /**
     * Set up the Excel file according to the user input, including creating the worksheets, setting the formulas and styling the cells
     */
    void setupExcelFile() const;

private:
    stringsPair processedUserInput;
};

#endif // RACE_XL_H


