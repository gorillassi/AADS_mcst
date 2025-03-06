# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17

# Название исполняемого файла
TARGET = sort_test
SRC = main.cpp

# Цель по умолчанию
all: $(TARGET)

# Компиляция C++ кода
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Запуск тестов и генерация данных
run: $(TARGET)
	./$(TARGET)
	python3 plot_graphs.py

# Очистка
clean:
	rm -f $(TARGET) sort_results.csv
