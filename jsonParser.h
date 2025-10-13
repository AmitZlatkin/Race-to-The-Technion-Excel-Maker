#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <vector>
#include <fstream>
#include "RaceActivity.h"

using std::string;


/**
 * makeHebrewReadable: Makes a UTF-8 string readable in Hebrew by reversing it if it contains Hebrew characters
 * @param hebText the input UTF-8 string
 * @return the reversed string if it contains Hebrew characters, otherwise returns the original string
 */
string makeHebrewReadable(const string& hebText);


/**
 * getDefaultJsonString: Returns the default JSON configuration string
 * @return the default JSON configuration string
 */
string getDefaultJsonString();


/**
 * getJsonString: Reads a JSON file and returns its content as a string
 * @param jsonFilePath the path to the JSON file
 * @return the content of the JSON file as a string
 */
string getJsonString(const string& jsonFilePath, bool quitCMD = true);


/**
 * parseFullJsonString: Parses the full JSON string to extract the number of teams and the activities
 * @param jsonString the input JSON string
 * @param teams reference to store the number of teams
 * @param activities reference to store the list of activities
 */
void parseFullJsonString(const string& jsonString, int& teams, std::vector<RaceActivity>& activities);


/**
 * parseActivityFromJson: Parses a JSON string representing a single activity and returns a RaceActivity object
 * @param jsonString the input JSON string representing a single activity
 * @return the parsed RaceActivity object
 */
RaceActivity parseActivityFromJson(const string& jsonString);

#endif // JSONPARSER_H