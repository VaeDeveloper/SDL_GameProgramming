
CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs/"
SOURCE_FILES = src/*.cpp src/Game/*.cpp src/Logger/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.4 

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SOURCE_FILES) $(LINKER_FLAGS) -o gameengine;

run:
	./gameengine

clear:
	rm gameengine
