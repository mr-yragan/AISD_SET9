#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using std::cout;
using std::mt19937;
using std::ofstream;
using std::random_device;
using std::string;
using std::uniform_int_distribution;
using std::vector;

class StringGenerator {
private:
    string alphabet;
    mt19937 generator;

    string generateString() {
        uniform_int_distribution<int> lengthDistribution(10, 200);

        int lastIndex = alphabet.size() - 1;
        uniform_int_distribution<int> charDistribution(0, lastIndex);

        int length = lengthDistribution(generator);
        string result;

        for (int i = 0; i < length; i++) {
            result += alphabet[charDistribution(generator)];
        }

        return result;
    }

public:
    StringGenerator() {
        alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        alphabet += "abcdefghijklmnopqrstuvwxyz";
        alphabet += "0123456789";
        alphabet += "!@#%:;^&*()-.";
        random_device device;
        generator = mt19937(device());
    }

    vector<string> generateRandomArray(int size) {
        vector<string> array;

        for (int i = 0; i < size; i++) {
            array.push_back(generateString());
        }

        return array;
    }

    vector<string> generateReverseSortedArray(int size) {
        vector<string> array = generateRandomArray(size);

        std::sort(array.begin(), array.end());
        std::reverse(array.begin(), array.end());

        return array;
    }

    vector<string> generateNearlySortedArray(int size) {
        vector<string> array = generateRandomArray(size);

        std::sort(array.begin(), array.end());

        int swapsCount = size / 20;
        uniform_int_distribution<int> indexDistribution(0, size - 1);

        for (int i = 0; i < swapsCount; i++) {
            int firstIndex = indexDistribution(generator);
            int secondIndex = indexDistribution(generator);

            std::swap(array[firstIndex], array[secondIndex]);
        }

        return array;
    }

    vector<string> getSubarray(const vector<string>& array, int size) {
        vector<string> result;
        int arraySize = array.size();

        for (int i = 0; i < size && i < arraySize; i++) {
            result.push_back(array[i]);
        }

        return result;
    }
};

class StringSortTester {
private:
    int alphabetPower = 74;

    int getSymbol(const string& value, int index) {
        int valueSize = value.size();

        if (index >= valueSize) {
            return -1;
        }

        return value[index];
    }

    int compareStrings(const string& first, const string& second, long long& comparisons) {
        int firstSize = first.size();
        int secondSize = second.size();
        int minSize = firstSize;

        if (secondSize < minSize) {
            minSize = secondSize;
        }

        for (int i = 0; i < minSize; i++) {
            comparisons++;

            if (first[i] < second[i]) {
                return -1;
            }

            if (first[i] > second[i]) {
                return 1;
            }
        }

        if (firstSize < secondSize) {
            return -1;
        }

        if (firstSize > secondSize) {
            return 1;
        }

        return 0;
    }

    struct LcpItem {
        string value;
        int lcp;
    };

    struct LcpCompareResult {
        int order;
        int lcp;
    };

    LcpCompareResult lcpCompare(const string& first, const string& second, int start, long long& comparisons) {
        int firstSize = first.size();
        int secondSize = second.size();
        int minSize = firstSize;

        if (secondSize < minSize) {
            minSize = secondSize;
        }

        int lcp = start;

        if (lcp > minSize) {
            lcp = minSize;
        }

        while (lcp < minSize) {
            comparisons++;

            if (first[lcp] != second[lcp]) {
                break;
            }

            lcp++;
        }

        LcpCompareResult result;
        result.lcp = lcp;

        if (lcp == firstSize && lcp == secondSize) {
            result.order = 0;
            return result;
        }

        if (lcp == firstSize) {
            result.order = -1;
            return result;
        }

        if (lcp == secondSize) {
            result.order = 1;
            return result;
        }

        if (first[lcp] < second[lcp]) {
            result.order = -1;
        } else {
            result.order = 1;
        }

        return result;
    }

    void quickSort(vector<string>& array, int left, int right, long long& comparisons) {
        int i = left;
        int j = right;
        string pivot = array[(left + right) / 2];

        while (i <= j) {
            while (compareStrings(array[i], pivot, comparisons) < 0) {
                i++;
            }

            while (compareStrings(array[j], pivot, comparisons) > 0) {
                j--;
            }

            if (i <= j) {
                std::swap(array[i], array[j]);
                i++;
                j--;
            }
        }

        if (left < j) {
            quickSort(array, left, j, comparisons);
        }

        if (i < right) {
            quickSort(array, i, right, comparisons);
        }
    }

    void merge(vector<string>& array, vector<string>& buffer, int left, int middle, int right, long long& comparisons) {
        int i = left;
        int j = middle + 1;
        int k = left;

        while (i <= middle && j <= right) {
            if (compareStrings(array[i], array[j], comparisons) <= 0) {
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

    void mergeSort(vector<string>& array, vector<string>& buffer, int left, int right, long long& comparisons) {
        if (left >= right) {
            return;
        }

        int middle = (left + right) / 2;

        mergeSort(array, buffer, left, middle, comparisons);
        mergeSort(array, buffer, middle + 1, right, comparisons);
        merge(array, buffer, left, middle, right, comparisons);
    }

    void stringQuickSort(vector<string>& array, int left, int right, int depth, long long& comparisons) {
        if (left >= right) {
            return;
        }

        int lower = left;
        int greater = right;
        int pivot = getSymbol(array[(left + right) / 2], depth);
        int index = left;

        while (index <= greater) {
            int current = getSymbol(array[index], depth);
            comparisons++;

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

        stringQuickSort(array, left, lower - 1, depth, comparisons);

        if (pivot >= 0) {
            stringQuickSort(array, lower, greater, depth + 1, comparisons);
        }

        stringQuickSort(array, greater + 1, right, depth, comparisons);
    }

    vector<LcpItem> stringMerge(const vector<LcpItem>& firstArray, const vector<LcpItem>& secondArray, long long& comparisons) {
        vector<LcpItem> result;

        int i = 0;
        int j = 0;
        int firstSize = firstArray.size();
        int secondSize = secondArray.size();
        int firstLcp = 0;
        int secondLcp = 0;

        if (firstSize > 0) {
            firstLcp = firstArray[0].lcp;
        }

        if (secondSize > 0) {
            secondLcp = secondArray[0].lcp;
        }

        while (i < firstSize && j < secondSize) {
            if (firstLcp > secondLcp) {
                LcpItem item;
                item.value = firstArray[i].value;
                item.lcp = firstLcp;
                result.push_back(item);

                i++;

                if (i < firstSize) {
                    firstLcp = firstArray[i].lcp;
                }
            } else if (firstLcp < secondLcp) {
                LcpItem item;
                item.value = secondArray[j].value;
                item.lcp = secondLcp;
                result.push_back(item);

                j++;

                if (j < secondSize) {
                    secondLcp = secondArray[j].lcp;
                }
            } else {
                LcpCompareResult compareResult = lcpCompare(firstArray[i].value, secondArray[j].value, firstLcp, comparisons);

                if (compareResult.order <= 0) {
                    LcpItem item;
                    item.value = firstArray[i].value;
                    item.lcp = firstLcp;
                    result.push_back(item);

                    i++;
                    secondLcp = compareResult.lcp;

                    if (i < firstSize) {
                        firstLcp = firstArray[i].lcp;
                    }
                } else {
                    LcpItem item;
                    item.value = secondArray[j].value;
                    item.lcp = secondLcp;
                    result.push_back(item);

                    j++;
                    firstLcp = compareResult.lcp;

                    if (j < secondSize) {
                        secondLcp = secondArray[j].lcp;
                    }
                }
            }
        }

        while (i < firstSize) {
            LcpItem item;
            item.value = firstArray[i].value;
            item.lcp = firstLcp;
            result.push_back(item);

            i++;

            if (i < firstSize) {
                firstLcp = firstArray[i].lcp;
            }
        }

        while (j < secondSize) {
            LcpItem item;
            item.value = secondArray[j].value;
            item.lcp = secondLcp;
            result.push_back(item);

            j++;

            if (j < secondSize) {
                secondLcp = secondArray[j].lcp;
            }
        }

        return result;
    }

    vector<LcpItem> stringMergeSort(vector<string>& array, int left, int right, long long& comparisons) {
        if (left == right) {
            vector<LcpItem> result;
            LcpItem item;
            item.value = array[left];
            item.lcp = 0;
            result.push_back(item);

            return result;
        }

        int middle = (left + right) / 2;

        vector<LcpItem> firstArray = stringMergeSort(array, left, middle, comparisons);
        vector<LcpItem> secondArray = stringMergeSort(array, middle + 1, right, comparisons);

        return stringMerge(firstArray, secondArray, comparisons);
    }

    void msdRadixSort(vector<string>& array, vector<string>& buffer, int left, int right, int depth, long long& comparisons, bool useQuickSort) {
        if (left >= right) {
            return;
        }

        if (useQuickSort && right - left + 1 < alphabetPower) {
            stringQuickSort(array, left, right, depth, comparisons);
            return;
        }

        int radix = 256;
        vector<int> count(radix + 2, 0);

        for (int i = left; i <= right; i++) {
            int symbol = getSymbol(array[i], depth);
            comparisons++;
            count[symbol + 2]++;
        }

        for (int i = 0; i < radix + 1; i++) {
            count[i + 1] += count[i];
        }

        vector<int> position = count;

        for (int i = left; i <= right; i++) {
            int symbol = getSymbol(array[i], depth);
            comparisons++;
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
                msdRadixSort(array, buffer, newLeft, newRight, depth + 1, comparisons, useQuickSort);
            }
        }
    }

public:
    void testQuickSort(const vector<string>& source, long long& averageTime, long long& averageComparisons) {
        int measurementsCount = 5;

        averageTime = 0;
        averageComparisons = 0;

        for (int i = 0; i < measurementsCount; i++) {
            vector<string> array = source;
            long long comparisons = 0;

            auto start = std::chrono::high_resolution_clock::now();

            if (!array.empty()) {
                quickSort(array, 0, array.size() - 1, comparisons);
            }

            auto finish = std::chrono::high_resolution_clock::now();

            long long time = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();

            averageTime += time;
            averageComparisons += comparisons;
        }

        averageTime /= measurementsCount;
        averageComparisons /= measurementsCount;
    }

    void testMergeSort(const vector<string>& source, long long& averageTime, long long& averageComparisons) {
        int measurementsCount = 5;

        averageTime = 0;
        averageComparisons = 0;

        for (int i = 0; i < measurementsCount; i++) {
            vector<string> array = source;
            vector<string> buffer(array.size());
            long long comparisons = 0;

            auto start = std::chrono::high_resolution_clock::now();

            if (!array.empty()) {
                mergeSort(array, buffer, 0, array.size() - 1, comparisons);
            }

            auto finish = std::chrono::high_resolution_clock::now();

            long long time = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();

            averageTime += time;
            averageComparisons += comparisons;
        }

        averageTime /= measurementsCount;
        averageComparisons /= measurementsCount;
    }

    void testStringQuickSort(const vector<string>& source, long long& averageTime, long long& averageComparisons) {
        int measurementsCount = 5;

        averageTime = 0;
        averageComparisons = 0;

        for (int i = 0; i < measurementsCount; i++) {
            vector<string> array = source;
            long long comparisons = 0;

            auto start = std::chrono::high_resolution_clock::now();

            if (!array.empty()) {
                stringQuickSort(array, 0, array.size() - 1, 0, comparisons);
            }

            auto finish = std::chrono::high_resolution_clock::now();

            long long time = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();

            averageTime += time;
            averageComparisons += comparisons;
        }

        averageTime /= measurementsCount;
        averageComparisons /= measurementsCount;
    }

    void testStringMergeSort(const vector<string>& source, long long& averageTime, long long& averageComparisons) {
        int measurementsCount = 5;

        averageTime = 0;
        averageComparisons = 0;

        for (int i = 0; i < measurementsCount; i++) {
            vector<string> array = source;
            long long comparisons = 0;

            auto start = std::chrono::high_resolution_clock::now();

            if (!array.empty()) {
                vector<LcpItem> result = stringMergeSort(array, 0, array.size() - 1, comparisons);
                int resultSize = result.size();

                for (int index = 0; index < resultSize; index++) {
                    array[index] = result[index].value;
                }
            }

            auto finish = std::chrono::high_resolution_clock::now();

            long long time = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();

            averageTime += time;
            averageComparisons += comparisons;
        }

        averageTime /= measurementsCount;
        averageComparisons /= measurementsCount;
    }

    void testMsdRadixSort(const vector<string>& source, long long& averageTime, long long& averageComparisons) {
        int measurementsCount = 5;

        averageTime = 0;
        averageComparisons = 0;

        for (int i = 0; i < measurementsCount; i++) {
            vector<string> array = source;
            vector<string> buffer(array.size());
            long long comparisons = 0;

            auto start = std::chrono::high_resolution_clock::now();

            if (!array.empty()) {
                msdRadixSort(array, buffer, 0, array.size() - 1, 0, comparisons, false);
            }

            auto finish = std::chrono::high_resolution_clock::now();

            long long time = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();

            averageTime += time;
            averageComparisons += comparisons;
        }

        averageTime /= measurementsCount;
        averageComparisons /= measurementsCount;
    }

    void testMsdRadixSortWithQuickSort(const vector<string>& source, long long& averageTime, long long& averageComparisons) {
        int measurementsCount = 5;

        averageTime = 0;
        averageComparisons = 0;

        for (int i = 0; i < measurementsCount; i++) {
            vector<string> array = source;
            vector<string> buffer(array.size());
            long long comparisons = 0;

            auto start = std::chrono::high_resolution_clock::now();

            if (!array.empty()) {
                msdRadixSort(array, buffer, 0, array.size() - 1, 0, comparisons, true);
            }

            auto finish = std::chrono::high_resolution_clock::now();

            long long time = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();

            averageTime += time;
            averageComparisons += comparisons;
        }

        averageTime /= measurementsCount;
        averageComparisons /= measurementsCount;
    }
};

void writeResult(ofstream& file, const string& arrayType, int size, const string& algorithm, long long time, long long comparisons) {
    file << arrayType << ";";
    file << size << ";";
    file << algorithm << ";";
    file << time << ";";
    file << comparisons << "\n";
}

void runTests(StringGenerator& generator, StringSortTester& tester, const vector<string>& array, const string& arrayType, ofstream& file) {
    for (int size = 100; size <= 3000; size += 100) {
        vector<string> part = generator.getSubarray(array, size);

        long long time = 0;
        long long comparisons = 0;

        tester.testQuickSort(part, time, comparisons);
        writeResult(file, arrayType, size, "QuickSort", time, comparisons);

        tester.testMergeSort(part, time, comparisons);
        writeResult(file, arrayType, size, "MergeSort", time, comparisons);

        tester.testStringQuickSort(part, time, comparisons);
        writeResult(file, arrayType, size, "StringQuickSort", time, comparisons);

        tester.testStringMergeSort(part, time, comparisons);
        writeResult(file, arrayType, size, "StringMergeSort", time, comparisons);

        tester.testMsdRadixSort(part, time, comparisons);
        writeResult(file, arrayType, size, "MsdRadixSort", time, comparisons);

        tester.testMsdRadixSortWithQuickSort(part, time, comparisons);
        writeResult(file, arrayType, size, "MsdRadixSortWithQuickSort", time, comparisons);
    }
}

int main() {
    StringGenerator generator;
    StringSortTester tester;

    vector<string> randomArray = generator.generateRandomArray(3000);
    vector<string> reverseSortedArray = generator.generateReverseSortedArray(3000);
    vector<string> nearlySortedArray = generator.generateNearlySortedArray(3000);

    ofstream file("results_a1.csv");

    file << "array_type;size;algorithm;time_microseconds;char_comparisons\n";

    runTests(generator, tester, randomArray, "random", file);
    runTests(generator, tester, reverseSortedArray, "reverse_sorted", file);
    runTests(generator, tester, nearlySortedArray, "nearly_sorted", file);

    file.close();

    cout << "Results saved to results_a1.csv\n";

    return 0;
}
// g++ A1_1.cpp -o A1_1.exe
// ./A1_1.exe