CXX = g++
CXX_FLAGS = -Wall -pedantic -Wextra -std=c++17 -g

MKDIR = mkdir -p
BUILD_DIR = build
USERNAME = bohacmi6
HELPER_DIRS = $(BUILD_DIR)/$(USERNAME)/src/diff
LINKER_LIBS = -lstdc++fs

OUTPUT_FILENAME = bohacmi6

.PHONY: all
all: clean compile doc

.PHONY: compile
compile: $(BUILD_DIR)

$(BUILD_DIR): $(BUILD_DIR)/$(USERNAME)/src/main.o $(BUILD_DIR)/$(USERNAME)/src/diff/ArgParser.o $(BUILD_DIR)/$(USERNAME)/src/diff/DiffObj.o $(BUILD_DIR)/$(USERNAME)/src/diff/ImageObj.o $(BUILD_DIR)/$(USERNAME)/src/diff/FolderObj.o $(BUILD_DIR)/$(USERNAME)/src/diff/TextObj.o $(BUILD_DIR)/$(USERNAME)/src/diff/BinObj.o
	$(CXX) $(CXX_FLAGS) $^ -o ./$(OUTPUT_FILENAME) $(LINKER_LIBS)

$(BUILD_DIR)/$(USERNAME)/%.o:
	$(MKDIR) $(BUILD_DIR)/$(USERNAME) $(HELPER_DIRS)
	$(CXX) $(CXX_FLAGS) $< -c -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(OUTPUT_FILENAME) ./doc 2>/dev/null

.PHONY: doc
doc:
	doxygen doxygen.conf

.PHONY: run
run:
	./$(OUTPUT_FILENAME)

# g++ -MM src/main.cpp
$(BUILD_DIR)/$(USERNAME)/src/main.o: src/main.cpp src/diff/ArgParser.h src/diff/constants/constants.h src/diff/DiffObj.h src/diff/ArgParser.h src/diff/constants/constants.h
# g++ -MM src/diff/ArgParser.cpp
$(BUILD_DIR)/$(USERNAME)/src/diff/ArgParser.o: src/diff/ArgParser.cpp src/diff/ArgParser.h src/diff/constants/constants.h
# g++ -MM src/diff/DiffObj.cpp
$(BUILD_DIR)/$(USERNAME)/src/diff/DiffObj.o: src/diff/DiffObj.cpp src/diff/DiffObj.h src/diff/ArgParser.h src/diff/constants/constants.h src/diff/BinObj.h src/diff/ImageObj.h src/diff/FolderObj.h src/diff/TextObj.h
# g++ -MM src/diff/ImageObj.cpp
$(BUILD_DIR)/$(USERNAME)/src/diff/ImageObj.o: src/diff/ImageObj.cpp src/diff/ImageObj.h src/diff/DiffObj.h src/diff/ArgParser.h src/diff/constants/constants.h
# g++ -MM src/diff/FolderObj.cpp
$(BUILD_DIR)/$(USERNAME)/src/diff/FolderObj.o: src/diff/FolderObj.cpp src/diff/FolderObj.h src/diff/DiffObj.h src/diff/ArgParser.h src/diff/constants/constants.h src/diff/TextObj.h
# g++ -MM src/diff/TextObj.cpp
$(BUILD_DIR)/$(USERNAME)/src/diff/TextObj.o: src/diff/TextObj.cpp src/diff/TextObj.h src/diff/DiffObj.h src/diff/ArgParser.h src/diff/constants/constants.h
# g++ -MM src/diff/BinObj.cpp
$(BUILD_DIR)/$(USERNAME)/src/diff/BinObj.o: src/diff/BinObj.cpp src/diff/BinObj.h src/diff/DiffObj.h src/diff/ArgParser.h src/diff/constants/constants.h
