#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

class MsdRadixQuickSort {
private:
    int alphabetPower = 74;

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

    void msdRadixSort(vector<string>& array, vector<string>& buffer, int left, int right, int depth) {
        if (left >= right) {
            return;
        }

        if (right - left + 1 < alphabetPower) {
            stringQuickSort(array, left, right, depth);
            return;
        }

        int radix = 256;
        vector<int> count(radix + 2, 0);

        for (int i = left; i <= right; i++) {
            int symbol = getSymbol(array[i], depth);
            count[symbol + 2]++;
        }

        for (int i = 0; i < radix + 1; i++) {
            count[i + 1] += count[i];
        }

        vector<int> position = count;

        for (int i = left; i <= right; i++) {
            int symbol = getSymbol(array[i], depth);
            buffer[left + position[symbol + 1]] = array[i];
            position[symbol + 1]++;
        }

        for (int i = left; i <= right; i++) {
            array[i] = buffer[i];
        }

        for (int i = 1; i <= radix; i++) {
            int newLeft = left + count[i];
            int newRight = left + count[i + 1] - 1;

            if (newLeft < newRight) {
                msdRadixSort(array, buffer, newLeft, newRight, depth + 1);
            }
        }
    }

public:
    void sort(vector<string>& array) {
        if (array.empty()) {
            return;
        }

        vector<string> buffer(array.size());
        msdRadixSort(array, buffer, 0, array.size() - 1, 0);
    }
};

int main() {
    int n;
    cin >> n;

    vector<string> array(n);

    for (int i = 0; i < n; i++) {
        cin >> array[i];
    }

    MsdRadixQuickSort sorter;
    sorter.sort(array);

    for (int i = 0; i < n; i++) {
        cout << array[i] << "\n";
    }

    return 0;
}