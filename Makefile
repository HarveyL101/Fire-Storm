SRC = "src/**.cpp"
INCLUDE ="include/**.hpp"
TARGET = "build/fire_storm.exe"
FLAGS = "-std=c++20 -Wall -Wextra -Wpedantic -Werror"

g++ $(SRC) $(INCLUDE) $(TARGET) $(FLAGS) 
