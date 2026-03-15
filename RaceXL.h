#ifndef RACE_XL_H
#define RACE_XL_H

#include "UtilsFunctions.h"

class RaceXL {
    stringsPair processedUserInput;
public:
    explicit RaceXL(bool autoQuitShell = true);
    void readShellInput(int argc, char** argv);
    void execute() const;
};

#endif // RACE_XL_H