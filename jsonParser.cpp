#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <sstream>
#include <algorithm>
#include "jsonParser.h"
#include "functions.h"

using std::cout;
using std::endl;
using std::string;


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


string makeHebrewReadable(const string& text) {
    // Convert UTF-8 string to wide string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring w_text = converter.from_bytes(text);
    if (containsHebrew(w_text)) {
        std::reverse(w_text.begin(), w_text.end());
    }
    return converter.to_bytes(w_text);
}


string getDefaultJsonString() {
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


string getJsonString(const string& jsonFilePath, bool autoQuitCMD) {
    std::ifstream jsonFile = std::ifstream(jsonFilePath);

    string jsonString;
    if (!jsonFile.good()) {
        printLine("Error: Could not open JSON file: " + jsonFilePath + "\n");
        jsonString = "";
    }
    if (jsonString == "") {
        custom_exit(1, autoQuitCMD);
    }

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

// Skip whitespace and commas
void skipWhitespaceAndComma(const string& s, int& pos) {
    while (pos < s.size() && (isspace(s[pos]) || s[pos] =='\n' || s[pos] == ',')) ++pos;
}



void parseFullJsonString(const string& jsonString, int& teams, std::vector<RaceActivity>& activities) {
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
        cout << "Invalid number of teams: " << teams << endl;
        exit(0);
    }

    if (activities.empty()) {
        cout << "No activities found." << endl;
        exit(0);
    }

    if (activities.size() < teams) {
        cout << "Number of activities (" << activities.size() << ") is less than the number of teams (" << teams << ")." << endl;
        exit(0);
    }

    if (activities.size() > teams) {
        cout << "Warning: Number of activities (" << activities.size() << ") is greater than the number of teams (" << teams << ")." << endl;
    }
}


RaceActivity parseActivityFromJson(const string& jsonString)
{
    RaceActivity race;
    int pos = 1;
    while (jsonString[pos] != '}') {
        skipWhitespaceAndComma(jsonString, pos);
        string itemKey = parseString(jsonString, pos);
        skipWhitespaceAndComma(jsonString, pos);
        if (jsonString[pos] == ':') ++pos;
        skipWhitespaceAndComma(jsonString, pos);

        if (itemKey == "name") {
            race.m_name = parseString(jsonString, pos);
        } else if (itemKey == "location") {
            race.m_location = parseString(jsonString, pos);
        } else if (itemKey == "makeWorksheet") {
            string val = parsePrimitive(jsonString, pos);
            race.m_makeWorksheet = (val == "true");
        } else if (itemKey == "rows") {
            string val = parsePrimitive(jsonString, pos);
            race.m_rows = std::stoi(val);
        } else if (itemKey == "numberTable") {
            string val = parsePrimitive(jsonString, pos);
            race.m_numberTable = (val == "true");
        } else if (itemKey == "pointsPerRow") {
            string val = parsePrimitive(jsonString, pos);
            race.m_pointsPerRow = std::stoi(val);
        }
    }
    
    return race;
}


