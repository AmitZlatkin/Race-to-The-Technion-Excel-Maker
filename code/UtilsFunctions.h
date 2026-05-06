
#ifndef UTILS_FUNCTIONS_H
#define UTILS_FUNCTIONS_H

#include <string>         // for 'std::string'
#include <vector>         // for 'std::vector'
#include <utility>        // for 'std::pair'
#include "RaceActivity.h" // for 'RaceActivity'

using std::string;
using stringVector = std::vector<string>;
using stringsPair = std::pair<string, string>;


static const string defaultOutputFilename = "race-to-the-technion";


/**
 * Make a UTF-8 string readable in Hebrew by reversing it if it contains Hebrew characters
 * 
 * @param hebText the input UTF-8 string
 * 
 * @return The reversed string if it contains Hebrew characters, otherwise returns the original string
 */
string makeHebrewReadable(const string& hebText);


/**
 * Make a UTF-8 path string readable in Hebrew by reversing each step if it contains Hebrew characters
 * 
 * @param path the input UTF-8 string
 * 
 * @return The reversed string if it contains Hebrew characters, otherwise returns the original string
 */
string makeHebrewReadablePath(const string& path);


/**
 * Convert argv from char** to std::vector<std::string>
 * 
 * @param argc argc that main() got
 * @param argv argv that main() got
 * 
 * @return A vector of strings that represents argv
 */
stringVector convertArgv(int argc, char** argv);


/**
 * Split a string by whitespace
 * 
 * @param str the string to split
 * @param delimiter the character to split by (default: ' ')
 * 
 * @return A vector of strings, split by whitespace
 */
stringVector splitString(const string& str, char delimiter = ' ');


/**
 * Use the exit() function with extra steps, allows the desired behavior in the App Icon version on windows
 * 
 * @param ret The value to give to exit()
 */
void custom_exit(int ret);


/**
 * Read the user input from the command line, and, if needend, prompt for the default configuration
 * 
 * @param argv the command line arguments
 * 
 * @return A pair containing the json configuration string and the output filename
 */
stringsPair readUserInput(const stringVector& argv);


/**
 * Check the command line arguments for validity and set flags accordingly
 * 
 * #### Possible flags:
 * 
 * - <file>.json : path to the json configuration file. If not provided, user will be prompted to use default configuration.
 * 
 * - -d, --default : use default configuration without prompting.
 * 
 * - -n, --name : specify the name of the output file.
 * 
 * - -h, --help : print help message and exit.
 * 
 * @param argv the command line arguments
 * @param flag_default reference to a boolean that will be set to true if the default flag is set
 * 
 * @return A pair containing the output filename (or default if not provided) and the json filename (or empty string if not provided)
 */
stringsPair checkArguments(const stringVector& argv, bool& flag_default);


/**
 * Find the index of a flag in the command line arguments
 * 
 * @param argv the command line arguments
 * @param flag_v1 the first version of the flag to search for
 * @param flag_v2 the second version of the flag to search for
 * 
 * @return the index of the flag in argv if found, -1 otherwise
 */
int findFlag(const stringVector& argv, const string& flag_v1, const string& flag_v2);


/**
 * Print the help message to the console
 * 
 * @param progName the name of the program (usually just argv[0] but not always)
 */
void printHelpMessage(const string& progName);


/**
 * An enum representing the different colors that can be used in the console output
 */
enum COLOUR_TYPE {
    BLACK,   BRIGHT_BLACK,
    RED,     BRIGHT_RED,
    GREEN,   BRIGHT_GREEN,
    YELLOW,  BRIGHT_YELLOW,
    BLUE,    BRIGHT_BLUE,
    MAGENTA, BRIGHT_MAGENTA,
    CYAN,    BRIGHT_CYAN,
    WHITE,   BRIGHT_WHITE,

    NO_COLOUR,

    NUM_COLOURS
};


/**
 * Print a line to the console
 * 
 * @param str the string to print
 * @param textColor the color to print the text in (default: no color)
 * @param end the line ending character (default: '\\n')
 */
void printLine(const string& str = "", COLOUR_TYPE textColor = COLOUR_TYPE::NO_COLOUR, string end = "\n");


#endif // UTILS_FUNCTIONS_H


