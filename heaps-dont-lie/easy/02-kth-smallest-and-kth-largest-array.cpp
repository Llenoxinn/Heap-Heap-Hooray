#include <climits>
#include <iostream>
#define MAX 100
using namespace std;

// 1-indexed fixed-capacity MaxHeap matching Heap-Implementation-OPTIMIZED.cpp.
class MaxHeap {
private:
    int items[MAX];
    int size;

    void heapifyUp(int curr);
    void maxHeapify(int curr);

public:
    MaxHeap();
    bool insert(int x);
    int getMax();
    int extractMax();
    bool isEmpty();
    int getSize();
    void display();
};

MaxHeap::MaxHeap() {
    size = 0;
}

void MaxHeap::heapifyUp(int curr) {
    int x = items[curr];
    while (curr / 2 > 0 && x > items[curr / 2]) {
        items[curr] = items[curr / 2];
        curr = curr / 2;
    }
    items[curr] = x;
}

void MaxHeap::maxHeapify(int curr) {
    int x = items[curr];
    while (curr * 2 <= size) {
        int s = curr * 2;
        if (s + 1 <= size && items[s + 1] > items[s]) {
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

bool MaxHeap::insert(int x) {
    if (size < MAX - 1) {
        items[++size] = x;
        heapifyUp(size);
        return true;
    }
    return false;
}

int MaxHeap::getMax() {
    if (size == 0) return INT_MIN;
    return items[1];
}

int MaxHeap::extractMax() {
    if (size == 0) return INT_MIN;
    int maxVal = items[1];
    items[1] = items[size];
    size--;
    if (size > 0) maxHeapify(1);
    return maxVal;
}

bool MaxHeap::isEmpty() { return size == 0; }
int MaxHeap::getSize() { return size; }

void MaxHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << items[i] << " ";
    }
    cout << endl;
}

// 1-indexed fixed-capacity MinHeap matching Heap-Implementation-OPTIMIZED.cpp.
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
    if (size == 0) return INT_MAX;
    return items[1];
}

int MinHeap::extractMin() {
    if (size == 0) return INT_MAX;
    int minVal = items[1];
    items[1] = items[size];
    size--;
    if (size > 0) minHeapify(1);
    return minVal;
}

bool MinHeap::isEmpty() { return size == 0; }
int MinHeap::getSize() { return size; }

void MinHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << items[i] << " ";
    }
    cout << endl;
}

// K'th Smallest using a MaxHeap of size k:
// keeps the k smallest elements seen so far; root is the k'th smallest.
int kthSmallest(int arr[], int n, int k) {
    if (k < 1 || k > n || k >= MAX) return INT_MIN;

    MaxHeap maxH;
    for (int i = 0; i < n; i++) {
        if (maxH.getSize() < k) {
            if (!maxH.insert(arr[i])) return INT_MIN;
        } else if (arr[i] < maxH.getMax()) {
            maxH.extractMax();
            maxH.insert(arr[i]);
        }
    }
    return maxH.getMax();
}

// K'th Largest using a MinHeap of size k:
// keeps the k largest elements seen so far; root is the k'th largest.
int kthLargest(int arr[], int n, int k) {
    if (k < 1 || k > n || k >= MAX) return INT_MIN;

    MinHeap minH;
    for (int i = 0; i < n; i++) {
        if (minH.getSize() < k) {
            if (!minH.insert(arr[i])) return INT_MIN;
        } else if (arr[i] > minH.getMin()) {
            minH.extractMin();
            minH.insert(arr[i]);
        }
    }
    return minH.getMin();
}

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i + 1 < n) cout << ", ";
    }
    cout << "]";
}

void runTest(int arr[], int n, int k) {
    cout << "  Array        : ";
    printArray(arr, n);
    cout << "\n";

    int small = kthSmallest(arr, n, k);
    int large = kthLargest(arr, n, k);

    if (small == INT_MIN) {
        cout << "  Result       : Invalid k (out of bounds)\n\n";
    } else {
        cout << "  K'th Smallest: " << small << "\n";
        cout << "  K'th Largest : " << large << "\n\n";
    }
}

int main() {
    cout << "Test 1 (Classic Unsorted Array) (k = 3)\n";
    int t1[] = {7, 10, 4, 3, 20, 15};
    runTest(t1, 6, 3);

    cout << "Test 2 (LeetCode 215 Example) (k = 2)\n";
    int t2[] = {3, 2, 1, 5, 6, 4};
    runTest(t2, 6, 2);

    cout << "Test 3 (With Duplicates) (k = 4)\n";
    int t3[] = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    runTest(t3, 9, 4);

    cout << "Test 4 (k = 1 Extremes) (k = 1)\n";
    int t4[] = {12, 5, 787, 1, 23};
    runTest(t4, 5, 1);

    cout << "Test 5 (k = n Extremes) (k = 5)\n";
    int t5[] = {12, 5, 787, 1, 23};
    runTest(t5, 5, 5);

    cout << "Test 6 (Invalid k) (k = 0)\n";
    int t6[] = {4, 1, 9};
    runTest(t6, 3, 0);

    return 0;
}
