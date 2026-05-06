
#include "RaceXL.h"

#include "libXL/OpenXLSX.hpp" // for the namespace 'OpenXLSX'
#include "UtilsFunctions.h"   // for the different utilities
#include "XL_Functions.h"     // for working cleanly with the excel file during 'setupExcelFile'
#include "JsonParser.h"       // for reading the json part of the input
#include "RaceActivity.h"     // for 'RaceActivity'
#include "AutoQuitShell.h"    // for access to 'AutoQuitShell'
#include <filesystem>         // for working with the directory

using std::string;
using namespace OpenXLSX;
namespace fs = std::filesystem;


/**
 * Parse the command line arguments, handling quoted arguments that may contain spaces. The function modifies 'conv_argv' to contain the parsed arguments.
 * 
 * @param conv_argv the vector to store the parsed arguments in, will be cleared at the beginning of the function
 * @param argv the original command line arguments to parse, as a vector of strings
 */
void parseArgvQuotes(stringVector& conv_argv, const stringVector& argv);


void RaceXL::readShellInput(int argc, char** argv) {

    stringVector conv_argv;

    // on linux, we can just read the arguments as they are, since the terminal will handle quoted arguments with spaces correctly. On windows, we need to handle this ourselves, since the way the app is run may not allow for correct parsing of quoted arguments with spaces (e.g. when running via the app icon).
    #ifndef WIN
    conv_argv = convertArgv(argc, argv);
    this->processedUserInput = readUserInput(conv_argv);
    return;
    #endif

    parseArgvQuotes(conv_argv, convertArgv(argc, argv));
    
    if (AutoQuitShell::getInstance().getAutoQuitShellFlag()) {
        this->processedUserInput = readUserInput(conv_argv);
        return;
    }

    string userInput;
    printLine();
    printLine("Please enter the command line arguments (or press Enter to use default configuration):", COLOUR_TYPE::CYAN);
    printLine();
    printLine(" >> RaceXL.exe ", COLOUR_TYPE::NO_COLOUR, "");
    std::getline(std::cin, userInput);

    if (userInput.empty()) {
        conv_argv.clear(); // just to be safe
        conv_argv.push_back("./RaceXL.exe");
        this->processedUserInput = readUserInput(conv_argv);
        return;
    }
    
    stringVector userInputArgv = splitString("./RaceXL.exe " + userInput + " "); // " " to ensure the last argument is read correctly

    parseArgvQuotes(conv_argv, userInputArgv);

    this->processedUserInput = readUserInput(conv_argv);
}


/**
 * An enum to represent the different types of command line arguments when parsing quoted arguments. This is used in the 'parseArgvQuotes' function to determine how to handle each argument based on whether it is inside quotes or not.
 */
enum ArgumentType {
    NOT_IN_QUOTES,
    SINGLE_QUOTES,
    DOUBLE_QUOTES
};


void parseArgvQuotes(stringVector& conv_argv, const stringVector& argv) {

    conv_argv.clear();

    int currType = ArgumentType::NOT_IN_QUOTES;

    for (const string& arg : argv)
    {
        string to_add = arg;
        int len = arg.length();

        switch (currType)
        {
        case ArgumentType::NOT_IN_QUOTES:
            if ( (arg.front() == '\'' || arg.front() == '"') && len > 1) {
                to_add = arg.substr(1, len-1);
                currType = (arg.front() == '\'') ? ArgumentType::SINGLE_QUOTES : ArgumentType::DOUBLE_QUOTES;

                if ((arg.back() == '\'' && currType == ArgumentType::SINGLE_QUOTES) ||
                    ( arg.back() == '"' && currType == ArgumentType::DOUBLE_QUOTES) ) {
                    to_add = (len > 2) ? arg.substr(1, len-2) : "";
                    currType = ArgumentType::NOT_IN_QUOTES;
                }
            }
            if (!to_add.empty()) conv_argv.push_back(to_add);
            break;
        
        case ArgumentType::SINGLE_QUOTES:
            if (arg.back() == '\'' && len > 1) {
                to_add = arg.substr(0, len-1);
                currType = ArgumentType::NOT_IN_QUOTES;
            }
            conv_argv.back() += " " + to_add;
            break;
        
        case ArgumentType::DOUBLE_QUOTES:
            if (arg.back() == '"' && len > 1) {
                to_add = arg.substr(0, len-1);
                currType = ArgumentType::NOT_IN_QUOTES;
            }
            conv_argv.back() += " " + to_add;
            break;
        
        default:
            break;
        }
    }
}


void RaceXL::setupExcelFile() const {
    const stringsPair& processedUserInput = this->processedUserInput;

    string jsonString = processedUserInput.first;
    string outputFilename = processedUserInput.second;

    std::vector<RaceActivity> activities;
    int teams = 0;
    JsonParser::parseFullJsonString(jsonString, teams, activities);

    printLine("Opening '" + makeHebrewReadablePath(outputFilename) + "' Excel Document...", COLOUR_TYPE::BRIGHT_MAGENTA);

    fs::path filepath = outputFilename;
    fs::path parentDir = filepath.parent_path();

    if (!parentDir.string().empty()) {
        try {
            if (!fs::exists(parentDir)) {
                printLine("Creating directory: '" + makeHebrewReadablePath(parentDir.string()) + "'", COLOUR_TYPE::MAGENTA);
                fs::create_directories(parentDir);
                printLine("Directory: '" + makeHebrewReadablePath(parentDir.string()) + "' successfully created", COLOUR_TYPE::GREEN);
            }
        } catch (const fs::filesystem_error& e) {
            printLine("\nError creating directory '" + makeHebrewReadablePath(parentDir.string()) + "':" + makeHebrewReadablePath(e.what()) + "\n", COLOUR_TYPE::BRIGHT_RED);
            custom_exit(1);
        }
    }

    XLDocument doc;
    doc.create(outputFilename, XLForceOverwrite);
    auto race_excel = doc.workbook();
    printLine("'" + makeHebrewReadablePath(outputFilename) + "' Excel Document Opened\n", COLOUR_TYPE::GREEN);

    XL_Functions::cleanWorkbook(race_excel, outputFilename);

    auto scores_wks = race_excel.worksheet("ניקוד");
    XL_Functions::initScoresWorksheet(doc, scores_wks, teams, activities.size());
    XL_Functions::setTotalFormulas(scores_wks, teams, activities.size()+2);

    // using index i and not range-based-for or iterators because the index is also needed
    for (int i = 0; i < activities.size(); ++i) {
        const auto& activityData = activities[i];
        int activityRow = i + 2;

        scores_wks.cell(activityRow, 1).value() = activityData.m_name;
        scores_wks.cell(activityRow, 2).value() = activityData.m_location;

        if (!activityData.m_makeWorksheet) {
            continue;
        }

        race_excel.addWorksheet(activityData.m_name);
        auto activity_wks = race_excel.worksheet(activityData.m_name);
        XL_Functions::initActivityWorksheet(activity_wks, teams, activityData.m_rows, activityData.m_numberTable);
        XL_Functions::setActivityFormulas(scores_wks, teams, activityRow, activityData);
    }

    printLine("Saving '" + makeHebrewReadablePath(outputFilename) + "' Excel Document...", COLOUR_TYPE::BRIGHT_MAGENTA);
    doc.save();
    doc.close();
    printLine("Done!\n", COLOUR_TYPE::BRIGHT_GREEN);
}


