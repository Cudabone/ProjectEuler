CXX = clang++
CXXFLAGS = -O3 -Wall -Wextra -std=c++17

SRC = Problem215.cpp
EXECUTABLE = Problem215

$(EXECUTABLE) : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXECUTABLE)

all: $(EXECUTABLE)

clean: 
	@rm -f $(EXECUTABLE)

.PHONY: all clean
