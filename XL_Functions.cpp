#include "XL_Functions.h"
#include "UtilsFunctions.h"


void XL_Functions::cleanWorkbook(XLWorkbook& wb, const string& outputFilename) {
    printLine("Cleaning '" + makeHebrewReadable(outputFilename) + ".xlsx" + "' Excel Document...");
    int sheets = wb.sheetCount();
    for (int i = 1; i < sheets; ++i) {
        wb.deleteSheet(wb.sheet(1).name());
    }
    wb.worksheet(1).setName("ניקוד");
    printLine("'" + makeHebrewReadable(outputFilename) + ".xlsx" + "' Excel Document Cleaned\n");
}


void XL_Functions::initWorksheet(const XLWorksheet& wks, int teams, int columnShift) {
    for (int col = 2; col <= teams+1; ++col) {
        wks.cell(1, col + columnShift).value() = "קבוצה " + std::to_string(col-1);
    }
}


void XL_Functions::setDeltaTimeFormulas(XLDocument& doc, XLWorksheet& wks, int teams) {
    
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

    auto dateTimeCells = getRange(wks, wks.cell(2, colShift+2), wks.cell(3, colShift+1+teams));
    for (auto cell : dateTimeCells) {
        cell.value() = dt;
    }
}


void XL_Functions::initScoresWorksheet(XLDocument& doc, XLWorksheet& wks, int teams, int activities) {

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


void XL_Functions::initActivityWorksheet(const XLWorksheet& wks, int teams, int rows, bool numberTable) {
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


void XL_Functions::setTotalFormulas(XLWorksheet& scores_wks, int teams, int row_num) {
    printLine("Setting Scores Formulas...");
    for (char col = 'C'; col <= 'A' + teams + 1; ++col) {
        std::stringstream formula;
        formula << "SUM(" << col << "2:" << col << std::to_string(row_num-1) << ")";
        scores_wks.cell(row_num, col-'A'+1).formula() = formula.str();
    }
    printLine("Scores Formulas Are Set\n");
}


void XL_Functions::setActivityFormulas(XLWorksheet& scores_wks, int teams, int formula_row_num, const RaceActivity& activity) {
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


XLCellRange XL_Functions::getRange(const XLWorksheet& wks, const XLCell& topLeft, const XLCell& bottomRight) {
    return wks.range(topLeft.cellReference(), bottomRight.cellReference());
}


void XL_Functions::styleRow(XLDocument& doc, XLCellRange& row, const XLColor& fontColor, bool makeBold, const XLColor& fillColor, bool horizontalBorders) {
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