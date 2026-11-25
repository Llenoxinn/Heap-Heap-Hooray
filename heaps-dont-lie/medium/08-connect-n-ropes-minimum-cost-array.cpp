#include <iostream>
#define MAX 100
using namespace std;

class MinHeap {
private:
    long long items[MAX];
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
        items[0] = 0;
        size = 0;
    }

    bool insert(long long x) {
        if (size < MAX - 1) {
            items[++size] = x;
            heapifyUp(size);
            return true;
        }
        return false;
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

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

long long minCostToConnectRopes(int ropes[], int n) {
    if (n <= 1) {
        return 0;
    }

    MinHeap minHeap;
    for (int i = 0; i < n; i++) {
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

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i + 1 < n) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(int ropes[], int n) {
    cout << "  Ropes   : ";
    printArray(ropes, n);
    cout << "\n  Min Cost: " << minCostToConnectRopes(ropes, n) << "\n\n";
}

int main() {
    cout << "Test 1 (Primary Example)\n";
    int r1[] = {4, 3, 2, 6};
    runTest(r1, 4);

    cout << "Test 2 (Sorted 1..5)\n";
    int r2[] = {1, 2, 3, 4, 5};
    runTest(r2, 5);

    cout << "Test 3 (Equal Ropes)\n";
    int r3[] = {5, 5, 5, 5};
    runTest(r3, 4);

    cout << "Test 4 (Single Rope)\n";
    int r4[] = {10};
    runTest(r4, 1);

    cout << "Test 5 (Empty Input)\n";
    int r5[1] = {0};
    runTest(r5, 0);

    return 0;
}
