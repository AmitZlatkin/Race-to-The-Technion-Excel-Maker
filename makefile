# Define variables
COMPILER_V = g++
# win:
# COMPILER_V = x86_64-w64-mingw32-g++

FLAGS = -std=c++17 -g
# win:
# FLAGS = -std=c++17 -g -static-libstdc++

TARGET = RaceXL
# win:
# TARGET = RaceXL.exe


SOURCE_DIR = libXL

BUILD_DIR = build
# win:
# BUILD_DIR = build-Win


LibXL_FILE_LIST = XLCell.cpp XLCellIterator.cpp XLCellRange.cpp XLCellReference.cpp XLCellValue.cpp XLColor.cpp XLColumn.cpp XLComments.cpp XLContentTypes.cpp XLDateTime.cpp XLDocument.cpp XLDrawing.cpp XLFormula.cpp XLMergeCells.cpp XLProperties.cpp XLRelationships.cpp XLRow.cpp XLRowData.cpp XLSharedStrings.cpp XLSheet.cpp XLStyles.cpp XLTables.cpp XLWorkbook.cpp XLXmlData.cpp XLXmlFile.cpp XLXmlParser.cpp XLZipArchive.cpp

# SRCS = $(addprefix $(SOURCE_DIR)/, $(LibXL_FILE_LIST)) main.cpp RaceActivity.cpp
# # win:
# # SRCS = $(addprefix $(SOURCE_DIR)/, $(LibXL_FILE_LIST)) main-win.cpp RaceActivity.cpp

OBJS = $(addprefix $(BUILD_DIR)/, $(LibXL_FILE_LIST:.cpp=.o)) $(addprefix $(BUILD_DIR)/, main.o RaceActivity.o jsonParser.o)
# win:
# OBJS = $(addprefix $(BUILD_DIR)/, $(LibXL_FILE_LIST:.cpp=.o)) $(addprefix $(BUILD_DIR)/, main-win.o RaceActivity.o jsonParser.o) resources.res

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	echo "\nBuild started...\n"
	$(COMPILER_V) $(FLAGS) -o $@ $^
	echo "\nBuild complete: $(TARGET)\n"

$(BUILD_DIR)/main.o: main.cpp
	$(COMPILER_V) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/main-win.o: main-win.cpp
	$(COMPILER_V) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/RaceActivity.o: RaceActivity.cpp
	$(COMPILER_V) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/jsonParser.o: jsonParser.cpp
	$(COMPILER_V) $(FLAGS) -c $< -o $@

# Compile source files into object files using a pattern rule
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(COMPILER_V) $(FLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(TARGET) $(OBJS)

# Declare phony targets
.PHONY: all clean