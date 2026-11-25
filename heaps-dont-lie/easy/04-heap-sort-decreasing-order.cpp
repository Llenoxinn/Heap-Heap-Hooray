#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Restores the Min-Heap property for subtree rooted at index i within heap size n.
void minHeapify(vector<int>& arr, int n, int i) {
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
void buildMinHeap(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        minHeapify(arr, n, i);
    }
}

// Sorts arr in decreasing order in-place using a Min-Heap.
void heapSortDecreasing(vector<int>& arr) {
    int n = (int)arr.size();
    if (n <= 1) return;

    buildMinHeap(arr);

    // Move current minimum (root at index 0) to the end of the unsorted prefix,
    // then restore Min-Heap on the shrunk prefix [0 .. i - 1].
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        minHeapify(arr, i, 0);
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

    heapSortDecreasing(arr);

    cout << "  After : ";
    printArray(arr);
    cout << "\n\n";
}

int main() {
    runTest("Test 1 (Classic Example)", {4, 10, 3, 5, 1});
    runTest("Test 2 (Larger Unsorted Array)", {12, 11, 13, 5, 6, 7});
    runTest("Test 3 (Ascending Input)", {1, 2, 3, 4, 5});
    runTest("Test 4 (Already Decreasing)", {5, 4, 3, 2, 1});
    runTest("Test 5 (Duplicates & Negatives)", {7, -3, 7, 9, 0, -3});
    runTest("Test 6 (Single Element)", {42});
    runTest("Test 7 (Empty Array)", {});

    return 0;
}
