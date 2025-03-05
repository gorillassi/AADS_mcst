import matplotlib.pyplot as plt
import pandas as pd

# Загрузка данных из CSV файла
data = pd.read_csv("sorting_times.csv")

# График для различных размеров массива
plt.figure(figsize=(10, 6))
for shuffle_percent in data['ShufflingPercent'].unique():
    subset = data[data['ShufflingPercent'] == shuffle_percent]
    plt.plot(subset['N'], subset['BubbleSortTime'], label=f'Bubble Sort (Shuffle {shuffle_percent}%)')
    plt.plot(subset['N'], subset['ShakerSortTime'], label=f'Shaker Sort (Shuffle {shuffle_percent}%)')
    plt.plot(subset['N'], subset['CombSortTime'], label=f'Comb Sort (Shuffle {shuffle_percent}%)')

plt.xlabel('Размер массива (N)')
plt.ylabel('Время (мс)')
plt.title('Сравнение времени работы для разных размеров массива')
plt.legend()
plt.grid(True)
plt.show()

# График для степени упорядоченности
plt.figure(figsize=(10, 6))
for size in data['N'].unique():
    subset = data[data['N'] == size]
    plt.plot(subset['ShufflingPercent'], subset['BubbleSortTime'], label=f'Bubble Sort (Size {size})')
    plt.plot(subset['ShufflingPercent'], subset['ShakerSortTime'], label=f'Shaker Sort (Size {size})')
    plt.plot(subset['ShufflingPercent'], subset['CombSortTime'], label=f'Comb Sort (Size {size})')

plt.xlabel('Степень упорядоченности (%)')
plt.ylabel('Время (мс)')
plt.title('Сравнение времени работы для разных степеней упорядоченности')
plt.legend()
plt.grid(True)
plt.show()
