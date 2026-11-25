#include <iostream>
#include <vector>
using namespace std;

class MinHeap {
private:
    vector<long long> items;
    int size;

    void heapifyUp(int curr) {
        long long x = items[curr];
        while (curr / 2 > 0 && x < items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        long long x = items[curr];
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

    void insert(long long x) {
        items.push_back(x);
        size++;
        heapifyUp(size);
    }

    long long getMin() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    long long extractMin() {
        if (size == 0) {
            return -1;
        }
        long long minVal = items[1];
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

long long minCostToConnectRopes(vector<int>& ropes) {
    if ((int)ropes.size() <= 1) {
        return 0;
    }

    MinHeap minHeap;
    for (int i = 0; i < (int)ropes.size(); i++) {
        minHeap.insert((long long)ropes[i]);
    }

    long long totalCost = 0;
    while (minHeap.getSize() > 1) {
        long long first = minHeap.extractMin();
        long long second = minHeap.extractMin();
        long long merged = first + second;
        totalCost += merged;
        minHeap.insert(merged);
    }

    return totalCost;
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

void runTest(string label, vector<int> ropes) {
    cout << label << "\n";
    cout << "  Ropes   : ";
    printArray(ropes);
    cout << "\n  Min Cost: " << minCostToConnectRopes(ropes) << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Example)", {4, 3, 2, 6});
    runTest("Test 2 (Sorted 1..5)", {1, 2, 3, 4, 5});
    runTest("Test 3 (Equal Ropes)", {5, 5, 5, 5});
    runTest("Test 4 (Single Rope)", {10});
    runTest("Test 5 (Empty Input)", {});
    return 0;
}
