#include <iostream>
using namespace std;

// Restores the Min-Heap property for subtree rooted at index i within heap size n.
void minHeapify(int arr[], int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] < arr[smallest]) {
        smallest = left;
    }
    if (right < n && arr[right] < arr[smallest]) {
        smallest = right;
    }

    if (smallest != i) {
        int temp = arr[i];
        arr[i] = arr[smallest];
        arr[smallest] = temp;

        minHeapify(arr, n, smallest);
    }
}

// Builds a Min-Heap bottom-up in O(N) time.
void buildMinHeap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        minHeapify(arr, n, i);
    }
}

// Sorts arr in decreasing order in-place using a Min-Heap.
void heapSortDecreasing(int arr[], int n) {
    if (n <= 1) return;

    buildMinHeap(arr, n);

    // Move current minimum (root at index 0) to the end of the unsorted prefix,
    // then restore Min-Heap on the shrunk prefix [0 .. i - 1].
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        minHeapify(arr, i, 0);
    }
}

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i + 1 < n) cout << ", ";
    }
    cout << "]";
}

void runTest(int arr[], int n) {
    cout << "  Before: ";
    printArray(arr, n);
    cout << "\n";

    heapSortDecreasing(arr, n);

    cout << "  After : ";
    printArray(arr, n);
    cout << "\n\n";
}

int main() {
    cout << "Test 1 (Classic Example)\n";
    int t1[] = {4, 10, 3, 5, 1};
    runTest(t1, 5);

    cout << "Test 2 (Larger Unsorted Array)\n";
    int t2[] = {12, 11, 13, 5, 6, 7};
    runTest(t2, 6);

    cout << "Test 3 (Ascending Input)\n";
    int t3[] = {1, 2, 3, 4, 5};
    runTest(t3, 5);

    cout << "Test 4 (Already Decreasing)\n";
    int t4[] = {5, 4, 3, 2, 1};
    runTest(t4, 5);

    cout << "Test 5 (Duplicates & Negatives)\n";
    int t5[] = {7, -3, 7, 9, 0, -3};
    runTest(t5, 6);

    cout << "Test 6 (Single Element)\n";
    int t6[] = {42};
    runTest(t6, 1);

    cout << "Test 7 (Empty Array)\n";
    int t7[1] = {0};
    runTest(t7, 0);

    return 0;
}
