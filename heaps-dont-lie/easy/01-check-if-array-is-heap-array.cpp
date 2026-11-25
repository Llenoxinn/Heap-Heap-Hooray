#include <iostream>
using namespace std;

// Checks whether a 0-indexed raw array satisfies the Max-Heap property:
// every parent node is >= both of its children.
bool isMaxHeap(int arr[], int n) {
    // Only internal nodes (0 to n/2 - 1) have at least one child.
    for (int i = 0; i <= (n / 2) - 1; i++) {
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

// Checks whether a 0-indexed raw array satisfies the Min-Heap property:
// every parent node is <= both of its children.
bool isMinHeap(int arr[], int n) {
    for (int i = 0; i <= (n / 2) - 1; i++) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[i] > arr[left]) {
            return false;
        }
        if (right < n && arr[i] > arr[right]) {
            return false;
        }
    }
    return true;
}

void printVerdict(bool maxH, bool minH) {
    if (maxH && minH) {
        cout << "Both Min-Heap and Max-Heap";
    } else if (maxH) {
        cout << "Max-Heap";
    } else if (minH) {
        cout << "Min-Heap";
    } else {
        cout << "Neither Min-Heap nor Max-Heap";
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
    bool maxH = isMaxHeap(arr, n);
    bool minH = isMinHeap(arr, n);

    cout << "  Array      : ";
    printArray(arr, n);
    cout << "\n";
    cout << "  isMaxHeap? : " << (maxH ? "true" : "false") << "\n";
    cout << "  isMinHeap? : " << (minH ? "true" : "false") << "\n";
    cout << "  Verdict    : ";
    printVerdict(maxH, minH);
    cout << "\n\n";
}

int main() {
    cout << "Test 1 (Valid Max-Heap)\n";
    int t1[] = {90, 15, 10, 7, 12, 2};
    runTest(t1, 6);

    cout << "Test 2 (Valid Min-Heap)\n";
    int t2[] = {1, 3, 6, 5, 9, 8};
    runTest(t2, 6);

    cout << "Test 3 (Violation at Internal Node)\n";
    int t3[] = {90, 15, 10, 7, 12, 11};
    runTest(t3, 6);

    cout << "Test 4 (All Equal Elements)\n";
    int t4[] = {5, 5, 5, 5};
    runTest(t4, 4);

    cout << "Test 5 (Single Element)\n";
    int t5[] = {42};
    runTest(t5, 1);

    cout << "Test 6 (Empty Array)\n";
    int t6[1] = {0};
    runTest(t6, 0);

    return 0;
}
