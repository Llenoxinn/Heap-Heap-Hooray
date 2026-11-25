#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class MinHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr) {
        int x = items[curr];
        while (curr / 2 > 0 && x < items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        int x = items[curr];
        while (curr * 2 <= size) {
            int s = curr * 2;
            if (s + 1 <= size && items[s] > items[s + 1]) {
                s++;
            }
            if (items[s] < x) {
                items[curr] = items[s];
                curr = s;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MinHeap() {
        items.push_back(0);
        size = 0;
    }

    void insert(int x) {
        items.push_back(x);
        size++;
        heapifyUp(size);
    }

    int getMin() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    int extractMin() {
        if (size == 0) {
            return -1;
        }
        int minVal = items[1];
        items[1] = items[size];
        items.pop_back();
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return minVal;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

void sortNearlySorted(vector<int>& arr, int k) {
    int n = (int)arr.size();
    if (n <= 1 || k <= 0) {
        return;
    }

    MinHeap minHeap;
    int windowSize = min(k + 1, n);

    for (int i = 0; i < windowSize; i++) {
        minHeap.insert(arr[i]);
    }

    int writeIdx = 0;
    for (int i = windowSize; i < n; i++) {
        arr[writeIdx++] = minHeap.extractMin();
        minHeap.insert(arr[i]);
    }

    while (!minHeap.isEmpty()) {
        arr[writeIdx++] = minHeap.extractMin();
    }
}

void printArray(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(string label, vector<int> arr, int k) {
    cout << label << " (k = " << k << ")\n";
    cout << "  Input : ";
    printArray(arr);
    cout << "\n";
    sortNearlySorted(arr, k);
    cout << "  Sorted: ";
    printArray(arr);
    cout << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Example)", {6, 5, 3, 2, 8, 10, 9}, 3);
    runTest("Test 2 (Larger Array)", {10, 9, 8, 7, 4, 70, 60, 50}, 4);
    runTest("Test 3 (Edge Case k = 0)", {1, 2, 3, 4, 5}, 0);
    runTest("Test 4 (Edge Case k >= n)", {5, 1, 4, 2, 3}, 10);
    runTest("Test 5 (Negatives & Duplicates)", {-3, -1, -5, 2, 0, -1}, 2);
    return 0;
}
