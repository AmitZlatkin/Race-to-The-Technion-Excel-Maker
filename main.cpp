#include <iostream>
#include <vector>
#include <utility>
#include <sstream>

#include "functions.h"
#include "libXL/OpenXLSX.hpp"
#include "RaceActivity.h"
#include "JsonParser.h"

using std::cout;
using std::endl;
using std::string;
using namespace OpenXLSX;


int main(int argc, char** argv) {

    stringVector conv_argv = convertArgv(argc, argv);
    stringsPair userInput = readUserInput(argc, conv_argv);

    string jsonString = userInput.first;
    string outputFilename = userInput.second;

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

    return 0;
}

