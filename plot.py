import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("results_a1.csv", sep=";")

algorithm_names = {
    "QuickSort": "Обычный QuickSort",
    "MergeSort": "Обычный MergeSort",
    "StringQuickSort": "String QuickSort",
    "StringMergeSort": "String MergeSort с LCP",
    "MsdRadixSort": "MSD Radix Sort",
    "MsdRadixSortWithQuickSort": "MSD Radix Sort + String QuickSort"
}

array_type_names = {
    "random": "случайный массив",
    "reverse_sorted": "обратно отсортированный массив",
    "nearly_sorted": "почти отсортированный массив"
}

groups = {
    "standard": {
        "name": "стандартные сортировки",
        "algorithms": ["QuickSort", "MergeSort"]
    },
    "string": {
        "name": "строковые сортировки",
        "algorithms": ["StringQuickSort", "StringMergeSort"]
    },
    "radix": {
        "name": "MSD Radix сортировки",
        "algorithms": ["MsdRadixSort", "MsdRadixSortWithQuickSort"]
    }
}

array_types = data["array_type"].unique()

for array_type in array_types:
    current = data[data["array_type"] == array_type]
    array_name = array_type_names[array_type]

    for group_name in groups:
        group = groups[group_name]
        algorithms = group["algorithms"]

        plt.figure(figsize=(10, 6))

        for algorithm in algorithms:
            part = current[current["algorithm"] == algorithm]
            part = part.sort_values("size")

            plt.plot(
                part["size"],
                part["time_microseconds"],
                marker="o",
                label=algorithm_names[algorithm]
            )

        plt.title("Время работы: " + array_name + " / " + group["name"])
        plt.xlabel("Размер массива")
        plt.ylabel("Время, микросекунды")
        plt.legend()
        plt.grid()
        plt.tight_layout()
        plt.savefig("a1_time_" + array_type + "_" + group_name + ".png", dpi=200)
        plt.close()

        plt.figure(figsize=(10, 6))

        for algorithm in algorithms:
            part = current[current["algorithm"] == algorithm]
            part = part.sort_values("size")

            plt.plot(
                part["size"],
                part["char_comparisons"],
                marker="o",
                label=algorithm_names[algorithm]
            )

        plt.title("Операции с символами: " + array_name + " / " + group["name"])
        plt.xlabel("Размер массива")
        plt.ylabel("Количество посимвольных сравнений / обращений")
        plt.legend()
        plt.grid()
        plt.tight_layout()
        plt.savefig("a1_comparisons_" + array_type + "_" + group_name + ".png", dpi=200)
        plt.close()