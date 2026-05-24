#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

class StringQuickSort {
private:
    int getSymbol(const string& value, int index) {
        int valueSize = value.size();

        if (index >= valueSize) {
            return -1;
        }

        return value[index];
    }

    void stringQuickSort(vector<string>& array, int left, int right, int depth) {
        if (left >= right) {
            return;
        }

        int lower = left;
        int greater = right;
        int pivot = getSymbol(array[(left + right) / 2], depth);
        int index = left;

        while (index <= greater) {
            int current = getSymbol(array[index], depth);

            if (current < pivot) {
                std::swap(array[lower], array[index]);
                lower++;
                index++;
            } else if (current > pivot) {
                std::swap(array[index], array[greater]);
                greater--;
            } else {
                index++;
            }
        }

        stringQuickSort(array, left, lower - 1, depth);

        if (pivot >= 0) {
            stringQuickSort(array, lower, greater, depth + 1);
        }

        stringQuickSort(array, greater + 1, right, depth);
    }

public:
    void sort(vector<string>& array) {
        if (array.empty()) {
            return;
        }

        stringQuickSort(array, 0, array.size() - 1, 0);
    }
};

int main() {
    int n;
    cin >> n;

    vector<string> array(n);

    for (int i = 0; i < n; i++) {
        cin >> array[i];
    }

    StringQuickSort sorter;
    sorter.sort(array);

    for (int i = 0; i < n; i++) {
        cout << array[i] << "\n";
    }

    return 0;
}