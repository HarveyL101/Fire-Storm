COMPILER = g++
FLAGS = "-std=c++20 -Wall -Wextra -Wpedantic -Werror"
SRC = "src/%.cpp"
INCLUDE ="include/%.hpp"
TARGET = build

COMPILE = $(COMPILER) $(FLAGS)

make:
	$(COMPILE) $(SRC) $(INCLUDE) $(TARGET)

build: 
	mkdir -p $(TARGET)

clean:
	rm -rf $(TARGET)

