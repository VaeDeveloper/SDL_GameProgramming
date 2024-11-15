# Компилятор и флаги
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I.

# Директории
SRC_DIR := .
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin

# Исполняемый файл
TARGET := $(BIN_DIR)/2DPhysicEngine

# Список исходников
SRCS := $(wildcard $(SRC_DIR)/AngryApp/*.cpp) \
        $(wildcard $(SRC_DIR)/CollisionBounce/*.cpp) \
        $(wildcard $(SRC_DIR)/FabricSimulation/*.cpp) \
        $(wildcard $(SRC_DIR)/Physics/Objects/*.cpp) \
        $(wildcard $(SRC_DIR)/Physics/Utils/*.cpp)

# Объектные файлы
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Правила сборки
.PHONY: all clean

all: $(TARGET)

# Создание исполняемого файла
$(TARGET): $(OBJS)
	@echo "Linking target: $@"
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@

# Компиляция исходников
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $< -> $@"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	@echo "Cleaning up build files..."
	@rm -rf $(BUILD_DIR)
