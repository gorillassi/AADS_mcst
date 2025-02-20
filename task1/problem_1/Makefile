CC=g++
CFLAGS=-Wall -Wextra -O2 -std=c++11
TARGET=canonical_path

all: $(TARGET)

$(TARGET): canonical_path.cpp
	$(CC) $(CFLAGS) -o $(TARGET) canonical_path.cpp

run: $(TARGET)
	@echo "Введите путь:" && ./$(TARGET)

test: $(TARGET)
	@echo "\nРезультаты тестов:" 
	@while read line; do \
		echo "Вход: $$line"; \
		echo "Выход:"; \
		echo "$$line" | ./$(TARGET); \
	done < tests.txt

clean:
	rm -f $(TARGET)