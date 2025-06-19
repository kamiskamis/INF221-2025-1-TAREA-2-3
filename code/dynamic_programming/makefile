CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./algorithm

SOURCES = dynamic_programming.cpp algorithm/sequence_difference.cpp
EXECUTABLE = dynamic_programming_executable

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)