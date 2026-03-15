#include "UtilsFunctions.h"
#include "AutoQuitShell.h"
#include "RaceXL.h"


#ifdef WIN
#define auto_quit_shell_flag argc>1
#else
#define auto_quit_shell_flag true
#endif

int main(int argc, char** argv) {
    
    RaceXL xlCreator(auto_quit_shell_flag);

    xlCreator.readShellInput(argc, argv);
    xlCreator.execute();

    #ifdef WIN
    if (!AutoQuitShell::_autoQuitShell) {
        printLine("Press Enter to quit...");
        std::cin.ignore(); // Waits for Enter (ignores one line of input)
    }
    #endif

    return 0;
}

