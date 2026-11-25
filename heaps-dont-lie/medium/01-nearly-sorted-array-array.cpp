#include <iostream>
#define MAX 100
using namespace std;

class MinHeap {
private:
    int items[MAX];
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
        items[0] = 0;
        size = 0;
    }

    bool insert(int x) {
        if (size < MAX - 1) {
            items[++size] = x;
            heapifyUp(size);
            return true;
        }
        return false;
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

void sortNearlySorted(int arr[], int n, int k) {
    if (n <= 1 || k <= 0) {
        return;
    }

    MinHeap minHeap;
    int windowSize = (k + 1 < n) ? (k + 1) : n;

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

void runTest(int arr[], int n, int k) {
    cout << "  Input : ";
    printArray(arr, n);
    cout << "\n";
    sortNearlySorted(arr, n, k);
    cout << "  Sorted: ";
    printArray(arr, n);
    cout << "\n\n";
}

int main() {
    cout << "Test 1 (Primary Example) (k = 3)\n";
    int a1[] = {6, 5, 3, 2, 8, 10, 9};
    runTest(a1, 7, 3);

    cout << "Test 2 (Larger Array) (k = 4)\n";
    int a2[] = {10, 9, 8, 7, 4, 70, 60, 50};
    runTest(a2, 8, 4);

    cout << "Test 3 (Edge Case k = 0) (k = 0)\n";
    int a3[] = {1, 2, 3, 4, 5};
    runTest(a3, 5, 0);

    cout << "Test 4 (Edge Case k >= n) (k = 10)\n";
    int a4[] = {5, 1, 4, 2, 3};
    runTest(a4, 5, 10);

    cout << "Test 5 (Negatives & Duplicates) (k = 2)\n";
    int a5[] = {-3, -1, -5, 2, 0, -1};
    runTest(a5, 6, 2);

    return 0;
}
