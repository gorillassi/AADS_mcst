import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

file_path = "sorting_results.csv"
try:
    df = pd.read_csv(file_path)

    df["Size"] = df["Size"].astype(int)
    df["Disorder"] = df["Disorder"].astype(float)
    df["Time"] = df["Time"].astype(float)

    plt.figure(figsize=(12, 6))
    sns.lineplot(data=df, x="Size", y="Time", hue="Algorithm", style="Disorder", markers=True)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Размер массива (лог масштаб)")
    plt.ylabel("Среднее время выполнения (мкс, лог масштаб)")
    plt.title("Зависимость времени сортировки от размера массива и степени перемешивания")
    plt.legend(title="Алгоритм и % перемешивания")
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.show()

    plt.figure(figsize=(12, 6))
    sns.lineplot(data=df, x="Disorder", y="Time", hue="Algorithm", style="Size", markers=True)
    plt.xlabel("Степень перемешивания массива (%)")
    plt.ylabel("Среднее время выполнения (мкс)")
    plt.title("Зависимость времени сортировки от степени перемешивания массива")
    plt.legend(title="Алгоритм и размер массива")
    plt.grid(True, linestyle="--", linewidth=0.5)
    plt.show()
except FileNotFoundError:
    print(f"Ошибка: Файл {file_path} не найден. Сначала запустите C++ программу для генерации данных.")
