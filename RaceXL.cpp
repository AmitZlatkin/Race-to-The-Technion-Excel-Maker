
#include "RaceXL.h"

#include "libXL/OpenXLSX.hpp" // for the namespace 'OpenXLSX'
#include "UtilsFunctions.h"   // for the different utilities
#include "XL_Functions.h"     // for working cleanly with the excel file during 'setupExcelFile'
#include "JsonParser.h"       // for reading the json part of the input
#include "RaceActivity.h"     // for 'RaceActivity'
#include "AutoQuitShell.h"    // for access to 'AutoQuitShell::_autoQuitShell'

using std::cout;
using std::endl;
using std::string;
using namespace OpenXLSX;


void RaceXL::readShellInput(int argc, char** argv) {

    stringVector conv_argv;
    
    if (AutoQuitShell::_autoQuitShell) {
        conv_argv = convertArgv(argc, argv);
        this->processedUserInput = readUserInput(conv_argv.size(), conv_argv);
        return;
    }

    string userInput;
    printLine();
    printLine("Please enter the command line arguments (or press Enter to use default configuration):");
    printLine();
    printLine("./RaceXL.exe", ' ');
    std::getline(std::cin, userInput);
    if (!userInput.empty()) {
        userInput = "./RaceXL.exe " + userInput + " ";  // " " to ensure the last argument is read correctly
        conv_argv = splitString(userInput);
    } else {
        conv_argv.push_back("./RaceXL.exe");
    }
    
    this->processedUserInput = readUserInput(conv_argv.size(), conv_argv);
}


void RaceXL::setupExcelFile() const {
    const stringsPair& processedUserInput = this->processedUserInput;

    string jsonString = processedUserInput.first;
    string outputFilename = processedUserInput.second;

    std::vector<RaceActivity> activities;
    int teams = 0;
    JsonParser::parseFullJsonString(jsonString, teams, activities);

    printLine("Opening '" + makeHebrewReadable(outputFilename) + ".xlsx" + "' Excel Document...");
    XLDocument doc;
    doc.create(outputFilename + ".xlsx", XLForceOverwrite);
    auto race_excel = doc.workbook();
    printLine("'" + makeHebrewReadable(outputFilename) + ".xlsx" + "' Excel Document Opened\n");

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

    printLine("Saving '" + makeHebrewReadable(outputFilename) + ".xlsx" + "' Excel Document...");
    doc.save();
    doc.close();
    printLine("Done!\n");
}


