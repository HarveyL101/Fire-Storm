COMPILER = g++
FLAGS = --std=c++20 -Wall -Wextra -Wpedantic -Werror -Iinclude
SRC = src/*.cpp
INCLUDE = include/*.hpp
TARGET = build/output.exe

.PHONY: all build clean run

all: $(TARGET)

$(TARGET): $(SRC) | build
	$(COMPILER) $(FLAGS) $(SRC) $(INCLUDE) -o $(TARGET)

build:
	mkdir -p build

clean: 
	rm -rf build

run:
	./$(TARGET)