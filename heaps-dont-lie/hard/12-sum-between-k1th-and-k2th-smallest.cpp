#include <climits>
#include <iostream>
#include <vector>
using namespace std;

class MinHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr);
    void minHeapify(int curr);

public:
    MinHeap();
    void insert(int x);
    int getMin();
    int extractMin();
    bool isEmpty();
    int getSize();
    void buildFromArray(vector<int>& arr);
};

MinHeap::MinHeap() {
    items.push_back(0);
    size = 0;
}

void MinHeap::heapifyUp(int curr) {
    int x = items[curr];
    while (curr / 2 > 0 && x < items[curr / 2]) {
        items[curr] = items[curr / 2];
        curr = curr / 2;
    }
    items[curr] = x;
}

void MinHeap::minHeapify(int curr) {
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

void MinHeap::insert(int x) {
    items.push_back(x);
    size++;
    heapifyUp(size);
}

int MinHeap::getMin() {
    if (size == 0) {
        return INT_MAX;
    }
    return items[1];
}

int MinHeap::extractMin() {
    if (size == 0) {
        return INT_MAX;
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

bool MinHeap::isEmpty() {
    return size == 0;
}

int MinHeap::getSize() {
    return size;
}

void MinHeap::buildFromArray(vector<int>& arr) {
    items.clear();
    items.push_back(0);
    for (int i = 0; i < (int)arr.size(); i++) {
        items.push_back(arr[i]);
    }
    size = (int)arr.size();
    for (int i = size / 2; i >= 1; i--) {
        minHeapify(i);
    }
}

long long sumBetweenK1AndK2(vector<int>& arr, int k1, int k2) {
    int n = (int)arr.size();
    if (k1 >= k2 - 1 || k1 < 1 || k2 > n) {
        return 0;
    }

    MinHeap pq;
    pq.buildFromArray(arr);

    // Discard the smallest k1 elements
    for (int i = 0; i < k1; i++) {
        pq.extractMin();
    }

    // Sum the next (k2 - k1 - 1) elements strictly between k1-th and k2-th
    long long total = 0;
    int count = k2 - k1 - 1;
    for (int i = 0; i < count; i++) {
        total += pq.extractMin();
    }

    return total;
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(string label, vector<int> arr, int k1, int k2) {
    cout << "=== " << label << " ===\n";
    cout << "Array: ";
    printVector(arr);
    cout << ", k1 = " << k1 << ", k2 = " << k2 << "\n";
    long long ans = sumBetweenK1AndK2(arr, k1, k2);
    cout << "Sum between " << k1 << "th and " << k2 << "th smallest: " << ans << "\n\n";
}

int main() {
    runTest("Test Case 1: Classic 7-element example", {20, 8, 22, 4, 12, 10, 14}, 3, 6);
    runTest("Test Case 2: 6-element example", {10, 2, 50, 12, 48, 13}, 2, 6);
    runTest("Test Case 3: Adjacent ranks (no elements strictly between)", {5, 1, 9, 3, 7}, 2, 3);
    runTest("Test Case 4: Array with duplicates", {4, 2, 4, 1, 6, 4}, 1, 5);
    return 0;
}
