
#include "UtilsFunctions.h"

#include <iostream>        // for 'cin' and 'cout'
#include <sstream>         // for 'istringstream'
#include <locale>          // for wide chars and wide strings
#include <codecvt>         // for wide chars and wide strings
#include <algorithm>       // for 'std::reverse'
#include "JsonParser.h"    // for it's part in reading the input
#include "AutoQuitShell.h" // for access to 'AutoQuitShell::_autoQuitShell'

using std::cout;
using std::endl;


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


stringVector convertArgv(int argc, char** argv) {
    stringVector res;
    for(int i=0; i<argc; i++) {
        res.push_back(string(argv[i]));
    }
    return res;
}


void custom_exit(int ret) {
    bool autoQuitShell = AutoQuitShell::_autoQuitShell;
    if (!autoQuitShell) {
        string dummy;
        printLine("Press Enter to quit...");
        std::getline(std::cin, dummy);
    }
    exit(ret);
}


stringVector splitString(const string& str) {
    std::vector<std::string> words;
    std::istringstream stream(str);
    std::string word;

    while (stream >> word) {
        words.push_back(word);
    }

    return words;
}


stringsPair readUserInput(int argc, const stringVector& argv) {

    bool flag_default = false;

    stringsPair inputResult = checkArguments(argc, argv, flag_default);
    string outputFilename = inputResult.first;    
    string configFilename = inputResult.second;

    printLine("\nReading json configuration file...");
    string jsonString;
    
    if (configFilename != "") {
        jsonString = JsonParser::getJsonString(configFilename);
        printLine("Race was successfully configured based on json file.\n");
    } else {

        if (flag_default) {
            printLine("Using the default configuration...");
            jsonString = JsonParser::getDefaultJsonString();
            printLine("Race was successfully configured based on default configuration.\n");
            return {jsonString, outputFilename};
        }

        printLine("No json file was given...");
        printLine("Do you want to use the default configuration? Y/n\n");
        
        string userInput;
        std::getline(std::cin, userInput);
        
        if (userInput == "Y" || userInput == "y") {
            printLine("\nUsing the default configuration...");
            jsonString = JsonParser::getDefaultJsonString();
            printLine("Race was successfully configured based on default configuration.\n");
        }
        else if (userInput == "N" || userInput == "n") {
            printLine("\nUser has chosen to not use default configuration.");
            printLine("Quitting execution.\n");
            custom_exit(0);
        }
        else {
            printLine("\nInvalid key was pressed, quitting execution.\n");
            custom_exit(1);
        }
    }
    return {jsonString, outputFilename};
}


int findFlag(int argc, const stringVector& argv, const string& flag_v1, const string& flag_v2) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == flag_v1 || argv[i] == flag_v2) {
            return i;
        }
    }
    return -1;
}


stringsPair checkArguments(int argc, const stringVector& argv, bool& flag_default) {
    int help_index = findFlag(argc, argv, "-h", "--help");
    int name_index = findFlag(argc, argv, "-n", "--name");
    int default_index = findFlag(argc, argv, "-d", "--default");
    if (help_index != -1) {
        if (help_index != 1) {
            printLine("\nError: The help option must be the first flag (if provided).");
            printLine("Use --help or -h by itself to see full usage instructions.\n");
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
        printLine("\nError: Incorrect order of flags.");
        printLine("Use --help or -h by itself to see full usage instructions.\n");
        custom_exit(1);
    }

    string outputFilename = defaultOutputFilename;
    string configFilename = "";
    if (name_index != -1) {
        if ((name_index + 1 >= argc) || (name_index + 1 >= default_index && default_index != -1)) {
            printLine("\nError: No output filename provided after --name or -n option.");
            printLine("Use --help or -h by itself to see full usage instructions.\n");
            custom_exit(1);
        }
        outputFilename = string(argv[name_index + 1]);
    }

    if (default_index != -1) {
        flag_default = true;
    } else {
        if (argc > name_index + 2) {
            configFilename = argv[argc - 1];
        }
    }
    return {outputFilename, configFilename};
}


void printLine(const string& str, char end)  {
    cout << str << end;
}


void printHelpMessage(const string& progName) {

    printLine();
    printLine("    Race to The Technion - Excel Score Sheet Generator");
    printLine("============================================================");
    printLine();
    printLine("This program generates an Excel score sheet for the yearly Race to The Technion based on a provided JSON configuration file.");
    printLine("If no configuration file is provided, the program can use a default configuration.");
    printLine("The output Excel file will be named 'race_to_the_technion_scores.xlsx' by default, but a custom name can be specified.");
    printLine();
    printLine("Configuration File Format:");
    printLine("The configuration file should be in JSON format and include the following fields:");
    printLine("  - teams: An integer representing the number of teams participating in the race.");
    printLine("  - activities: An array of activity objects, each containing:");
    printLine("      - name: A string representing the name of the activity.");
    printLine("      - location: A string representing the location of the activity.");
    printLine("      - makeWorksheet: A boolean indicating whether to create a separate worksheet for the activity (true) or not (false).");
    printLine("      - rows: An integer representing the number of rows (sub-tasks or checkpoints) in the activity.");
    printLine("      - numberTable: A boolean indicating whether the activity uses a numeric input table (true) or checkboxes (false).");
    printLine("      - pointsPerRow: An integer representing the points awarded for each completed row. Relevant only if numberTable is false.");
    printLine();
    printLine("Usage:");
    printLine();
    printLine("    " + string(progName) + " [help option] [name options] [configuration options]");
    printLine();
    printLine("Options:");
    printLine("  --help, -h");
    printLine("        Show this help message and exit.");
    printLine("        This option must be the first flag if provided.");
    printLine();
    printLine("  --name, -n");
    printLine("        Use custom output filename.");
    printLine("        This option must be followed by the desired output filename.");
    printLine("        If no name options are provided, the output file will be named 'race_to_the_technion.xlsx'.");
    printLine("        Note: The output filename should not include the .xlsx extension, as it will be added automatically.");
    printLine();
    printLine("  --default, -d");
    printLine("        Use default configuration without prompting.");
    printLine("        No need to specify json file.");
    printLine();
    printLine("  If no configuration options are provided, the program will prompt the user whether to use the default configuration or not.");
    printLine("  If both name options and configuration options are provided, the program will use the specified name and configuration file.");
    printLine();
    printLine("The program was created by Amit Zlatkin, a graduate of the 11th class of Odyssey at the Techion, for the use of future generations.");
    printLine("For any issues, please contact me at amit.zlatkin@campus.technion.ac.il");
    printLine("For more information and for the most up-to-date version - check out the github repository at:");
    printLine();
    printLine("    https://github.com/AmitZlatkin/Race-to-The-Technion-Excel-Maker");
    printLine();
}


