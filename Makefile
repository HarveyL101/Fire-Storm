COMPILER = g++
FLAGS = --std=c++20 -Wall -Wextra -Wpedantic -Werror
SRC = src/main.cpp
# INCLUDE =""
TARGET = build/dump_test.exe

.PHONY: all build clean run

all: $(TARGET)

$(TARGET): $(SRC) | build
	$(COMPILER) $(FLAGS) $(SRC) -o $(TARGET)

build:
	mkdir -p build

clean: 
	rm -rf build

run:
	./$(TARGET)

