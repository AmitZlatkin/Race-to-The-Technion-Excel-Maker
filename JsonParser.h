#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <vector>
#include "RaceActivity.h"

using std::string;

class JsonParser
{
public:

    /**
     * makeHebrewReadable: Makes a UTF-8 string readable in Hebrew by reversing it if it contains Hebrew characters
     * @param hebText the input UTF-8 string
     * @return the reversed string if it contains Hebrew characters, otherwise returns the original string
     */
    static string makeHebrewReadable(const string& hebText);


    /**
     * getDefaultJsonString: Returns the default JSON configuration string
     * @return the default JSON configuration string
     */
    static string getDefaultJsonString();


    /**
     * getJsonString: Reads a JSON file and returns its content as a string
     * @param jsonFilePath the path to the JSON file
     * @param autoQuitCMD whether to quit the cmd upon problems with the JSON file.
     *                defaulted to true, so as to not break linux app.
     * @return the content of the JSON file as a string
     */
    static string getJsonString(const string& jsonFilePath, bool autoQuitCMD = true);


    /**
     * parseFullJsonString: Parses the full JSON string to extract the number of teams and the activities
     * @param jsonString the input JSON string
     * @param teams reference to store the number of teams
     * @param activities reference to store the list of activities
     */
    static void parseFullJsonString(const string& jsonString, int& teams, std::vector<RaceActivity>& activities);


    /**
     * parseActivityFromJson: Parses a JSON string representing a single activity and returns a RaceActivity object
     * @param jsonString the input JSON string representing a single activity
     * @return the parsed RaceActivity object
     */
    static RaceActivity parseActivityFromJson(const string& jsonString);
};

#endif // JSON_PARSER_H