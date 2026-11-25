#include <iostream>
using namespace std;

// Restores the Max-Heap property for subtree rooted at index i within heap size n.
void maxHeapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        maxHeapify(arr, n, largest);
    }
}

// Builds a Max-Heap bottom-up in O(N) time.
void buildMaxHeap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, n, i);
    }
}

// Sorts arr in ascending order in-place using a Max-Heap.
void heapSort(int arr[], int n) {
    if (n <= 1) return;

    buildMaxHeap(arr, n);

    // Move current maximum (root at index 0) to the end of the unsorted prefix,
    // then restore Max-Heap on the shrunk prefix [0 .. i - 1].
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        maxHeapify(arr, i, 0);
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

    heapSort(arr, n);

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

    cout << "Test 3 (Reverse Sorted)\n";
    int t3[] = {5, 4, 3, 2, 1};
    runTest(t3, 5);

    cout << "Test 4 (Already Sorted)\n";
    int t4[] = {1, 2, 3, 4, 5};
    runTest(t4, 5);

    cout << "Test 5 (Duplicates & Negatives)\n";
    int t5[] = {4, -2, 4, 0, -2, 1};
    runTest(t5, 6);

    cout << "Test 6 (Single Element & Empty)\n";
    int t6[] = {9};
    runTest(t6, 1);

    cout << "Test 7 (Empty Array)\n";
    int t7[1] = {0};
    runTest(t7, 0);

    return 0;
}
