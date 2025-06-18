CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

TARGET = sequence_difference
SRC = brute_force.cpp algorithm/sequence_difference.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
