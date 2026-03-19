
#include "JsonParser.h"

#include <iostream>         // for 'cin' and 'cout'
#include <fstream>          // for 'istream' and 'getLine'
#include "UtilsFunctions.h" // for 'printLine' and 'custom_exit'

using std::cout;
using std::endl;
using std::string;


string JsonParser::getDefaultJsonString() {
    string defaultJsonString = R"(
    {
        "teams": 3,
        "activities": [
            {
                "name": "Activity A",
                "location": "Location A",
                "makeWorksheet": false,
                "rows": 0,
                "numberTable": false,
                "pointsPerRow": 0
            },
            {
                "name": "Activity B",
                "location": "Location B",
                "makeWorksheet": false,
                "rows": 0,
                "numberTable": false,
                "pointsPerRow": 0
            },
            {
                "name": "Activity C",
                "location": "Location C",
                "makeWorksheet": false,
                "rows": 0,
                "numberTable": false,
                "pointsPerRow": 0
            }
        ]
    }
    )";

    return defaultJsonString;
}


string JsonParser::getJsonString(const string& jsonFilePath) {
    printLine("Opening '" + makeHebrewReadablePath(jsonFilePath) + "' json configuration file...");
    std::ifstream jsonFile = std::ifstream(jsonFilePath);
    
    string jsonString;
    if (!jsonFile.good()) {
        printLine("\nError: Could not open JSON file: " + jsonFilePath + "\n", COLOUR_TYPE::BRIGHT_RED);
        custom_exit(1);
    }
    
    printLine("Reading '" + makeHebrewReadablePath(jsonFilePath) + "' json configuration file...");
    string row;
    while (!jsonFile.eof()) {
        getline(jsonFile, row);
        jsonString.append(row);
    }
    return jsonString;
}


// Simple trimming
string trim(const string& s) {
    int start = s.find_first_not_of(" \t\n\r");
    int end = s.find_last_not_of(" \t\n\r");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}


// Parses a string like: "some string"
string parseString(const string& s, int& pos) {
    string result;
    ++pos; // skip opening quote
    while (pos < s.size() && s[pos] != '"') {
        result += s[pos++];
    }
    ++pos; // skip closing quote
    return result;
}


// Parses a primitive (number or boolean)
string parsePrimitive(const string& s, int& pos) {
    string result;
    while (pos < s.size() && s[pos] != ',' && s[pos] != '}' && s[pos] != ']') {
        result += s[pos++];
    }
    return trim(result);
}


// Skip whitespace and commas - advance 'pos' while 's[pos]' is whitespace  or a comma
void skipWhitespaceAndComma(const string& s, int& pos) {
    while (pos < s.size() && (isspace(s[pos]) || s[pos] =='\n' || s[pos] == ',')) ++pos;
}


void JsonParser::parseFullJsonString(const string& jsonString, int& teams, std::vector<RaceActivity>& activities) {
    int pos = 0;

    while (pos < jsonString.size()) {
        skipWhitespaceAndComma(jsonString, pos);

        if (jsonString[pos] != '"') {
            ++pos;
            continue;
        }

        string key = parseString(jsonString, pos);
        skipWhitespaceAndComma(jsonString, pos);

        if (jsonString[pos] == ':') ++pos;
        skipWhitespaceAndComma(jsonString, pos);

        if (key == "teams") {
            string value = parsePrimitive(jsonString, pos);
            teams = std::stoi(value);
        }
        else if (key == "activities") {
            if (jsonString[pos] == '[') ++pos; // enter array

            while (pos < jsonString.size() && jsonString[pos] != ']') {
                skipWhitespaceAndComma(jsonString, pos);

                if (jsonString[pos] == '{') {
                    
                    int size = 1;
                    while (jsonString[pos+size] != '}') { size++; }
                    
                    string activityJson = jsonString.substr(pos, size+1);
                    activities.push_back(parseActivityFromJson(activityJson));
                    
                    while (jsonString[pos] != '}') {
                        ++pos; // skip to after the closing }
                    }
                    ++pos; // skip the closing }

                }

            }

            skipWhitespaceAndComma(jsonString, pos);
            if (jsonString[pos] == ']') ++pos; // end of array
        }
    }

    if (teams <= 0) {
        printLine("Invalid number of teams: " + std::to_string(teams) + "\n", COLOUR_TYPE::BRIGHT_RED);
        custom_exit(0);
    }

    if (activities.empty()) {
        printLine("No activities found.\n", COLOUR_TYPE::BRIGHT_RED);
        custom_exit(0);
    }

    if (activities.size() < teams) {
        printLine("Number of activities (" + std::to_string(activities.size()) + ") is less than the number of teams (" + std::to_string(teams) + ").\n", COLOUR_TYPE::BRIGHT_RED);
        custom_exit(0);
    }

    if (activities.size() > teams) {
        printLine("Warning: Number of activities (" + std::to_string(activities.size()) + ") is greater than the number of teams (" + std::to_string(teams) + ").\n", COLOUR_TYPE::YELLOW);
    }
}


RaceActivity JsonParser::parseActivityFromJson(const string& jsonString)
{
    RaceActivity activity;
    int pos = 1;
    while (jsonString[pos] != '}') {
        skipWhitespaceAndComma(jsonString, pos);
        string itemKey = parseString(jsonString, pos);
        skipWhitespaceAndComma(jsonString, pos);
        if (jsonString[pos] == ':') ++pos;
        skipWhitespaceAndComma(jsonString, pos);

        if (itemKey == "name") {
            activity.m_name = parseString(jsonString, pos);
        } else if (itemKey == "location") {
            activity.m_location = parseString(jsonString, pos);
        } else if (itemKey == "makeWorksheet") {
            string val = parsePrimitive(jsonString, pos);
            activity.m_makeWorksheet = (val == "true");
        } else if (itemKey == "rows") {
            string val = parsePrimitive(jsonString, pos);
            activity.m_rows = std::stoi(val);
        } else if (itemKey == "numberTable") {
            string val = parsePrimitive(jsonString, pos);
            activity.m_numberTable = (val == "true");
        } else if (itemKey == "pointsPerRow") {
            string val = parsePrimitive(jsonString, pos);
            activity.m_pointsPerRow = std::stoi(val);
        }
    }
    
    return activity;
}


