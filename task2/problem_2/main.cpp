#include <iostream>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <random>

#define BASE_N 10

struct Queue {
    long long* data;
    ssize_t capacity;
    ssize_t front;
    ssize_t rear;
    ssize_t size;

    Queue(ssize_t cap) {
        capacity = cap;
        data = new long long[cap];
        front = rear = size = 0;
    }

    ~Queue() {
        delete[] data;
    }

    bool isEmpty() {
        return size == 0;
    }

    void enqueue(long long value) {
        if (size < capacity) {
            data[rear] = value;
            rear = (rear + 1) % capacity;
            size++;
        }
    }

    long long dequeue() {
        if (size > 0) {
            long long value = data[front];
            front = (front + 1) % capacity;
            size--;
            return value;
        }
        return -1;
    }
};

long long get_digit(long long n, long long digit) {
    for (long long i = 1; i < digit; i++) {
        n /= BASE_N;
    }
    return n % BASE_N;
}

void array_radix_sort(long long* array, ssize_t size) {
    if (size <= 1) return;

    long long max_number = array[0];
    for (ssize_t i = 1; i < size; i++) {
        if (array[i] > max_number) {
            max_number = array[i];
        }
    }

    long long max_digits = 0;
    while (max_number > 0) {
        max_digits++;
        max_number /= BASE_N;
    }

    Queue* buckets[BASE_N];
    for (int i = 0; i < BASE_N; i++) {
        buckets[i] = new Queue(size);
    }

    for (long long digit = 1; digit <= max_digits; digit++) {
        for (ssize_t i = 0; i < size; i++) {
            long long d = get_digit(array[i], digit);
            buckets[d]->enqueue(array[i]);
        }

        ssize_t index = 0;
        for (int i = 0; i < BASE_N; i++) {
            while (!buckets[i]->isEmpty()) {
                array[index++] = buckets[i]->dequeue();
            }
        }
    }

    for (int i = 0; i < BASE_N; i++) {
        delete buckets[i];
    }
}

void array_std_sort(long long* array, ssize_t size) {
    std::sort(array, array + size);
}

void generate_random_array(long long* array, ssize_t size, long long max_value) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(0, max_value);
    for (ssize_t i = 0; i < size; i++) {
        array[i] = dis(gen);
    }
}

void shuffle_partially(long long* array, ssize_t size, double disorder) {
    ssize_t swaps = static_cast<ssize_t>(size * disorder);
    std::random_device rd;
    std::mt19937 gen(rd());

    for (ssize_t i = 0; i < swaps; i++) {
        ssize_t idx1 = gen() % size;
        ssize_t idx2 = gen() % size;
        std::swap(array[idx1], array[idx2]);
    }
}

template <typename SortingFunction>
void test_sort(long long* array, ssize_t size, SortingFunction sorting_function,
               const std::string& sorting_name, std::ofstream& results_file, const std::string& test_param) {
    long long* temp_array = new long long[size];
    std::copy(array, array + size, temp_array);

    auto start = std::chrono::high_resolution_clock::now();
    sorting_function(temp_array, size);
    auto end = std::chrono::high_resolution_clock::now();

    double elapsed_time = std::chrono::duration<double>(end - start).count();

    results_file << sorting_name << "," << test_param << "," << elapsed_time << "\n";

    delete[] temp_array;
}

int main() {
    std::ofstream results_file("sort_results.csv");
    results_file << "Algorithm,Parameter,Time\n";

    ssize_t sizes[] = {10, 100, 1000, 10000, 100000, 1000000};
    double disorders[] = {0.2, 0.4, 0.6, 0.8, 1.0};
    long long max_values[] = {255, 65535, 16777215, 2147483647};

    // Тест по размеру массива
    for (ssize_t size : sizes) {
        long long* array = new long long[size];
        generate_random_array(array, size, 2147483647);
        test_sort(array, size, array_radix_sort, "Radix Sort", results_file, "Size " + std::to_string(size));
        test_sort(array, size, array_std_sort, "std::sort", results_file, "Size " + std::to_string(size));
        delete[] array;
    }

    // Тест по степени упорядоченности
    ssize_t test_size = 100000;
    for (double disorder : disorders) {
        long long* array = new long long[test_size];
        for (ssize_t i = 0; i < test_size; i++) {
            array[i] = i;
        }
        shuffle_partially(array, test_size, disorder);
        test_sort(array, test_size, array_radix_sort, "Radix Sort", results_file, "Disorder " + std::to_string(disorder));
        test_sort(array, test_size, array_std_sort, "std::sort", results_file, "Disorder " + std::to_string(disorder));
        delete[] array;
    }

    // Тест по количеству разрядов
    for (long long max_val : max_values) {
        long long* array = new long long[test_size];
        generate_random_array(array, test_size, max_val);
        test_sort(array, test_size, array_radix_sort, "Radix Sort", results_file, "Digits " + std::to_string(max_val));
        test_sort(array, test_size, array_std_sort, "std::sort", results_file, "Digits " + std::to_string(max_val));
        delete[] array;
    }

    results_file.close();
    std::cout << "Тестирование завершено. Данные сохранены в sort_results.csv" << std::endl;
    return 0;
}
