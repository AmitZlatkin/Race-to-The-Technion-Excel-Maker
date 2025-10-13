
# Race to The Technion - Excel Score Sheet Generator

Created by Amit Zlatkin, a graduate of the 11th class of Odyssey at the Technion, for the use of future generations.\
For any issues, contact me at [amit.zlatkin@campus.technion.ac.il](amit.zlatkin@campus.technion.ac.il).\
Check out the [github repository](https://github.com/AmitZlatkin/Race-to-The-Technion-Excel-Maker) for more information and for the most up-to-date version.

## Intro:
This program generates an Excel score sheet for the yearly Race to the Technion based on a provided JSON configuration file.\
If no configuration file is provided, the program can use a default configuration.\
The output Excel file will be named 'race_to_the_technion_scores.xlsx' by default, but a custom name can be specified.

The resulting Excel file will contain a 'Scores' sheet and additional sheets for each activity as specified in the configuration.\
The 'Scores' sheet will automatically calculate the total scores for each team based on their performance in the activities.\
Activity sheets will include unformatted checkboxes or numeric input fields for score entry, depending on the activity type, and are named according to the activity names provided in the configuration.\
If an activity has a sheet, a formula will be added to the 'Scores' sheet to calculate points based on the activity's scoring rules.

**The program is available two executable files:**
* [**Linux based**](https://github.com/AmitZlatkin/Race-to-The-Technion-Excel-Maker/blob/main/RaceXL)
* [**Windows based (.exe)**](https://github.com/AmitZlatkin/Race-to-The-Technion-Excel-Maker/blob/main/RaceXL.exe)

## Configuration File Format:
The configuration file should be in JSON format and include the following fields:
  - teams: An integer representing the number of teams participating in the race.
  - activities: An array of activity objects, each containing:
      - name: A string representing the name of the activity.
      - location: A string representing the location of the activity.
      - makeWorksheet: A boolean indicating whether to create a separate worksheet for the activity (true) or not (false).
      - rows: An integer representing the number of rows (sub-tasks or checkpoints) in the activity.
      - numberTable: A boolean indicating whether the activity uses a numeric input table (true) or checkboxes (false).
      - pointsPerRow: An integer representing the points awarded for each completed row.\
        Relevant only if numberTable is false.

You can see an [example JSON at the end](#example-json-configuration).

## Usage:
### On Linux:
```
./RaceXL [help option] [name options] [configuration options]
```
### On Windows:
```
./RaceXL.exe [help option] [name options] [configuration options]
```
Or just click the app icon (the terminal will be opened automatically).
### Options:
```
--help, -h
```
Show the help message and exit.
This option must be the first flag if provided.
```
--name, -n
```
Use custom output filename.\
This option must be followed by the desired output filename.\
If no name options are provided, the output file will be named 'race_to_the_technion.xlsx'.\
Note: The output filename should not include the .xlsx extension, as it will be added automatically.
```
--default, -d
```
Use default configuration without prompting. No need to specify json file.\
If no configuration options are provided, the program will prompt the user whether to use the default configuration or not.\
If both name options and configuration options are provided, the program will use the specified name and configuration file.

### All possible combinations:
Print the help message and exit:
```
./RaceXL --help
./RaceXL -h
```

Use custom output filename with default configuration (will prompt whether to use default configuration or not):
```
./RaceXL --name <output_filename>
./RaceXL -n <output_filename>
```

Use default configuration (will prompt whether to use default configuration or not) with default output filename 'race-to-the-technion-scores':
```
./RaceXL
```

Use default configuration without prompting:
```
./RaceXL --default
./RaceXL -d
```

Use custom configuration file with default output filename 'race-to-the-technion-scores':
```
./RaceXL <json_filename>
```

Use custom output filename with default configuration without prompting:
```
./RaceXL --name <output_filename> --default
./RaceXL -n <output_filename> --default
./RaceXL --name <output_filename> -d
./RaceXL -n <output_filename> -d
```

Use custom output filename with custom configuration file:
```
./RaceXL --name <output_filename> <json_filename>
./RaceXL -n <output_filename> <json_filename>
```

## Example JSON Configuration:
```
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
```

