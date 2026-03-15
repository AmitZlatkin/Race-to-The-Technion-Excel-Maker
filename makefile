# Define variables
SOURCE_DIR = libXL
LibXL_FILE_LIST = XLCell.cpp XLCellIterator.cpp XLCellRange.cpp XLCellReference.cpp XLCellValue.cpp XLColor.cpp XLColumn.cpp XLComments.cpp XLContentTypes.cpp XLDateTime.cpp XLDocument.cpp XLDrawing.cpp XLFormula.cpp XLMergeCells.cpp XLProperties.cpp XLRelationships.cpp XLRow.cpp XLRowData.cpp XLSharedStrings.cpp XLSheet.cpp XLStyles.cpp XLTables.cpp XLWorkbook.cpp XLXmlData.cpp XLXmlFile.cpp XLXmlParser.cpp XLZipArchive.cpp

# Linux:
COMPILER_V = g++
FLAGS = -std=c++17 -g
TARGET = RaceXL
BUILD_DIR = build
OBJS = $(addprefix $(BUILD_DIR)/, $(LibXL_FILE_LIST:.cpp=.o)) $(addprefix $(BUILD_DIR)/, main.o RaceActivity.o JsonParser.o functions.o)

# Windows:
COMPILER_V_WIN = x86_64-w64-mingw32-g++
FLAGS_WIN = -std=c++17 -g -static-libstdc++ -static-libgcc
TARGET_WIN = RaceXL.exe
BUILD_DIR_WIN = build-Win
OBJS_WIN = $(addprefix $(BUILD_DIR_WIN)/, $(LibXL_FILE_LIST:.cpp=.o)) $(addprefix $(BUILD_DIR_WIN)/, main-win.o RaceActivity.o JsonParser.o functions.o) resources.res


# Linux:

# Default target
all: $(TARGET) $(TARGET_WIN)

# Link object files to create the executable
$(TARGET): $(OBJS)
	echo "\nBuild started...\n"
	$(COMPILER_V) $(FLAGS) -o $@ $^
	echo "\nBuild complete: $(TARGET)\n"

$(BUILD_DIR)/main.o: main.cpp
	$(COMPILER_V) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/functions.o: functions.cpp
	$(COMPILER_V) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/RaceActivity.o: RaceActivity.cpp
	$(COMPILER_V) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/JsonParser.o: JsonParser.cpp
	$(COMPILER_V) $(FLAGS) -c $< -o $@

# Compile source files into object files using a pattern rule
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(COMPILER_V) $(FLAGS) -c $< -o $@


# Windows:

$(TARGET_WIN): $(OBJS_WIN)
	echo "\nBuild started...\n"
	$(COMPILER_V_WIN) $(FLAGS_WIN) -o $@ $^
	echo "\nBuild complete: $(TARGET_WIN)\n"

$(BUILD_DIR_WIN)/main-win.o: main-win.cpp
	$(COMPILER_V_WIN) $(FLAGS_WIN) -c $< -o $@

$(BUILD_DIR_WIN)/functions.o: functions.cpp
	$(COMPILER_V_WIN) $(FLAGS_WIN) -c $< -o $@

$(BUILD_DIR_WIN)/RaceActivity.o: RaceActivity.cpp
	$(COMPILER_V_WIN) $(FLAGS_WIN) -c $< -o $@

$(BUILD_DIR_WIN)/JsonParser.o: JsonParser.cpp
	$(COMPILER_V_WIN) $(FLAGS_WIN) -c $< -o $@

# Compile source files into object files using a pattern rule
$(BUILD_DIR_WIN)/%.o: $(SOURCE_DIR)/%.cpp
	$(COMPILER_V_WIN) $(FLAGS_WIN) -c $< -o $@



# Clean up generated files
clean:
	rm -f $(TARGET) $(OBJS)

# Declare phony targets
.PHONY: all clean