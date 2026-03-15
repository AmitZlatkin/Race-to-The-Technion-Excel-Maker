
#ifndef AUTO_QUIT_SHELL_H
#define AUTO_QUIT_SHELL_H

/**
 * @brief A class with one static member, to allow for a clean global variable.
 * 
 * @param _autoQuitShell The static member that serves as the global value.
 */
class AutoQuitShell {
public:
    static bool _autoQuitShell; /**< The static member that serves as the global value. */
};

#endif // AUTO_QUIT_SHELL_H


