
#include "AutoQuitShell.h"

AutoQuitShell& AutoQuitShell::getInstance() {
    static AutoQuitShell inst; // Guaranteed to be destroyed, instantiated on first use
    return inst;
}


void AutoQuitShell::setAutoQuitShellFlag(bool value) { _autoQuitShellFlag = value; }


bool AutoQuitShell::getAutoQuitShellFlag() const { return _autoQuitShellFlag; }


