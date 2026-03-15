
#ifndef RACE_XL_H
#define RACE_XL_H

#include "UtilsFunctions.h" // for 'stringsPair'


/**
 * @brief A class for the core functionality of creating the Excel Spreadsheet for the race.
 */
class RaceXL {
    stringsPair processedUserInput;
public:
    
    
    /**
     * 
     */
    explicit RaceXL(bool autoQuitShell = true);
    
    
    /**
     * 
     */
    void readShellInput(int argc, char** argv);
    
    
    /**
     * 
     */
    void setupExcelFile() const;
};

#endif // RACE_XL_H


