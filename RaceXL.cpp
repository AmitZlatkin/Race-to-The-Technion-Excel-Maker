
#include "RaceXL.h"

#include "libXL/OpenXLSX.hpp" // for the namespace 'OpenXLSX'
#include "UtilsFunctions.h"   // for the different utilities
#include "XL_Functions.h"     // for working cleanly with the excel file during 'setupExcelFile'
#include "JsonParser.h"       // for reading the json part of the input
#include "RaceActivity.h"     // for 'RaceActivity'
#include "AutoQuitShell.h"    // for access to 'AutoQuitShell'
#include <filesystem>         // for creating the parent folder if needed

using std::string;
using namespace OpenXLSX;
namespace fs = std::filesystem;


string substr(const string& s, int startIndex, int endIndex) {
    int length = endIndex - startIndex + 1;
    return s.substr(startIndex, length);
}


void RaceXL::readShellInput(int argc, char** argv) {

    stringVector conv_argv;
    
    if (AutoQuitShell::getInstance().getFlag()) {
        conv_argv = convertArgv(argc, argv);
        this->processedUserInput = readUserInput(conv_argv);
        return;
    }

    string userInput;
    printLine();
    printLine("Please enter the command line arguments (or press Enter to use default configuration):");
    printLine();
    printLine("./RaceXL.exe", ' ');
    std::getline(std::cin, userInput);

    if (userInput.empty()) {
        conv_argv.push_back("./RaceXL.exe");
        this->processedUserInput = readUserInput(conv_argv);
        return;
    }
    
    stringVector temp = splitString("./RaceXL.exe " + userInput + " "); // " " to ensure the last argument is read correctly
    bool insideFilePath = false;
    for (const string& s : temp) {
        if (s.front() == '"' && s.size() > 1) { // '"' is not the only char
            insideFilePath = true;
            conv_argv.push_back(substr(s, 1, s.size()-1));
        }
        else if (s.back() == '"' && s.size() > 1) { // '"' is not the only char
            insideFilePath = false;
            conv_argv.back() += " " + substr(s, 0, s.size()-2);
        }
        else if (insideFilePath) {
            conv_argv.back() += " " + s; // add the current part to the last argument
        }
        else {
            conv_argv.push_back(s);
        }
    }

    this->processedUserInput = readUserInput(conv_argv);
}


void RaceXL::setupExcelFile() const {
    const stringsPair& processedUserInput = this->processedUserInput;

    string jsonString = processedUserInput.first;
    string outputFilename = processedUserInput.second;

    std::vector<RaceActivity> activities;
    int teams = 0;
    JsonParser::parseFullJsonString(jsonString, teams, activities);

    printLine("Opening '" + makeHebrewReadablePath(outputFilename) + ".xlsx" + "' Excel Document...");

    fs::path filepath = outputFilename;
    fs::path parentDir = filepath.parent_path();

    if (!parentDir.string().empty()) {
        try {
            if (!fs::exists(parentDir)) {
                printLine("Creating directory: '" + makeHebrewReadablePath(parentDir.string()) + "'");
                fs::create_directory(parentDir);
                printLine("Directory: '" + makeHebrewReadablePath(parentDir.string()) + "'");
            }
        } catch (const fs::filesystem_error& e) {
            printLine("Error creating directory: '" + makeHebrewReadablePath(e.what()) + "'");
            custom_exit(1);
        }
    }

    XLDocument doc;
    doc.create(outputFilename + ".xlsx", XLForceOverwrite);
    auto race_excel = doc.workbook();
    printLine("'" + makeHebrewReadablePath(outputFilename) + ".xlsx" + "' Excel Document Opened\n");

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

    printLine("Saving '" + makeHebrewReadablePath(outputFilename) + ".xlsx" + "' Excel Document...");
    doc.save();
    doc.close();
    printLine("Done!\n");
}


