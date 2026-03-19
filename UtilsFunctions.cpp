
#include "UtilsFunctions.h"

#include <iostream>        // for 'cin' and 'cout'
#include <sstream>         // for 'istringstream'
#include <locale>          // for wide chars and wide strings
#include <codecvt>         // for wide chars and wide strings
#include <algorithm>       // for 'std::reverse'
#include "JsonParser.h"    // for it's part in reading the input
#include "AutoQuitShell.h" // for access to 'AutoQuitShell'
#include <filesystem>      // for working with the directory
#include <map>             // for 'std::map'

using std::cout;
using std::endl;
namespace fs = std::filesystem;


// Checks if a single wide character is Hebrew
bool isHebrewChar(wchar_t ch) {
    return (ch >= 0x0590 && ch <= 0x05FF);
}


// Checks if a wide string contains any Hebrew characters
bool containsHebrew(const std::wstring& winput) {
    for (wchar_t ch : winput) {
        if (isHebrewChar(ch)) {
            return true; // At least one Hebrew character found
        }
    }
    return false;
}


string makeHebrewReadable(const string& hebText) {
    // Convert UTF-8 string to wide string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring w_text = converter.from_bytes(hebText);
    if (containsHebrew(w_text)) {
        std::reverse(w_text.begin(), w_text.end());
    }
    return converter.to_bytes(w_text);
}


string makeHebrewReadablePath(const string& path) {

    string result;

    bool first = true;
    for (const string& step : splitString(path, '/')) {
        if (!first) result.append("/");
        result.append(makeHebrewReadable(step));
        first = false;
    }

    return result;
}


stringVector convertArgv(int argc, char** argv) {
    stringVector res;
    for(int i=0; i<argc; i++) {
        res.push_back(string(argv[i]));
    }
    return res;
}


void custom_exit(int ret) {
    if (!AutoQuitShell::getInstance().getFlag()) {
        string dummy;
        printLine("Press Enter to quit...", COLOUR_TYPE::WHITE);
        std::getline(std::cin, dummy);
    }
    exit(ret);
}


stringVector splitString(const string& str, char delimiter) {
    stringVector words;
    std::istringstream stream(str);
    string word;

    while (std::getline(stream, word, delimiter)) {
        words.push_back(word);
    }

    return words;
}


stringsPair readUserInput(const stringVector& argv) {

    bool flag_default = false;

    stringsPair inputResult = checkArguments(argv, flag_default);
    string outputFilename = inputResult.first;    
    string configFilename = inputResult.second;

    if (!fs::path(outputFilename).has_extension()) {
        outputFilename += ".xlsx";
    }

    printLine("\nStarting json configuration...", COLOUR_TYPE::BLUE);
    string jsonString;
    
    if (configFilename != "") {
        if (!fs::path(configFilename).has_extension()) {
            configFilename += ".json";
        }
        jsonString = JsonParser::getJsonString(configFilename);
        printLine("Race was successfully configured based on json file.\n", COLOUR_TYPE::GREEN);
        return {jsonString, outputFilename};
    }

    if (flag_default) {
        printLine("Using the default configuration...", COLOUR_TYPE::WHITE);
        jsonString = JsonParser::getDefaultJsonString();
        printLine("Race was successfully configured based on default configuration.\n", COLOUR_TYPE::GREEN);
        return {jsonString, outputFilename};
    }

    printLine("No json file was given...", COLOUR_TYPE::YELLOW);
    printLine("\nDo you want to use the default configuration? Y/n\n", COLOUR_TYPE::BRIGHT_YELLOW);
    
    string userInput;
    std::getline(std::cin, userInput);
    
    if (userInput == "Y" || userInput == "y") {
        printLine("\nUsing the default configuration...", COLOUR_TYPE::WHITE);
        jsonString = JsonParser::getDefaultJsonString();
        printLine("Race was successfully configured based on default configuration.\n", COLOUR_TYPE::GREEN);
    }
    else if (userInput == "N" || userInput == "n") {
        printLine("\nUser has chosen to not use default configuration.", COLOUR_TYPE::CYAN);
        printLine("Quitting execution.\n", COLOUR_TYPE::CYAN);
        custom_exit(0);
    }
    else {
        printLine("\nInvalid key was pressed, quitting execution.\n", COLOUR_TYPE::BRIGHT_RED);
        custom_exit(1);
    }
    return {jsonString, outputFilename};
}


int findFlag(const stringVector& argv, const string& flag_v1, const string& flag_v2) {
    for (int i = 1; i < argv.size(); ++i) {
        if (argv[i] == flag_v1 || argv[i] == flag_v2) {
            return i;
        }
    }
    return -1;
}


stringsPair checkArguments(const stringVector& argv, bool& flag_default) {
    int help_index = findFlag(argv, "-h", "--help");
    int name_index = findFlag(argv, "-n", "--name");
    int default_index = findFlag(argv, "-d", "--default");
    if (help_index != -1) {
        if (help_index != 1) {
            printLine("\nError: The help option must be the first flag (if provided).", COLOUR_TYPE::BRIGHT_RED);
            printLine("Use ", COLOUR_TYPE::NO_COLOUR, "");
            printLine("--help", COLOUR_TYPE::CYAN, "");
            printLine(" or ", COLOUR_TYPE::NO_COLOUR, "");
            printLine("-h", COLOUR_TYPE::CYAN, "");
            printLine(" by itself to see full usage instructions.\n");
            custom_exit(1);
        }
        printHelpMessage(argv[0]);
        custom_exit(0);
    }

    // correct order of flags: help, name, output filename, default, json filename
    
    bool correct_order = true;
    if (name_index != -1 && name_index <= help_index) correct_order = false;
    if (default_index != -1 && default_index <= name_index) correct_order = false;
    if (!correct_order) {
        printLine("\nError: Incorrect order of flags.", COLOUR_TYPE::BRIGHT_RED);
        printLine("Use ", COLOUR_TYPE::NO_COLOUR, "");
        printLine("--help", COLOUR_TYPE::CYAN, "");
        printLine(" or ", COLOUR_TYPE::NO_COLOUR, "");
        printLine("-h", COLOUR_TYPE::CYAN, "");
        printLine(" by itself to see full usage instructions.\n");
        custom_exit(1);
    }

    string outputFilename = defaultOutputFilename;
    string configFilename = "";
    if (name_index != -1) {
        if ((name_index + 1 >= argv.size()) || (name_index + 1 >= default_index && default_index != -1)) {
            printLine("\nError: No output filename provided after --name or -n option.", COLOUR_TYPE::BRIGHT_RED);
            printLine("Use ", COLOUR_TYPE::NO_COLOUR, "");
            printLine("--help", COLOUR_TYPE::CYAN, "");
            printLine(" or ", COLOUR_TYPE::NO_COLOUR, "");
            printLine("-h", COLOUR_TYPE::CYAN, "");
            printLine(" by itself to see full usage instructions.\n");
            custom_exit(1);
        }
        outputFilename = string(argv[name_index + 1]);
    }

    if (default_index != -1) {
        flag_default = true;
    } else {
        if (argv.size() > name_index + 2) {
            configFilename = argv[argv.size() - 1];
        }
    }
    return {outputFilename, configFilename};
}


static const std::map<COLOUR_TYPE, int> ansi_map = {
    { COLOUR_TYPE::BLACK,          30 },
    { COLOUR_TYPE::BRIGHT_BLACK,   90 },
    { COLOUR_TYPE::RED,            31 },
    { COLOUR_TYPE::BRIGHT_RED,     91 },
    { COLOUR_TYPE::GREEN,          32 },
    { COLOUR_TYPE::BRIGHT_GREEN,   92 },
    { COLOUR_TYPE::YELLOW,         33 },
    { COLOUR_TYPE::BRIGHT_YELLOW,  93 },
    { COLOUR_TYPE::BLUE,           34 },
    { COLOUR_TYPE::BRIGHT_BLUE,    94 },
    { COLOUR_TYPE::MAGENTA,        35 },
    { COLOUR_TYPE::BRIGHT_MAGENTA, 95 },
    { COLOUR_TYPE::CYAN,           36 },
    { COLOUR_TYPE::BRIGHT_CYAN,    96 },
    { COLOUR_TYPE::WHITE,          37 },
    { COLOUR_TYPE::BRIGHT_WHITE,   97 },
    { COLOUR_TYPE::NO_COLOUR,       0 }
};


/**
 * Get the ANSI colour code for a given COLOUR_TYPE
 * 
 * @param color the COLOUR_TYPE for which to get the ANSI code
 * 
 * @return the ANSI colour code corresponding to the given COLOUR_TYPE
 */
int getAnsiCode(COLOUR_TYPE color) {
    if (ansi_map.count(color) != 0) {
        return ansi_map.at(color);
    }
    return 0; // No color, just to be safe
}


/** Function to set the console text colour using ANSI escape
 * 
 * @param textColour the text colour
*/
void setColour(COLOUR_TYPE textColour) {
    cout << "\033[" << getAnsiCode(textColour) << "m";
}

// Function to reset the console color
void resetColour() { setColour(COLOUR_TYPE::NO_COLOUR); }


void printLine(const string& str, COLOUR_TYPE textColor, string end)  {
    setColour(textColor);
    cout << str << end;
    resetColour();
}


void printHelpMessage(const string& progName) {

    printLine();
    printLine();
    printLine("============================================================================================", COLOUR_TYPE::BRIGHT_WHITE);
    printLine();
    printLine("                     Race to The Technion - Excel Score Sheet Generator", COLOUR_TYPE::BRIGHT_CYAN);
    printLine();
    printLine("============================================================================================", COLOUR_TYPE::BRIGHT_WHITE);
    printLine();
    printLine("This program generates an Excel score sheet for the yearly Race to The Technion based on a");
    printLine("provided JSON configuration file.");
    printLine("The '", COLOUR_TYPE::NO_COLOUR, "");
    printLine(".json", COLOUR_TYPE::YELLOW, "");
    printLine("' file extension can be omitted, it will be added automatically if missing.");
    printLine("If no configuration JSON file is provided, the program can use a default configuration.");
    printLine("The output Excel file will be named '", COLOUR_TYPE::NO_COLOUR, "");
    printLine("race-to-the-technion.xlsx", COLOUR_TYPE::BRIGHT_MAGENTA, "");
    printLine("' by default, but a custom");
    printLine("name can be specified.");
    printLine();
    printLine("Configuration File Format:", COLOUR_TYPE::BRIGHT_CYAN);
    printLine("The configuration file should be in JSON format and include the following fields:");
    printLine("  - ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("teams", COLOUR_TYPE::CYAN, "");
    printLine(": An integer representing the number of teams participating in the race.");
    printLine("  - ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("activities", COLOUR_TYPE::CYAN, "");
    printLine(": An array of activity objects, each one of them represents a single activity.");
    printLine();
    printLine("Each \"activity\" object in the mentioned list has the following fields:");
    printLine("  - ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("name", COLOUR_TYPE::CYAN, "");
    printLine(": A string representing the name of the activity.");
    printLine("  - ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("location", COLOUR_TYPE::CYAN, "");
    printLine(": A string representing the location of the activity.");
    printLine("  - ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("makeWorksheet", COLOUR_TYPE::CYAN, "");
    printLine(": A boolean indicating whether to create a separate worksheet for the");
    printLine("                   activity (true) or not (false).");
    printLine("  - ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("rows", COLOUR_TYPE::CYAN, "");
    printLine(": An integer representing the number of rows (sub-tasks or checkpoints) that should");
    printLine("          be in the activity's spreadsheet.");
    printLine("  - ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("numberTable", COLOUR_TYPE::CYAN, "");
    printLine(": A boolean indicating whether the activity uses a numeric input table (true)");
    printLine("                 or checkboxes (false).");
    printLine("  - ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("pointsPerRow", COLOUR_TYPE::CYAN, "");
    printLine(": An integer representing the points awarded for each completed row.");
    printLine("                  Relevant only if numberTable is false.");
    printLine();
    printLine("Usage:", COLOUR_TYPE::BRIGHT_CYAN);
    printLine();
    printLine("    " + string(progName) + " [help option] [name options] [configuration options]");
    printLine();
    printLine("Options:", COLOUR_TYPE::BRIGHT_CYAN);
    printLine("  --help", COLOUR_TYPE::CYAN, "");
    printLine(", ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("-h", COLOUR_TYPE::CYAN);
    printLine("        Show this help message and exit.");
    printLine("        This option must be the first flag if provided.");
    printLine();
    printLine("  --name", COLOUR_TYPE::CYAN, "");
    printLine(", ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("-n", COLOUR_TYPE::CYAN);
    printLine("        Use custom output filename.");
    printLine("        This option must be followed by the desired output filename.");
    printLine("        If no name options are provided, the output file will be named: '", COLOUR_TYPE::NO_COLOUR, "");
    printLine("race-to-the-technion.xlsx", COLOUR_TYPE::BRIGHT_MAGENTA, "");
    printLine("'.");
    printLine();
    printLine("        Note", COLOUR_TYPE::BRIGHT_YELLOW, "");
    printLine(": The output filename doesn't have to include the '", COLOUR_TYPE::NO_COLOUR, "");
    printLine(".xlsx", COLOUR_TYPE::YELLOW, "");
    printLine("' extension, it will be");
    printLine("              added automatically if missing.");
    printLine();
    printLine("        The filename may also be a path, it will be treated as a RELATIVE path to the");
    printLine("        directory in which the app is running.");
    printLine("        The path should be specified in Unix notation.");
    printLine("        If some of the directories in the path don't exists, they will be created.");
    printLine();
    printLine("  --default", COLOUR_TYPE::CYAN, "");
    printLine(", ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("-d", COLOUR_TYPE::CYAN);
    printLine("        Use default configuration without prompting.");
    printLine("        No need to specify json file.");
    printLine();
    printLine("  If no configuration options are provided, the program will prompt the user whether to");
    printLine("use the default configuration or not.");
    printLine("  If both name options and configuration options are provided, the program will use the");
    printLine("specified name and configuration file.");
    printLine();
    printLine("The program was created by Amit Zlatkin, a graduate of the 11th class of ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("Odyssey at the Technion", COLOUR_TYPE::BRIGHT_GREEN, "");
    printLine(",");
    printLine("for the use of future generations.");
    printLine("For any issues, please contact me at ", COLOUR_TYPE::NO_COLOUR, "");
    printLine("amit.zlatkin@campus.technion.ac.il", COLOUR_TYPE::BRIGHT_BLUE);
    printLine("For more information and for the most up-to-date version - check out the github");
    printLine("repository at:");
    printLine();
    printLine("    https://github.com/AmitZlatkin/Race-to-The-Technion-Excel-Maker", COLOUR_TYPE::BLUE);
    printLine();
    printLine();
    printLine();
}


