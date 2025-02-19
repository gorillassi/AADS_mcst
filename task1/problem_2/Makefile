CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
TARGET = browser_history
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@echo "Running tests..."
	@./$(TARGET) < tests/test1.in > tests/test1.out && diff tests/test1.out tests/test1.expected || echo "Test 1 failed"
	@./$(TARGET) < tests/test2.in > tests/test2.out && diff tests/test2.out tests/test2.expected || echo "Test 2 failed"

clean:
	rm -f $(TARGET) tests/*.out