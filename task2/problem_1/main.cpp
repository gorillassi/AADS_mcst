#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;


class ManualArray {
public:
    int* data;
    int size;
    ManualArray(int s) : size(s) { data = new int[s]; }
    ~ManualArray() { delete[] data; }
    int get(int index) { return data[index]; }
    void set(int index, int value) { data[index] = value; }
};

void bubbleSort(ManualArray& arr) {
    bool swapped;
    for (int i = 0; i < arr.size - 1; ++i) {
        swapped = false;
        for (int j = 0; j < arr.size - i - 1; ++j) {
            if (arr.get(j) > arr.get(j + 1)) {
                int temp = arr.get(j);
                arr.set(j, arr.get(j + 1));
                arr.set(j + 1, temp);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void shakerSort(ManualArray& arr) {
    bool swapped = true;
    int start = 0, end = arr.size - 1;
    while (swapped) {
        swapped = false;
        for (int i = start; i < end; ++i) {
            if (arr.get(i) > arr.get(i + 1)) {
                int temp = arr.get(i);
                arr.set(i, arr.get(i + 1));
                arr.set(i + 1, temp);
                swapped = true;
            }
        }
        if (!swapped) break;
        swapped = false;
        --end;
        for (int i = end; i > start; --i) {
            if (arr.get(i) < arr.get(i - 1)) {
                int temp = arr.get(i);
                arr.set(i, arr.get(i - 1));
                arr.set(i - 1, temp);
                swapped = true;
            }
        }
        ++start;
    }
}

void combSort(ManualArray& arr) {
    int gap = arr.size;
    const double shrink = 1.3;
    bool swapped = true;
    while (gap > 1 || swapped) {
        gap = static_cast<int>(gap / shrink);
        if (gap < 1) gap = 1;
        swapped = false;
        for (int i = 0; i + gap < arr.size; ++i) {
            if (arr.get(i) > arr.get(i + gap)) {
                int temp = arr.get(i);
                arr.set(i, arr.get(i + gap));
                arr.set(i + gap, temp);
                swapped = true;
            }
        }
    }
}

ManualArray generateArray(int size, double disorder) {
    ManualArray arr(size);
    for (int i = 0; i < size; i++) {
        arr.set(i, i + 1);
    }
    int numSwaps = static_cast<int>(size * disorder);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, size - 1);
    for (int i = 0; i < numSwaps; ++i) {
        int idx1 = dis(gen);
        int idx2 = dis(gen);
        int temp = arr.get(idx1);
        arr.set(idx1, arr.get(idx2));
        arr.set(idx2, temp);
    }
    return arr;
}

// Функция измерения времени
long long measureTime(void (*sortFunction)(ManualArray&), int size, double disorder, int iterations = 5) {
    long long total_time = 0;
    for (int i = 0; i < iterations; ++i) {
        ManualArray arr = generateArray(size, disorder);
        auto start = high_resolution_clock::now();
        sortFunction(arr);
        auto stop = high_resolution_clock::now();
        total_time += duration_cast<microseconds>(stop - start).count();
    }
    return total_time / iterations;
}

int main() {
    int sizes[] = {10, 100, 1000, 10000, 100000};
    double disorders[] = {0.2, 0.4, 0.6, 0.8, 1.0};
    
    ofstream file("sorting_results.csv");
    file << "Algorithm,Size,Disorder,Time" << endl;
    
    for (int size : sizes) {
        cout << "\nОбработка массива размером: " << size << " элементов" << endl;
        for (double disorder : disorders) {
            file << "Bubble Sort," << size << "," << disorder * 100 << "," << measureTime(bubbleSort, size, disorder) << endl;
            file << "Shaker Sort," << size << "," << disorder * 100 << "," << measureTime(shakerSort, size, disorder) << endl;
            file << "Comb Sort," << size << "," << disorder * 100 << "," << measureTime(combSort, size, disorder) << endl;
        }
    }
    
    file.close();
    return 0;
}
