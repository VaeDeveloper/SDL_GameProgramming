# 
CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors -Wextra -pedantic
INCLUDE_PATH = -I"./libs/"

# 
SOURCE_FILES = ./src/*.cpp \
               ./src/Game/*.cpp \
               ./src/Logger/*.cpp \
               ./src/ECS/*.cpp \
               ./src/AssetManager/*.cpp \
			   ./src/FileManager/*.cpp

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.4

# 
OBJECT_NAME = gameengine
BUILD_DIR = build
REPORT_DIR = reports

# 
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NO_COLOR = \033[0m

# 
.PHONY: all build run clean report

# 
all: build report

# 
build:
	@echo -e "$(YELLOW)[Building Project]$(NO_COLOR)"
	@mkdir -p $(BUILD_DIR)
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SOURCE_FILES) $(LINKER_FLAGS) -o $(BUILD_DIR)/$(OBJECT_NAME)
	@echo -e "$(GREEN)[Build Complete]$(NO_COLOR)"

# 
run:
	@echo -e "$(YELLOW)[Running $(OBJECT_NAME)]$(NO_COLOR)"
	@./$(BUILD_DIR)/$(OBJECT_NAME)

# 
clean:
	@echo -e "$(RED)[Cleaning Project Files]$(NO_COLOR)"
	@rm -rf $(BUILD_DIR)
	@rm -rf $(REPORT_DIR)
	@echo -e "$(GREEN)[Clean Complete]$(NO_COLOR)"

# 
report:
	@echo -e "$(YELLOW)[Generating Build Report]$(NO_COLOR)"
	@mkdir -p $(REPORT_DIR)
	@echo "Build Date: $$(date)" > $(REPORT_DIR)/build_report.txt
	@echo "Source Files:" >> $(REPORT_DIR)/build_report.txt
	@echo "$(SOURCE_FILES)" >> $(REPORT_DIR)/build_report.txt
	@echo -e "$(GREEN)[Build Report Generated at $(REPORT_DIR)/build_report.txt]$(NO_COLOR)"
