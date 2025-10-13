#include <iostream>
#include <vector>
#include <utility>
#include <sstream>

#include "functions.h"
#include "libXL/OpenXLSX.hpp"
#include "RaceActivity.h"
#include "jsonParser.h"

using std::cout;
using std::endl;
using std::string;
using namespace OpenXLSX;


int main(int argc, char** argv) {

    stringsPair processedUserInput;

    if (argc <= 1) {
        string userInput;
        printLine();
        printLine("Please enter the command line arguments (or press Enter to use default configuration):");
        printLine();
        printLine("./RaceXL.exe", ' ');
        std::getline(std::cin, userInput);
        userInput = "./RaceXL.exe " + userInput + " ";  // " " to ensure the last argument is read correctly
        std::vector<string> user_argv = splitString(userInput);
        int user_argc = user_argv.size();  
        processedUserInput = readUserInput_WIN(user_argc, user_argv);
    } else {
        processedUserInput = readUserInput(argc, argv);
    }
    string jsonString = processedUserInput.first;
    string outputFilename = processedUserInput.second;

    std::vector<RaceActivity> activities;
    int teams = 0;
    parseFullJsonString(jsonString, teams, activities);

    printLine("Opening '" + makeHebrewReadable(outputFilename) + ".xlsx" + "' Excel Document...");
    XLDocument doc;
    doc.create(outputFilename + ".xlsx", XLForceOverwrite);
    auto race_excel = doc.workbook();
    printLine("'" + makeHebrewReadable(outputFilename) + ".xlsx" + "' Excel Document Opened\n");

    cleanWorkbook(race_excel, outputFilename);

    auto scores_wks = race_excel.worksheet("ניקוד");
    initScoresWorksheet(doc, scores_wks, teams, activities.size());
    setTotalFormulas(scores_wks, teams, activities.size()+2);

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
        initActivityWorksheet(activity_wks, teams, activityData.m_rows, activityData.m_numberTable);
        setActivityFormulas(scores_wks, teams, activityRow, activityData);
    }

    printLine("Saving '" + makeHebrewReadable(outputFilename) + ".xlsx" + "' Excel Document...");
    doc.save();
    doc.close();
    printLine("Done!\n");

    if (argc <= 1) {
        printLine("Press Enter to quit...");
        std::cin.ignore(); // Waits for Enter (ignores one line of input)
    }

    return 0;
}

