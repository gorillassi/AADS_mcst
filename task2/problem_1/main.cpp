#include <iostream>
#include <chrono>
#include <random>
#include <cstdlib>
#include <fstream>

using namespace std;
using namespace std::chrono;

struct DynamicArray {
    int* data;
    int size;
    int capacity;

    DynamicArray(int initial_capacity = 10) {
        capacity = initial_capacity;
        size = 0;
        data = new int[capacity];
    }

    DynamicArray(const DynamicArray& other) {
        size = other.size;
        capacity = other.capacity;
        data = new int[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;  

        delete[] data; 

        size = other.size;
        capacity = other.capacity;
        data = new int[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
    }

    void push_back(int value) {
        if (size == capacity) {
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

    int& operator[](int index) {
        return data[index];
    }

    int operator[](int index) const {
        return data[index];
    }
};

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
        if (!swapped) break;
    }
}

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

DynamicArray generateShuffledArray(int size, int max_value, int shuffle_percent) {
    DynamicArray arr = generateRandomArray(size, max_value);
    int shuffle_count = size * shuffle_percent / 100;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, size - 1);

    for (int i = 0; i < size; i++) {
        arr[i] = arr[i];
    }

    for (int i = 0; i < shuffle_count; i++) {
        int idx1 = dis(gen);
        int idx2 = dis(gen);
        swap(arr[idx1], arr[idx2]);
    }

    return arr;
}

template<typename Func>
long long measureTime(Func sortFunction, DynamicArray& arr) {
    auto start = high_resolution_clock::now();
    sortFunction(arr);
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start).count();
}

void writeResultsToCSV(const string& filename, int size, int shuffle_percent, long long bubbleTime, long long shakerTime, long long combTime) {
    ofstream outFile;
    outFile.open(filename, ios::app); 
    if (outFile.is_open()) {
        outFile << size << "," << shuffle_percent << "," << bubbleTime << "," << shakerTime << "," << combTime << endl;
    } else {
        cerr << "Ошибка открытия файла!" << endl;
    }
}

int main() {
    string filename = "sorting_times.csv";
    ofstream outFile;
    outFile.open(filename);
    outFile << "N,ShufflingPercent,BubbleSortTime,ShakerSortTime,CombSortTime" << endl;
    outFile.close();

    int max_size = 100000;
    int step = 10;

    for (int size = 10; size <= max_size; size *= step) {
        for (int shuffle_percent = 0; shuffle_percent <= 100; shuffle_percent += 20) {
            DynamicArray arr = generateShuffledArray(size, 99999, shuffle_percent);


            DynamicArray arr_copy = arr;
            long long bubbleTime = measureTime(bubbleSort, arr_copy);

            arr_copy = arr;
            long long shakerTime = measureTime(shakerSort, arr_copy);

            arr_copy = arr;
            long long combTime = measureTime(combSort, arr_copy);

            writeResultsToCSV(filename, size, shuffle_percent, bubbleTime, shakerTime, combTime);

            cout << "N=" << size << ", Shuffle=" << shuffle_percent << "%, "
                 << "Bubble=" << bubbleTime << "ms, "
                 << "Shaker=" << shakerTime << "ms, "
                 << "Comb=" << combTime << "ms" << endl;
        }
    }
    return 0;
}
