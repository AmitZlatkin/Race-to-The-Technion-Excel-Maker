#ifndef UTILS_FUNCTIONS_H
#define UTILS_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>

using std::string;
using std::cout;
using std::endl;
using stringVector = std::vector<string>;
using stringsPair = std::pair<string, string>;


static const string defaultOutputFilename = "race-to-the-technion-scores";


void parseFullJsonString(const string& jsonString, int& teams, std::vector<RaceActivity>& activities);

string makeHebrewReadable(const string& hebText);



/**
 * convertArgv: convert argv from char** to vector<string>
 * @param argc argc that main() got
 * @param argv argv that main() got
 * @return the converted argv
 */
stringVector convertArgv(int argc, char** argv);


/**
 * splitString: split a string by a delimiter
 * @param str the string to split
 * @return a vector of strings, split by whitespace
 */
stringVector splitString(const string& str);


/**
 * custom_exit: use exit() function with extra steps
 * @param ret the value to give to exit()
 */
void custom_exit(int ret);


/**
 * readUserInput: read user input from command line or prompt for default configuration
 * 
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 * 
 * @return a pair containing the json configuration string and the output filename
 */
stringsPair readUserInput(int argc, const stringVector& argv);


/**
 * checkArguments: check the command line arguments for validity and set flags accordingly
 * 
 * possible flags:
 * 
 * - <file>.json : path to the json configuration file. If not provided, user will be prompted to use default configuration
 * 
 * - -d, --default : use default configuration without prompting
 * 
 * - -n, --name : specify the name of the output file
 * 
 * - -h, --help : print help message and exit
 * 
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 * @param flag_default reference to a boolean that will be set to true if the default flag is set
 * 
 * @return a pair containing the output filename (or default if not provided) and the json filename (or empty string if not provided)
 */
stringsPair checkArguments(int argc, const stringVector& argv, bool& flag_default);


/**
 * findFlag: find the index of a flag in the command line arguments
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 * @param flag_v1 the first version of the flag to search for
 * @param flag_v2 the second version of the flag to search for
 * 
 * @return the index of the flag in argv if found, -1 otherwise
 */
int findFlag(int argc, const stringVector& argv, const string& flag_v1, const string& flag_v2);


/**
 * @brief print the help message to the console
 * @param progName the name of the program (usually argv[0])
 */
void printHelpMessage(const string& progName);


/**
 * printLine: print a line to the console
 * @param str the string to print
 * @param end the line ending character (default: '\n')
 */
void printLine(const string& str = "", char end = '\n');


#endif // UTILS_FUNCTIONS_H