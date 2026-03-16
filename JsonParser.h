
#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>         // for 'std::string'
#include <vector>         // for 'std::vector'
#include "RaceActivity.h" // for 'RaceActivity'

using std::string;


/**
 * @brief A class o handle the reading of the json
 * 
 * @note This class isn't responsible for reading the entire input, only for the correct handling of the json part
 */
class JsonParser
{
public:

    /**
     * getDefaultJsonString: Returns the default JSON configuration string
     * 
     * @return the default JSON configuration string
     */
    static string getDefaultJsonString();


    /**
     * getJsonString: Reads a JSON file and returns its content as a string
     * 
     * @param jsonFilePath the path to the JSON file
     * 
     * @return the content of the JSON file as a string
     */
    static string getJsonString(const string& jsonFilePath);


    /**
     * parseFullJsonString: Parses the full JSON string to extract the number of teams and the activities
     * 
     * @param jsonString the input JSON string
     * @param teams reference to store the number of teams
     * @param activities reference to store the list of activities
     */
    static void parseFullJsonString(const string& jsonString, int& teams, std::vector<RaceActivity>& activities);


    /**
     * parseActivityFromJson: Parses a JSON string representing a single activity and returns a RaceActivity object
     * 
     * @param jsonString the input JSON string representing a single activity
     * 
     * @return the parsed RaceActivity object
     */
    static RaceActivity parseActivityFromJson(const string& jsonString);
};

#endif // JSON_PARSER_H


