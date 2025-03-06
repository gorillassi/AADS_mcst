CXX = g++
CXXFLAGS = -std=c++17 -O2

SRC = main.cpp
TARGET = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)
	python3 plot_graphs.py

clean:
	rm -f $(TARGET)

python:
	python3 plot_graphs.py
