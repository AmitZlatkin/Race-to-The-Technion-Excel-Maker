
# Race to The Technion - Excel Score Sheet Generator


<div align="center">
<img src="AppIcon.ico" alt="RaceXL App Icon" width="150">
</div>

\
This in an app that was created by Amit Zlatkin, a graduate of the 11th class of Odyssey at the Technion, for the use of future generations.\
For any issues, contact me at [amit.zlatkin@campus.technion.ac.il](amit.zlatkin@campus.technion.ac.il).\
This [repository](https://github.com/AmitZlatkin/Race-to-The-Technion-Excel-Maker) includes all information about the app, with the most up-to-date version available under [Releases](https://github.com/AmitZlatkin/Race-to-The-Technion-Excel-Maker/releases) to be downloaded directly.\
This project incorporates the OpenXLSX library, more on that in the [`LICENSE.md`](./LICENSE.md#third-party-software-notices) file, and at the [Credits](#credits) and [License](#license) sections at the end.

---

## Table of Contents:
- [Intro](#intro)
- [Usage](#usage)
    - [On Linux](#on-linux)
    - [On Windows](#on-windows)
    - [Options](#options)
    - [All Possible Commands](#all-possible-commands)
- [Configuration File Format](#configuration-file-format)
- [Example JSON Configuration](#example-json-configuration)
- [Credits](#credits)
- [License](#license)

---

## Intro:
This program generates an Excel score sheet for the yearly Race to the Technion based on a provided JSON configuration file.

* The `.json` file extension can be omitted, it will be added automatically if missing.

* If no configuration file is provided, the program can use a default configuration.

* The output Excel file will be named `race-to-the-technion.xlsx` by default, but a custom name can be specified.

The resulting Excel file will contain a Scores sheet and additional sheets for each activity as specified in the configuration.

* The scores sheet will automatically calculate the total scores for each team based on their performance in the activities, and the time that took them to complete the race.

* Activity sheets will include unformatted checkboxes or numeric input fields for score entry, depending on the activity type, and are named according to the activity names provided in the configuration.

* If an activity has a sheet, a formula will be added to the scores sheet to calculate points based on the activity's scoring rules.

**The program is available in two executable files:**
* [**Linux based**](https://github.com/AmitZlatkin/Race-to-The-Technion-Excel-Maker/blob/main/RaceXL) - Tested on Ubuntu-24.04 and on Kali-linux with Wsl2.
* [**Windows based (.exe)**](https://github.com/AmitZlatkin/Race-to-The-Technion-Excel-Maker/blob/main/RaceXL.exe) - Tested on Windows 11.

---

## Usage:

### General Usage:

The app utilizes shell arguments that are passed in Unix notation.\
As such, if the desired output filename or the JSON filename have spaces in them, they must be surrounded by single or double quotes ( `'...'` or `"..."` ).

### On Linux:
The following command can be used to launch the app from a terminal:
```
./RaceXL [help] [name] [configuration]
```
### On Windows:
On Windows, there are two ways of using the app:
1. By clicking on the app icon (the terminal will open automatically).\
Then the arguments can be typed directly, no need to type the program name!

2. Via the various windows terminals (cmd, powershell, and more), using a similar command to Linux.

For a better experience on windows, it is recommended to use the app icon directly and not launch from the terminal.
### Options:
#### Help flag:
```
--help, -h
```
Print the help message and exit the program.
This flag must be the first flag.
#### Naming option:
```
--name, -n
```
Use a custom output filename.\
This flag must be followed by the desired output filename.\
If no name options are provided, the output file will be named `race-to-the-technion.xlsx`.
 - Note 1: The output filename doesn't have to include the `.xlsx` extension, it will be added automatically if omitted.

 - Note 2: The output filename can be a path, e.g. `folder/scores sheet`.\
 This path will be treated as a **Relative** path to the current directory.\
 The path should be specified in Unix notation, using **ONLY Forward Slash '/'**.\
 If some of the directories in the path don't exists, they will be created.

#### Configuration options:
As stated, a configuration JSON file can be used, and this is the exact place to put it.\
The `.json` file extension can be omitted, it will be added automatically if missing.

Additionally, there is the default flag:
```
--default, -d
```
Which allows the use of the default configuration without prompting and without the need to specify a JSON file.

#### More info
If no configuration options are provided, the program prompts the user whether to use the default configuration or not.

If both naming and configuration options are provided, the program will use the specified name and the specified configuration file.

---

### All possible commands:
#### Print the help message and exit:
```
./RaceXL --help
./RaceXL -h
```

#### Use default configuration (will prompt whether to use default configuration or not) with default output filename:
```
./RaceXL
```

#### Use default configuration without prompting with default output filename:
```
./RaceXL --default
./RaceXL -d
```

#### Use custom configuration file with default output filename:
```
./RaceXL json-filename
```

#### Use custom output filename with default configuration (will prompt whether to use default configuration or not):
```
./RaceXL --name output-filename
./RaceXL -n output-filename
```

#### Use custom output filename with default configuration without prompting:
```
./RaceXL --name output-filename --default
./RaceXL -n output-filename --default
./RaceXL --name output-filename -d
./RaceXL -n output-filename -d
```

#### Use custom output filename with custom configuration file:
```
./RaceXL --name output-filename json-filename
./RaceXL -n output-filename json-filename
```

---


## Configuration File Format:
The configuration file should be in JSON format and include the following fields:
  - `teams`: An integer representing the number of teams participating in the race.
  - `activities`: An array of `activity` objects.

Each `activity` object in the mentioned list has the following fields:
 - `name`: A string representing the name of the activity.
 - `location`: A string representing the location of the activity.
 - `makeWorksheet`: A boolean indicating whether to create a separate worksheet for the activity (true) or not (false).
 - `rows`: An integer representing the number of rows (sub-tasks or checkpoints) in the activity.
 - `numberTable`: A boolean indicating whether the activity uses a numeric input table (true) or checkboxes (false).
 - `pointsPerRow`: An integer representing the points awarded for each completed row.\
 Relevant only if numberTable is false.

You can see an example JSON configuration file below.



## Example JSON Configuration:
In the file [`Example Race Layout.docx`](./Example%20Race%20Layout.docx) is an example of a possible race layout with 3 various activities.

Below is the JSON configuration that is associated with that race layout.

**Note:** The configuration JSON file can contain hebrew activity names and locations as well, as shown in [`tests/2025 race config.json`](./tests/2025%20race%20config.json).

```json
{
    "teams": 3,
    "activities": [
        {
            "name": "People Quiz",
            "location": "The Odyssey Office",
            "makeWorksheet": true,
            "rows": 6,
            "numberTable": false,
            "pointsPerRow": 2
        },
        {
            "name": "Song Completion",
            "location": "The Main Library",
            "makeWorksheet": true,
            "rows": 5,
            "numberTable": false,
            "pointsPerRow": 1
        },
        {
            "name": "Human Pyramid",
            "location": "ASAT Grass",
            "makeWorksheet": false,
            "rows": 0,
            "numberTable": false,
            "pointsPerRow": 0
        }
    ]
}
```

---

## Credits

This project was made possible thanks to the following open-source software:

**OpenXLSX** - an open-source C++ library for reading, writing, creating and modifying Microsoft Excel® files, with the .xlsx format.\
Source: https://github.com/troldal/OpenXLSX.\
Distributed under the BSD 3-Clause License. Full details available in [`LICENSE.md`](./LICENSE.md#third-party-software-notices).

---

## License

Copyright (c) 2026, Amit Zlatkin,
All rights reserved.

Distributed under the BSD 3-Clause License. Full details available in [`LICENSE.md`](./LICENSE.md).