
#include "functions.h"
#include <ctime>

// Windows .exe functions:

void custom_exit(int ret, bool quitCMD) {
    if (!quitCMD) {
        string useless_str;
        printLine("Press Enter to quit...");
        std::cin.ignore(); // Waits for Enter (ignores one line of input)
    }
    exit(ret);
}


std::vector<string> splitString(const string& str) {
    std::vector<std::string> words;
    std::istringstream stream(str);
    std::string word;

    while (stream >> word) {
        words.push_back(word);
    }

    return words;
}


stringsPair readUserInput_WIN(int argc, std::vector<string> argv, bool quitCMD) {

    bool flag_default = false;

    std::pair<string, string> inputResult = checkArguments_WIN(argc, argv, flag_default, quitCMD);
    string outputFilename = inputResult.first;    
    string configFilename = inputResult.second;

    printLine("\nReading json configuration file...");
    string jsonString;
    
    if (configFilename != "") {
        jsonString = getJsonString(configFilename);
        printLine("Race was successfully configured based on json file.\n");
    } else {

        if (flag_default) {
            printLine("Using the default configuration...");
            jsonString = getDefaultJsonString();
            printLine("Race was successfully configured based on default configuration.\n");
            return {jsonString, outputFilename};
        }

        printLine("No json file was given...");
        printLine("Do you want to use the default configuration? Y/n\n");
        
        char userInput;
        std::cin >> userInput;
        
        if (userInput == 'Y' || userInput == 'y') {
            printLine("\nUsing the default configuration...");
            jsonString = getDefaultJsonString();
            printLine("Race was successfully configured based on default configuration.\n");
        }
        else if (userInput == 'N' || userInput == 'n') {
            printLine("\nUser has chosen to not use default configuration.");
            printLine("Quitting execution.\n");
            custom_exit(0);
        }
        else {
            printLine("\nInvalid key was pressed, quitting execution.\n");
            custom_exit(1);
        }
    }
    return {jsonString, outputFilename};
}


int findFlag_WIN(int argc, std::vector<string> argv, const string& flag_v1, const string& flag_v2) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == flag_v1 || argv[i] == flag_v2) {
            return i;
        }
    }
    return -1;
}


stringsPair checkArguments_WIN(int argc, std::vector<string> argv, bool& flag_default, bool quitCMD) {
    int help_index = findFlag_WIN(argc, argv, "-h", "--help");
    int name_index = findFlag_WIN(argc, argv, "-n", "--name");
    int default_index = findFlag_WIN(argc, argv, "-d", "--default");

    if (help_index != -1) {
        if (help_index != 1) {
            printLine("\nError: The help option must be the first flag (if provided).");
            printLine("Use --help or -h by itself to see full usage instructions.\n");
            custom_exit(1, quitCMD);
        }
        printHelpMessage(argv[0]);
        custom_exit(0, quitCMD);
    }

    // correct order of flags: help, name, output filename, default, json filename
    
    bool correct_order = true;
    if (name_index != -1 && name_index <= help_index) correct_order = false;
    if (default_index != -1 && default_index <= name_index) correct_order = false;
    if (!correct_order) {
        printLine("\nError: Incorrect order of flags.");
        printLine("Use --help or -h by itself to see full usage instructions.\n");
        custom_exit(1, quitCMD);
    }

    string outputFilename = defaultOutputFilename;
    string configFilename = "";
    if (name_index != -1) {
        if ((name_index + 1 >= argc) || (name_index + 1 >= default_index && default_index != -1)) {
            printLine("\nError: No output filename provided after --name or -n option.");
            printLine("Use --help or -h by itself to see full usage instructions.\n");
            custom_exit(1, quitCMD);
        }
        outputFilename = string(argv[name_index + 1]);
    }

    if (default_index != -1) {
        flag_default = true;
    } else {
        if (argc > name_index + 2) {
            configFilename = argv[argc - 1];
        }
    }
    return {outputFilename, configFilename};
}

// end of .exe functions


// terminal functions:

stringsPair readUserInput(int argc, char** argv) {

    bool flag_default = false;

    std::pair<string, string> inputResult = checkArguments(argc, argv, flag_default);
    string outputFilename = inputResult.first;    
    string configFilename = inputResult.second;

    printLine("\nReading json configuration file...");
    string jsonString;
    
    if (configFilename != "") {
        jsonString = getJsonString(configFilename);
        printLine("Race was successfully configured based on json file.\n");
    } else {

        if (flag_default) {
            printLine("Using the default configuration...");
            jsonString = getDefaultJsonString();
            printLine("Race was successfully configured based on default configuration.\n");
            return {jsonString, outputFilename};
        }

        printLine("No json file was given...");
        printLine("Do you want to use the default configuration? Y/n\n");
        
        char userInput;
        std::cin >> userInput;
        
        if (userInput == 'Y' || userInput == 'y') {
            printLine("\nUsing the default configuration...");
            jsonString = getDefaultJsonString();
            printLine("Race was successfully configured based on default configuration.\n");
        }
        else if (userInput == 'N' || userInput == 'n') {
            printLine("\nUser has chosen to not use default configuration.");
            printLine("Quitting execution.\n");
            exit(0);
        }
        else {
            printLine("\nInvalid key was pressed, quitting execution.\n");
            exit(1);
        }
    }
    return {jsonString, outputFilename};
}


int findFlag(int argc, char** argv, const string& flag_v1, const string& flag_v2) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == flag_v1 || argv[i] == flag_v2) {
            return i;
        }
    }
    return -1;
}


stringsPair checkArguments(int argc, char** argv, bool& flag_default) {
    int help_index = findFlag(argc, argv, "-h", "--help");
    int name_index = findFlag(argc, argv, "-n", "--name");
    int default_index = findFlag(argc, argv, "-d", "--default");

    if (help_index != -1) {
        if (help_index != 1) {
            printLine("\nError: The help option must be the first flag (if provided).");
            printLine("Use --help or -h by itself to see full usage instructions.\n");
            exit(1);
        }
        printHelpMessage(argv[0]);
        exit(0);
    }

    // correct order of flags: help, name, output filename, default, json filename
    
    bool correct_order = true;
    if (name_index != -1 && name_index <= help_index) correct_order = false;
    if (default_index != -1 && default_index <= name_index) correct_order = false;
    if (!correct_order) {
        printLine("\nError: Incorrect order of flags.");
        printLine("Use --help or -h by itself to see full usage instructions.\n");
        exit(1);
    }

    string outputFilename = defaultOutputFilename;
    string configFilename = "";
    if (name_index != -1) {
        if ((name_index + 1 >= argc) || (name_index + 1 >= default_index && default_index != -1)) {
            printLine("\nError: No output filename provided after --name or -n option.");
            printLine("Use --help or -h by itself to see full usage instructions.\n");
            exit(1);
        }
        outputFilename = string(argv[name_index + 1]);
    }

    if (default_index != -1) {
        flag_default = true;
    } else {
        if (argc > name_index + 2) {
            configFilename = argv[argc - 1];
        }
    }
    return {outputFilename, configFilename};
}

// end of terminal functions


void printLine(const string& str, char end)  {
    cout << str << end;
}


void printHelpMessage(const string& progName) {

    printLine();
    printLine("    Race to The Technion - Excel Score Sheet Generator");
    printLine("============================================================");
    printLine();
    printLine("This program generates an Excel score sheet for the yearly Race to The Technion based on a provided JSON configuration file.");
    printLine("If no configuration file is provided, the program can use a default configuration.");
    printLine("The output Excel file will be named 'race_to_the_technion_scores.xlsx' by default, but a custom name can be specified.");
    printLine();
    printLine("The resulting Excel file will contain a 'Scores' sheet and additional sheets for each activity as specified in the configuration.");
    printLine("The 'Scores' sheet will automatically calculate the total scores for each team based on their performance in the activities.");
    printLine("Activity sheets will include unformatted checkboxes or numeric input fields for score entry, depending on the activity type.");
    printLine("Activity sheets will be named according to the activity names provided in the configuration.");
    printLine("If an activity has a sheet, a formula will be added to the 'Scores' sheet to calculate points based on the activity's scoring rules.");
    printLine();
    printLine("Configuration File Format:");
    printLine("The configuration file should be in JSON format and include the following fields:");
    printLine("  - teams: An integer representing the number of teams participating in the race.");
    printLine("  - activities: An array of activity objects, each containing:");
    printLine("      - name: A string representing the name of the activity.");
    printLine("      - location: A string representing the location of the activity.");
    printLine("      - makeWorksheet: A boolean indicating whether to create a separate worksheet for the activity (true) or not (false).");
    printLine("      - rows: An integer representing the number of rows (sub-tasks or checkpoints) in the activity.");
    printLine("      - numberTable: A boolean indicating whether the activity uses a numeric input table (true) or checkboxes (false).");
    printLine("      - pointsPerRow: An integer representing the points awarded for each completed row. Relevant only if numberTable is false.");
    printLine();
    printLine("Example JSON Configuration:");
    printLine(getDefaultJsonString());
    printLine();
    printLine("Usage:");
    printLine();
    printLine("    " + string(progName) + " [help option] [name options] [configuration options]");
    printLine();
    printLine("Options:");
    printLine("  --name, -n");
    printLine("        Use custom output filename.");
    printLine("        This option must be followed by the desired output filename.");
    printLine("        If no name options are provided, the output file will be named 'race_to_the_technion.xlsx'.");
    printLine("        Note: The output filename should not include the .xlsx extension, as it will be added automatically.");
    printLine();
    printLine("  --default, -d");
    printLine("        Use default configuration without prompting.");
    printLine("        No need to specify json file.");
    printLine();
    printLine("  If no configuration options are provided, the program will prompt the user whether to use the default configuration or not.");
    printLine("  If both name options and configuration options are provided, the program will use the specified name and configuration file.");
    printLine();
    printLine("  --help, -h");
    printLine("        Show this help message and exit.");
    printLine("        This option must be the first flag if provided.");
    printLine();
    printLine("All possible combinations:");
    printLine("    " + progName + " --help");
    printLine("    " + progName + " -h");
    printLine("        Print this help message and exit.");
    printLine();
    printLine("    " + progName + " --name <output_filename>");
    printLine("    " + progName + " -n <output_filename>");
    printLine("        Use custom output filename with default configuration (will prompt whether to use default configuration or not).");
    printLine();
    printLine("    " + progName + "");
    printLine("        Use default configuration (will prompt whether to use default configuration or not) with default output filename '" + defaultOutputFilename + "'.");
    printLine();
    printLine("    " + progName + " --default");
    printLine("    " + progName + " -d");
    printLine("        Use default configuration without prompting.");
    printLine();
    printLine("    " + progName + " <json_filename>");
    printLine("        Use custom configuration file with default output filename '" + defaultOutputFilename + "'.");
    printLine();
    printLine("    " + progName + " --name <output_filename> --default");
    printLine("    " + progName + " -n <output_filename> --default");
    printLine("    " + progName + " --name <output_filename> -d");
    printLine("    " + progName + " -n <output_filename> -d");
    printLine("        Use custom output filename with default configuration without prompting.");
    printLine();
    printLine("    " + progName + " --name <output_filename> <json_filename>");
    printLine("    " + progName + " -n <output_filename> <json_filename>");
    printLine("        Use custom output filename with custom configuration file.");
    printLine();
    printLine("Created by Amit Zlatkin, graduate of the 11th class of Odyssey at the techion, for the use of future generations.");
    printLine("For any issues, please contact me at amit.zlatkin@campus.technion.ac.il");
    printLine();
}


void cleanWorkbook(XLWorkbook& wb, const string& outputFilename) {
    printLine("Cleaning '" + makeHebrewReadable(outputFilename) + ".xlsx" + "' Excel Document...");
    int sheets = wb.sheetCount();
    for (int i = 1; i < sheets; ++i) {
        wb.deleteSheet(wb.sheet(1).name());
    }
    wb.worksheet(1).setName("ניקוד");
    printLine("'" + makeHebrewReadable(outputFilename) + ".xlsx" + "' Excel Document Cleaned\n");
}


void initWorksheet(const XLWorksheet& wks, int teams, int columnShift) {
    for (int col = 2; col <= teams+1; ++col) {
        wks.cell(1, col + columnShift).value() = "קבוצה " + std::to_string(col-1);
    }
}


void setDeltaTimeFormulas(XLDocument& doc, XLWorksheet& wks, int teams) {
    
    int colShift = teams + 3;
    initWorksheet(wks, teams, colShift);

    auto timesHeader    = getRange(wks, wks.cell(1, colShift+1), wks.cell(1, colShift+1+teams));
    auto startTimesRow  = getRange(wks, wks.cell(2, colShift+1), wks.cell(2, colShift+1+teams));
    auto finishTimesRow = getRange(wks, wks.cell(3, colShift+1), wks.cell(3, colShift+1+teams));
    auto deltaTimesRow  = getRange(wks, wks.cell(4, colShift+1), wks.cell(4, colShift+1+teams));

    styleRow(doc, timesHeader,    white, true,  dark_green, true);
    styleRow(doc, startTimesRow,  black, false, white,      false);
    styleRow(doc, finishTimesRow, black, false, white,      false);
    styleRow(doc, deltaTimesRow,  black, false, light_grey, true);

    wks.cell(1, colShift+1).value() = "זמנים";
    wks.cell(2, colShift+1).value() = "זמן התחלה";
    wks.cell(3, colShift+1).value() = "זמן סיום";
    wks.cell(4, colShift+1).value() = "הפרש";

    auto begin = ++deltaTimesRow.begin(); // skip first cell
    auto end = deltaTimesRow.end();
    for (auto cell = begin; cell != end; ++cell) {
        auto cell_ref = cell->cellReference();
        std::stringstream formula;
        string col = cell_ref.columnAsString(cell_ref.column());
        formula << col << "3-" << col << "2";
        cell->formula() = formula.str();
    }

    std::tm tm;
    tm.tm_year = 2025 - 1900; // Years since 1900
    tm.tm_mon = 0; // January (0-based, so 0 means January)
    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    XLDateTime dt (tm);
    
    auto dateTimeCells = getRange(wks, wks.cell(2, colShift+2), wks.cell(3, colShift+1+teams));;
    for (auto cell : dateTimeCells) {
        cell.value() = dt;
    }
}


void initScoresWorksheet(XLDocument& doc, XLWorksheet& wks, int teams, int activities) {

    printLine("Initializing Scores Sheet...");

    initWorksheet(wks, teams, 1);

    wks.cell("A1").value() = "תחנה";
    wks.cell("B1").value() = "מיקום";
    XLCellRange topRow = getRange(wks, wks.cell("A1"), wks.cell(1, teams+2));
    styleRow(doc, topRow, white, true, dark_green, true);
    
    XLCellRange row;
    for(int row_num = 2; row_num <= activities+2; row_num++) {
        row = getRange(wks, wks.cell(row_num, 1), wks.cell(row_num, teams+2));
        styleRow(doc, row , black , false, (row_num % 2 == 0) ? white : light_grey, (row_num == activities+2));
    }
    
    XLCell scoresFirstCell = *(row.begin());
    XLCell scoresSecondCell = *(++row.begin());
    
    scoresFirstCell.value() = "סך הכל";
    
    XLCellFormats& cellFormats = doc.styles().cellFormats();
    XLStyleIndex mergedCellFormat = cellFormats.create( cellFormats[ scoresFirstCell.cellFormat() ] );

	cellFormats[ mergedCellFormat ].alignment(XLCreateIfMissing).setHorizontal(XLAlignmentStyle::XLAlignCenterContinuous);
	cellFormats[ mergedCellFormat ].alignment(XLCreateIfMissing).setVertical(XLAlignmentStyle::XLAlignCenterContinuous);

    XLCellRange rangeToMerge = getRange(wks, scoresFirstCell, scoresSecondCell);
    wks.mergeCells(rangeToMerge); // merge cells without deletion of contents
    scoresFirstCell.setCellFormat(mergedCellFormat);

    setDeltaTimeFormulas(doc, wks, teams);

    printLine("Scores Sheet Initialized");
}


void initActivityWorksheet(const XLWorksheet& wks, int teams, int rows, bool numberTable) {
    const string hebrewReadable = makeHebrewReadable(wks.name());
    printLine("Initializing Activity '" + hebrewReadable + "' Sheet...");
    initWorksheet(wks, teams);
    XLCellReference checkboxRangeTopLeft = wks.cell(2, 2).cellReference();
    XLCellReference checkboxRangeBottomRight = wks.cell(rows + 1, teams + 1).cellReference();
    for (auto cellReference : wks.range(checkboxRangeTopLeft, checkboxRangeBottomRight)) {
        if (!numberTable) {
            cellReference.value() = false; // Initialize all checkboxes to false (unchecked)
        }
    }
    printLine("Activity '" + hebrewReadable + "' Sheet Initialized");
}


void setTotalFormulas(XLWorksheet& scores_wks, int teams, int row_num) {
    printLine("Setting Scores Formulas...");
    for (char col = 'C'; col <= 'A' + teams + 1; ++col) {
        std::stringstream formula;
        formula << "SUM(" << col << "2:" << col << std::to_string(row_num-1) << ")";
        scores_wks.cell(row_num, col-'A'+1).formula() = formula.str();
    }
    printLine("Scores Formulas Are Set\n");
}


void setActivityFormulas(XLWorksheet& scores_wks, int teams, int formula_row_num, const RaceActivity& activity) {
    const string hebrewReadable = makeHebrewReadable(activity.m_name);
    printLine("Setting Activity '" + hebrewReadable + "' Formulas...");
    for (char col = 'B'; col <= 'A' + teams; ++col) {
        std::stringstream formula;
        if (!activity.m_numberTable) {
            // activity.m_pointsPerRow*COUNTIF('activity.m_name'!(col)(2):(col)(activity.m_rows+1), TRUE())
            formula << activity.m_pointsPerRow << "*COUNTIF('" << activity.m_name << "'!" << col << "2:" << col << std::to_string(activity.m_rows+1) << ", TRUE())";
        } else {
            // SUM('activity.m_name'!(col)(2):(col)(activity.m_rows+1))
            formula << "SUM('" << activity.m_name << "'!" << col << "2:" << col << std::to_string(activity.m_rows+1) << ")";
        }
        scores_wks.cell(formula_row_num, col-'A'+2).formula() = formula.str();
    }    
    printLine("Activity '" + hebrewReadable + "' Formulas Are Set\n");
}


XLCellRange getRange(const XLWorksheet& wks, const XLCell& topLeft, const XLCell& bottomRight) {
    return wks.range(topLeft.cellReference(), bottomRight.cellReference());
}


void styleRow(XLDocument& doc, XLCellRange& row, const XLColor& fontColor, bool makeBold, const XLColor& fillColor, bool horizontalBorders) {
    XLCellFormats& cellFormats = doc.styles().cellFormats();
    XLFonts& fonts = doc.styles().fonts();
    XLFills& fills = doc.styles().fills();
    XLBorders& borders = doc.styles().borders();
    
    XLStyleIndex baseStyle = row.begin()->cellFormat();
    XLStyleIndex newStyle = cellFormats.create(         cellFormats[ baseStyle ] );
    
    XLStyleIndex newFontStyle =   fonts.create(fonts[ cellFormats[ baseStyle ].fontIndex() ]);
    XLStyleIndex newFillStyle =   fills.create(fills[ cellFormats[ baseStyle ].fillIndex() ]);
    XLStyleIndex newBorderStyle = borders.create(borders[ cellFormats[ baseStyle ].borderIndex() ]);;
    
    
    fonts[ newFontStyle ].setFontColor(fontColor);
    if (makeBold) {
        fonts[ newFontStyle ].setBold();
    }
    
    fills[ newFillStyle ].setPatternType(XLPatternType::XLPatternSolid);
    fills[ newFillStyle ].setColor(fillColor);
    
    borders[ newBorderStyle ].setRight( XLLineStyle::XLLineStyleThin, black );
    borders[ newBorderStyle ].setLeft ( XLLineStyle::XLLineStyleThin, black );
    
    if(horizontalBorders) {
        borders[ newBorderStyle ].setTop   ( XLLineStyle::XLLineStyleThin, black );
        borders[ newBorderStyle ].setBottom( XLLineStyle::XLLineStyleThin, black );
    }

    cellFormats[ newStyle ].setFontIndex( newFontStyle );
    cellFormats[ newStyle ].setFillIndex( newFillStyle );
    cellFormats[ newStyle ].setBorderIndex( newBorderStyle );
    
    row.setFormat(newStyle);
}

