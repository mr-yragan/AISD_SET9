#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

class StringMergeSort {
private:
    int lcpCompare(const string& first, const string& second) {
        int firstSize = first.size();
        int secondSize = second.size();
        int minSize = firstSize;

        if (secondSize < minSize) {
            minSize = secondSize;
        }

        int lcp = 0;

        while (lcp < minSize && first[lcp] == second[lcp]) {
            lcp++;
        }

        if (lcp == firstSize && lcp == secondSize) {
            return 0;
        }

        if (lcp == firstSize) {
            return -1;
        }

        if (lcp == secondSize) {
            return 1;
        }

        if (first[lcp] < second[lcp]) {
            return -1;
        }

        return 1;
    }

    void merge(vector<string>& array, vector<string>& buffer, int left, int middle, int right) {
        int i = left;
        int j = middle + 1;
        int k = left;

        while (i <= middle && j <= right) {
            if (lcpCompare(array[i], array[j]) <= 0) {
                buffer[k] = array[i];
                i++;
            } else {
                buffer[k] = array[j];
                j++;
            }

            k++;
        }

        while (i <= middle) {
            buffer[k] = array[i];
            i++;
            k++;
        }

        while (j <= right) {
            buffer[k] = array[j];
            j++;
            k++;
        }

        for (int index = left; index <= right; index++) {
            array[index] = buffer[index];
        }
    }

    void mergeSort(vector<string>& array, vector<string>& buffer, int left, int right) {
        if (left >= right) {
            return;
        }

        int middle = (left + right) / 2;

        mergeSort(array, buffer, left, middle);
        mergeSort(array, buffer, middle + 1, right);
        merge(array, buffer, left, middle, right);
    }

public:
    void sort(vector<string>& array) {
        if (array.empty()) {
            return;
        }

        vector<string> buffer(array.size());
        mergeSort(array, buffer, 0, array.size() - 1);
    }
};

int main() {
    int n;
    cin >> n;

    vector<string> array(n);

    for (int i = 0; i < n; i++) {
        cin >> array[i];
    }

    StringMergeSort sorter;
    sorter.sort(array);

    for (int i = 0; i < n; i++) {
        cout << array[i] << "\n";
    }

    return 0;
}