#include <iostream>
#include <chrono>
#include <random>
#include <cstdlib>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Структура для динамического массива
struct DynamicArray {
    int* data;
    int size;
    int capacity;

    // Конструктор
    DynamicArray(int initial_capacity = 10) {
        capacity = initial_capacity;
        size = 0;
        data = new int[capacity];
    }

    // Конструктор копирования
    DynamicArray(const DynamicArray& other) {
        size = other.size;
        capacity = other.capacity;
        data = new int[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Оператор присваивания
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;  // Защита от самоприсваивания

        delete[] data;  // Освобождаем старую память

        size = other.size;
        capacity = other.capacity;
        data = new int[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

    // Деструктор
    ~DynamicArray() {
        delete[] data;
    }

    // Добавление элемента в конец
    void push_back(int value) {
        if (size == capacity) {
            // Увеличиваем емкость массива в два раза, если достигнут предел
            capacity *= 2;
            int* new_data = new int[capacity];
            for (int i = 0; i < size; ++i) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
        }
        data[size++] = value;
    }

    // Получение элемента по индексу
    int& operator[](int index) {
        return data[index];
    }

    // Получение элемента по индексу (для чтения)
    int operator[](int index) const {
        return data[index];
    }
};

// Стандартная сортировка пузырьком
void bubbleSort(DynamicArray& arr) {
    int n = arr.size;
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // Если массив уже отсортирован, выходим
    }
}

// Шейкерная сортировка
void shakerSort(DynamicArray& arr) {
    int n = arr.size;
    bool swapped = true;
    int start = 0, end = n - 1;

    while (swapped) {
        swapped = false;
        for (int i = start; i < end; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
        end--;

        for (int i = end - 1; i >= start; i--) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        start++;
    }
}

// Сортировка расчёской
void combSort(DynamicArray& arr) {
    int n = arr.size;
    int gap = n;
    bool swapped = true;
    double shrink = 1.3;

    while (gap > 1 || swapped) {
        gap = (int)(gap / shrink);
        if (gap < 1) gap = 1;
        swapped = false;

        for (int i = 0; i < n - gap; i++) {
            if (arr[i] > arr[i + gap]) {
                swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
    }
}

// Генерация случайного массива
DynamicArray generateRandomArray(int size, int max_value) {
    DynamicArray arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, max_value);

    for (int i = 0; i < size; i++) {
        arr.push_back(dis(gen));
    }
    return arr;
}

// Генерация массива с заданным процентом перемешанных элементов
DynamicArray generateShuffledArray(int size, int max_value, int shuffle_percent) {
    DynamicArray arr = generateRandomArray(size, max_value);
    int shuffle_count = size * shuffle_percent / 100;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, size - 1);

    // Сортируем массив
    for (int i = 0; i < size; i++) {
        arr[i] = arr[i];
    }

    // Перемешиваем заданный процент элементов
    for (int i = 0; i < shuffle_count; i++) {
        int idx1 = dis(gen);
        int idx2 = dis(gen);
        swap(arr[idx1], arr[idx2]);
    }

    return arr;
}

// Функция для измерения времени выполнения сортировки
template<typename Func>
long long measureTime(Func sortFunction, DynamicArray& arr) {
    auto start = high_resolution_clock::now();
    sortFunction(arr);
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start).count();
}

// Функция для записи результатов в CSV файл
void writeResultsToCSV(const string& filename, int size, int shuffle_percent, long long bubbleTime, long long shakerTime, long long combTime) {
    ofstream outFile;
    outFile.open(filename, ios::app);  // Открываем файл в режиме добавления
    if (outFile.is_open()) {
        outFile << size << "," << shuffle_percent << "," << bubbleTime << "," << shakerTime << "," << combTime << endl;
    } else {
        cerr << "Ошибка открытия файла!" << endl;
    }
}

int main() {
    // Открытие CSV файла и запись заголовков
    string filename = "sorting_times.csv";
    ofstream outFile;
    outFile.open(filename);
    outFile << "N,ShufflingPercent,BubbleSortTime,ShakerSortTime,CombSortTime" << endl;
    outFile.close();

    // Настройки для тестов
    int max_size = 100000;
    int step = 10;

    for (int size = 10; size <= max_size; size *= step) {
        for (int shuffle_percent = 0; shuffle_percent <= 100; shuffle_percent += 20) {
            DynamicArray arr = generateShuffledArray(size, 99999, shuffle_percent);

            // Время работы для пузырьковой сортировки
            DynamicArray arr_copy = arr;
            long long bubbleTime = measureTime(bubbleSort, arr_copy);

            // Время работы для шейкерной сортировки
            arr_copy = arr;
            long long shakerTime = measureTime(shakerSort, arr_copy);

            // Время работы для сортировки расчёской
            arr_copy = arr;
            long long combTime = measureTime(combSort, arr_copy);

            // Сохранение результатов в CSV файл
            writeResultsToCSV(filename, size, shuffle_percent, bubbleTime, shakerTime, combTime);

            // Выводим результаты
            cout << "N=" << size << ", Shuffle=" << shuffle_percent << "%, "
                 << "Bubble=" << bubbleTime << "ms, "
                 << "Shaker=" << shakerTime << "ms, "
                 << "Comb=" << combTime << "ms" << endl;
        }
    }

    return 0;
}
