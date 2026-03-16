
#include "AutoQuitShell.h"

AutoQuitShell& AutoQuitShell::getInstance() {
    static AutoQuitShell inst; // Guaranteed to be destroyed, instantiated on first use
    return inst;
}


void AutoQuitShell::setFlag(bool value) { _autoQuitShell = value; }


bool AutoQuitShell::getFlag() const { return _autoQuitShell; }


