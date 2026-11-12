CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Werror -Iinclude
SRC      = $(wildcard src/*.cpp)
INCLUDE  = $(wildcard include/*.hpp)
TARGET   = build/firestorm.exe

.PHONY: all build clean run container

all: $(TARGET)

$(TARGET): $(SRC) $(INCLUDE) | build
	$(CXX) $(CXXFLAGS) $(SRC) -o $@

build:
	mkdir -p build

clean: 
	rm -rf build

run: all
	./$(TARGET)

container: 
	./container.sh