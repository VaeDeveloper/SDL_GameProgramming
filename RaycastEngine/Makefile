CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic

SRC = $(wildcard src/*.c src/Utilities/*.c)
OBJ = $(SRC:.c = .o)
	

TARGET = PhysicApp

all: $(TARGET)

$(TARGET) : $(OBJ)
	$(CXX) -o $@ $^

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET) 



