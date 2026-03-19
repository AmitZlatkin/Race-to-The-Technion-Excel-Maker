
#include "UtilsFunctions.h" // for 'pribtLine'
#include "AutoQuitShell.h"  // for access to 'AutoQuitShell'
#include "RaceXL.h"         // for 'RaceXL'
#include <iostream>         // for 'std::cin'

// different behavior on different platforms
// on windows - the program can be run from te terminal and via the app icon.
// on linux - currently only the terminal is supported.
#ifdef WIN
#define auto_quit_shell_flag argc>1
#else
#define auto_quit_shell_flag true
#endif


int main(int argc, char** argv) {

    AutoQuitShell::getInstance().setAutoQuitShellFlag(auto_quit_shell_flag);
    
    RaceXL xlCreator;

    xlCreator.readShellInput(argc, argv);
    xlCreator.setupExcelFile();

    // additional logic for running via windows app icon
    #ifdef WIN
    if (!AutoQuitShell::getInstance().getAutoQuitShellFlag()) {
        printLine("Press Enter to quit...");
        std::cin.ignore(); // Waits for Enter (ignores one line of input)
    }
    #endif

    return 0;
}


