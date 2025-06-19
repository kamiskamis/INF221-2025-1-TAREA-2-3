CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ialgorithm

SOURCES = brute_force.cpp algorithm/sequence_difference.cpp
EXECUTABLE = brute_force_executable

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)