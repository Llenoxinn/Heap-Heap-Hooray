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

class MaxHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr) {
        int x = items[curr];
        while (curr / 2 > 0 && x > items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        int x = items[curr];
        while (curr * 2 <= size) {
            int s = curr * 2;
            if (s + 1 <= size && items[s] < items[s + 1]) {
                s++;
            }
            if (items[s] > x) {
                items[curr] = items[s];
                curr = s;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        items.push_back(0);
        size = 0;
    }

    void insert(int x) {
        items.push_back(x);
        size++;
        heapifyUp(size);
    }

    int getMax() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    int extractMax() {
        if (size == 0) {
            return -1;
        }
        int maxVal = items[1];
        items[1] = items[size];
        items.pop_back();
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return maxVal;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<int> findKLargest(vector<int>& arr, int k) {
    vector<int> result;
    if (k <= 0 || arr.empty()) {
        return result;
    }
    int limit = min(k, (int)arr.size());
    MinHeap minHeap;

    for (int i = 0; i < (int)arr.size(); i++) {
        if (minHeap.getSize() < limit) {
            minHeap.insert(arr[i]);
        } else if (arr[i] > minHeap.getMin()) {
            minHeap.extractMin();
            minHeap.insert(arr[i]);
        }
    }

    while (!minHeap.isEmpty()) {
        result.push_back(minHeap.extractMin());
    }
    reverse(result.begin(), result.end());
    return result;
}

vector<int> findKSmallest(vector<int>& arr, int k) {
    vector<int> result;
    if (k <= 0 || arr.empty()) {
        return result;
    }
    int limit = min(k, (int)arr.size());
    MaxHeap maxHeap;

    for (int i = 0; i < (int)arr.size(); i++) {
        if (maxHeap.getSize() < limit) {
            maxHeap.insert(arr[i]);
        } else if (arr[i] < maxHeap.getMax()) {
            maxHeap.extractMax();
            maxHeap.insert(arr[i]);
        }
    }

    while (!maxHeap.isEmpty()) {
        result.push_back(maxHeap.extractMax());
    }
    reverse(result.begin(), result.end());
    return result;
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
    cout << "  Input     : ";
    printArray(arr);
    cout << "\n";

    vector<int> largest = findKLargest(arr, k);
    vector<int> smallest = findKSmallest(arr, k);

    cout << "  K Largest : ";
    printArray(largest);
    cout << "\n  K Smallest: ";
    printArray(smallest);
    cout << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Example)", {1, 23, 12, 9, 30, 2, 50}, 3);
    runTest("Test 2 (k = 4)", {7, 10, 4, 3, 20, 15}, 4);
    runTest("Test 3 (Duplicates & Negatives)", {-5, 12, -5, 0, 8, 12}, 3);
    runTest("Test 4 (Edge Case k = 1)", {42, 7, 19, -3, 88}, 1);
    runTest("Test 5 (Edge Case k >= n)", {3, 1, 2}, 5);
    return 0;
}
