import pandas as pd
import matplotlib.pyplot as plt

csv_path = "sort_results.csv"

try:
    df = pd.read_csv(csv_path)

    size_data = df[df["Parameter"].str.contains("Size")]
    disorder_data = df[df["Parameter"].str.contains("Disorder")]
    digits_data = df[df["Parameter"].str.contains("Digits")]

    size_data["Parameter"] = size_data["Parameter"].str.replace("Size ", "").astype(int)
    disorder_data["Parameter"] = disorder_data["Parameter"].str.replace("Disorder ", "").astype(float)
    digits_data["Parameter"] = digits_data["Parameter"].str.replace("Digits ", "").astype(int)

    # === ГРАФИК 1 ===
    plt.figure(figsize=(10, 5))
    for algorithm in size_data["Algorithm"].unique():
        subset = size_data[size_data["Algorithm"] == algorithm]
        plt.plot(subset["Parameter"], subset["Time"], marker='o', linestyle='-', label=algorithm)

    plt.xscale("log")  
    plt.yscale("log")  
    plt.xlabel("Размер массива")
    plt.ylabel("Время (сек)")
    plt.title("Время работы сортировок от размера массива")
    plt.legend()
    plt.grid()
    plt.show()

    # === ГРАФИК 2 ===
    plt.figure(figsize=(10, 5))
    for algorithm in disorder_data["Algorithm"].unique():
        subset = disorder_data[disorder_data["Algorithm"] == algorithm]
        plt.plot(subset["Parameter"], subset["Time"], marker='o', linestyle='-', label=algorithm)

    plt.xlabel("Степень упорядоченности (доля перемешанных элементов)")
    plt.ylabel("Время (сек)")
    plt.title("Влияние упорядоченности на время работы сортировок")
    plt.legend()
    plt.grid()
    plt.show()

    # === ГРАФИК 3 ===
    plt.figure(figsize=(10, 5))
    for algorithm in digits_data["Algorithm"].unique():
        subset = digits_data[digits_data["Algorithm"] == algorithm]
        plt.plot(subset["Parameter"], subset["Time"], marker='o', linestyle='-', label=algorithm)

    plt.xlabel("Количество бит в числе")
    plt.ylabel("Время (сек)")
    plt.title("Влияние количества разрядов чисел на время сортировки")
    plt.legend()
    plt.grid()
    plt.show()

except FileNotFoundError:
    print("Файл sort_results.csv не найден. Сначала запустите C++ код для генерации данных.")
