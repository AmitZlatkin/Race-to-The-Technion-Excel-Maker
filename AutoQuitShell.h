
#ifndef AUTO_QUIT_SHELL_H
#define AUTO_QUIT_SHELL_H

/**
 * @brief A class that is a Singleton, used to store the global value of whether to auto quit the shell or not
 * 
 * @param _autoQuitShell The static member that serves as the global value
 */
class AutoQuitShell
{
private:

    bool _autoQuitShell; /**< The static member that serves as the global value */

    /**
     * Private constructor to prevent instantiation of the class, since it is a Singleton and only serves to store the global value of whether to auto quit the shell or not
     */
    AutoQuitShell() = default;
    
public:
    
    /**
     * The static method to get the instance of the class, since it is a Singleton
     */
    static AutoQuitShell& getInstance();


    /**
     * Set the value of whether to auto quit the shell or not
     * 
     * @param value The value to set the global value to
     */
    void setFlag(bool value);


    /**
     * Get the value of whether to auto quit the shell or not
     * 
     * @return The value of whether to auto quit the shell or not
     */
    bool getFlag() const;
};

#endif // AUTO_QUIT_SHELL_H


