
CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs/"
SOURCE_FILES = ./src/*.cpp ./src/Game/*.cpp ./src/Logger/*.cpp ./src/ECS/*.cpp ./src/AssetManager/*.cpp 

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.4 
OBJECT_NAME = gameengine 
build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SOURCE_FILES) $(LINKER_FLAGS) -o gameengine;

run:
	./$(OBJECT_NAME)

clear:
	rm $(OBJECT_NAME)
