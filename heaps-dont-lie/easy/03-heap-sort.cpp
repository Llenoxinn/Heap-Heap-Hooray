#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Restores the Max-Heap property for subtree rooted at index i within heap size n.
void maxHeapify(vector<int>& arr, int n, int i) {
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
void buildMaxHeap(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, n, i);
    }
}

// Sorts arr in ascending order in-place using a Max-Heap.
void heapSort(vector<int>& arr) {
    int n = (int)arr.size();
    if (n <= 1) return;

    buildMaxHeap(arr);

    // Move current maximum (root at index 0) to the end of the unsorted prefix,
    // then restore Max-Heap on the shrunk prefix [0 .. i - 1].
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        maxHeapify(arr, i, 0);
    }
}

void printArray(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) cout << ", ";
    }
    cout << "]";
}

void runTest(string label, vector<int> arr) {
    cout << label << "\n";
    cout << "  Before: ";
    printArray(arr);
    cout << "\n";

    heapSort(arr);

    cout << "  After : ";
    printArray(arr);
    cout << "\n\n";
}

int main() {
    runTest("Test 1 (Classic Example)", {4, 10, 3, 5, 1});
    runTest("Test 2 (Larger Unsorted Array)", {12, 11, 13, 5, 6, 7});
    runTest("Test 3 (Reverse Sorted)", {5, 4, 3, 2, 1});
    runTest("Test 4 (Already Sorted)", {1, 2, 3, 4, 5});
    runTest("Test 5 (Duplicates & Negatives)", {4, -2, 4, 0, -2, 1});
    runTest("Test 6 (Single Element & Empty)", {9});
    runTest("Test 7 (Empty Array)", {});

    return 0;
}
