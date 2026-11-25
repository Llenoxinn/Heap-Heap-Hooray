#include <climits>
#include <iostream>
#define MAX 100
using namespace std;

class MinHeap {
private:
    int items[MAX];
    int size;

    void heapifyUp(int curr);
    void minHeapify(int curr);

public:
    MinHeap();
    bool insert(int x);
    int getMin();
    int extractMin();
    bool isEmpty();
    int getSize();
    void buildFromArray(int arr[], int n);
    void display();
};

MinHeap::MinHeap() {
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

bool MinHeap::insert(int x) {
    if (size < MAX - 1) {
        items[++size] = x;
        heapifyUp(size);
        return true;
    }
    return false;
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

void MinHeap::buildFromArray(int arr[], int n) {
    if (n > MAX - 1) {
        n = MAX - 1;
    }
    for (int i = 0; i < n; i++) {
        items[i + 1] = arr[i];
    }
    size = n;
    for (int i = size / 2; i >= 1; i--) {
        minHeapify(i);
    }
}

void MinHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << items[i] << " ";
    }
    cout << endl;
}

long long sumBetweenK1AndK2(int arr[], int n, int k1, int k2) {
    if (k1 >= k2 - 1 || k1 < 1 || k2 > n) {
        return 0;
    }

    MinHeap pq;
    pq.buildFromArray(arr, n);

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

void runTest(int arr[], int n, int k1, int k2) {
    cout << "Array: ";
    printArray(arr, n);
    cout << ", k1 = " << k1 << ", k2 = " << k2 << "\n";
    long long ans = sumBetweenK1AndK2(arr, n, k1, k2);
    cout << "Sum between " << k1 << "th and " << k2 << "th smallest: " << ans << "\n\n";
}

int main() {
    int arr1[] = {20, 8, 22, 4, 12, 10, 14};
    cout << "=== Test Case 1: Classic 7-element example ===\n";
    runTest(arr1, 7, 3, 6);

    int arr2[] = {10, 2, 50, 12, 48, 13};
    cout << "=== Test Case 2: 6-element example ===\n";
    runTest(arr2, 6, 2, 6);

    int arr3[] = {5, 1, 9, 3, 7};
    cout << "=== Test Case 3: Adjacent ranks (no elements strictly between) ===\n";
    runTest(arr3, 5, 2, 3);

    int arr4[] = {4, 2, 4, 1, 6, 4};
    cout << "=== Test Case 4: Array with duplicates ===\n";
    runTest(arr4, 6, 1, 5);

    return 0;
}
