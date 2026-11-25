#include <climits>
#include <iostream>
#include <vector>
using namespace std;

class MaxHeap {
private:
    vector<int> items;

    void heapifyUp(int curr);
    void maxHeapify(int curr);

public:
    MaxHeap();
    void insert(int x);
    int getMax();
    int extractMax();
    bool isEmpty();
    int getSize();
    void buildFromArray(vector<int>& arr);
    vector<int> getArray();
};

MaxHeap::MaxHeap() {
}

void MaxHeap::heapifyUp(int curr) {
    int x = items[curr];
    while (curr > 0) {
        int parent = (curr - 1) / 2;
        if (x > items[parent]) {
            items[curr] = items[parent];
            curr = parent;
        } else {
            break;
        }
    }
    items[curr] = x;
}

void MaxHeap::maxHeapify(int curr) {
    int n = (int)items.size();
    int x = items[curr];
    while (2 * curr + 1 < n) {
        int largest = 2 * curr + 1;
        int right = largest + 1;
        if (right < n && items[right] > items[largest]) {
            largest = right;
        }
        if (items[largest] > x) {
            items[curr] = items[largest];
            curr = largest;
        } else {
            break;
        }
    }
    items[curr] = x;
}

void MaxHeap::insert(int x) {
    items.push_back(x);
    heapifyUp((int)items.size() - 1);
}

int MaxHeap::getMax() {
    if (items.empty()) {
        return INT_MIN;
    }
    return items[0];
}

int MaxHeap::extractMax() {
    if (items.empty()) {
        return INT_MIN;
    }
    int maxVal = items[0];
    items[0] = items[(int)items.size() - 1];
    items.pop_back();
    if (!items.empty()) {
        maxHeapify(0);
    }
    return maxVal;
}

bool MaxHeap::isEmpty() {
    return items.empty();
}

int MaxHeap::getSize() {
    return (int)items.size();
}

void MaxHeap::buildFromArray(vector<int>& arr) {
    items = arr;
    int n = (int)items.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(i);
    }
}

vector<int> MaxHeap::getArray() {
    return items;
}

vector<int> mergeTwoMaxHeaps(vector<int>& a, vector<int>& b) {
    vector<int> combined;
    combined.reserve(a.size() + b.size());
    for (int i = 0; i < (int)a.size(); i++) {
        combined.push_back(a[i]);
    }
    for (int i = 0; i < (int)b.size(); i++) {
        combined.push_back(b[i]);
    }

    MaxHeap mergedHeap;
    mergedHeap.buildFromArray(combined);
    return mergedHeap.getArray();
}

bool isValidMaxHeap(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i <= n / 2 - 1; i++) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[i] < arr[left]) {
            return false;
        }
        if (right < n && arr[i] < arr[right]) {
            return false;
        }
    }
    return true;
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

void runTest(string label, vector<int> a, vector<int> b) {
    cout << "=== " << label << " ===\n";
    cout << "Heap A: ";
    printVector(a);
    cout << "\nHeap B: ";
    printVector(b);
    cout << "\n";

    vector<int> merged = mergeTwoMaxHeaps(a, b);
    cout << "Merged MaxHeap: ";
    printVector(merged);
    cout << " (valid max-heap: " << (isValidMaxHeap(merged) ? "YES" : "NO") << ")\n";

    MaxHeap drain;
    drain.buildFromArray(merged);
    cout << "Extracted descending:";
    while (!drain.isEmpty()) {
        cout << " " << drain.extractMax();
    }
    cout << "\n\n";
}

int main() {
    runTest("Test Case 1: Classic Two Max Heaps", {10, 5, 6, 2}, {12, 7, 9});
    runTest("Test Case 2: Duplicates Across Heaps", {8, 4, 8, 1}, {8, 3, 2});
    runTest("Test Case 3: One Empty Heap", {}, {20, 15, 10, 5});
    return 0;
}
