#include <iostream>
#define MAX 100
using namespace std;

struct FreqItem {
    int val;
    int freq;
};

bool hasHigherPriority(FreqItem a, FreqItem b) {
    if (a.freq != b.freq) {
        return a.freq > b.freq; // Higher frequency first
    }
    return a.val < b.val; // Tie-breaker: smaller value first
}

void swapInts(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int partitionInts(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swapInts(arr[i], arr[j]);
        }
    }
    swapInts(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortInts(int arr[], int low, int high) {
    if (low < high) {
        int pi = partitionInts(arr, low, high);
        quickSortInts(arr, low, pi - 1);
        quickSortInts(arr, pi + 1, high);
    }
}

class MaxHeap {
private:
    FreqItem items[MAX];
    int size;

    void heapifyUp(int curr) {
        FreqItem x = items[curr];
        while (curr / 2 > 0 && hasHigherPriority(x, items[curr / 2])) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        FreqItem x = items[curr];
        while (curr * 2 <= size) {
            int best = curr * 2;
            if (best + 1 <= size && hasHigherPriority(items[best + 1], items[best])) {
                best++;
            }
            if (hasHigherPriority(items[best], x)) {
                items[curr] = items[best];
                curr = best;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        size = 0;
    }

    bool insert(FreqItem item) {
        if (size < MAX - 1) {
            items[++size] = item;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    FreqItem getMax() {
        if (size == 0) {
            return {0, 0};
        }
        return items[1];
    }

    FreqItem extractMax() {
        if (size == 0) {
            return {0, 0};
        }
        FreqItem top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return top;
    }

    bool remove() {
        if (size == 0) {
            return false;
        }
        extractMax();
        return true;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << "(" << items[i].val << ":" << items[i].freq << ") ";
        }
        cout << endl;
    }
};

void sortByFrequency(int arr[], int n, int result[], int& resultSize) {
    int sorted[MAX];
    for (int i = 0; i < n; i++) {
        sorted[i] = arr[i];
    }
    quickSortInts(sorted, 0, n - 1);

    MaxHeap heap;
    int i = 0;
    while (i < n) {
        int j = i;
        while (j < n && sorted[j] == sorted[i]) {
            j++;
        }
        heap.insert({sorted[i], j - i});
        i = j;
    }

    resultSize = 0;
    while (!heap.isEmpty()) {
        FreqItem top = heap.extractMax();
        for (int c = 0; c < top.freq; c++) {
            result[resultSize++] = top.val;
        }
    }
}

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i + 1 < n) {
            cout << ", ";
        }
    }
    cout << "]\n";
}

int main() {
    int arr1[5] = {5, 5, 4, 6, 4};
    int res1[MAX];
    int resSize1 = 0;
    sortByFrequency(arr1, 5, res1, resSize1);
    cout << "Test 1 ([5, 5, 4, 6, 4]): ";
    printArray(res1, resSize1);

    int arr2[5] = {9, 9, 9, 2, 5};
    int res2[MAX];
    int resSize2 = 0;
    sortByFrequency(arr2, 5, res2, resSize2);
    cout << "Test 2 ([9, 9, 9, 2, 5]): ";
    printArray(res2, resSize2);

    int arr3[8] = {2, 5, 2, 8, 5, 6, 8, 8};
    int res3[MAX];
    int resSize3 = 0;
    sortByFrequency(arr3, 8, res3, resSize3);
    cout << "Test 3 ([2, 5, 2, 8, 5, 6, 8, 8]): ";
    printArray(res3, resSize3);

    return 0;
}
