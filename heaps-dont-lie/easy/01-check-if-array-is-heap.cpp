#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Checks whether a 0-indexed array satisfies the Max-Heap property:
// every parent node is >= both of its children.
bool isMaxHeap(vector<int>& arr) {
    int n = (int)arr.size();

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

// Checks whether a 0-indexed array satisfies the Min-Heap property:
// every parent node is <= both of its children.
bool isMinHeap(vector<int>& arr) {
    int n = (int)arr.size();

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

string classifyHeap(vector<int>& arr) {
    bool maxH = isMaxHeap(arr);
    bool minH = isMinHeap(arr);

    if (maxH && minH) return "Both Min-Heap and Max-Heap";
    if (maxH) return "Max-Heap";
    if (minH) return "Min-Heap";
    return "Neither Min-Heap nor Max-Heap";
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
    cout << "  Array      : ";
    printArray(arr);
    cout << "\n";
    cout << "  isMaxHeap? : " << (isMaxHeap(arr) ? "true" : "false") << "\n";
    cout << "  isMinHeap? : " << (isMinHeap(arr) ? "true" : "false") << "\n";
    cout << "  Verdict    : " << classifyHeap(arr) << "\n\n";
}

int main() {
    runTest("Test 1 (Valid Max-Heap)", {90, 15, 10, 7, 12, 2});
    runTest("Test 2 (Valid Min-Heap)", {1, 3, 6, 5, 9, 8});
    runTest("Test 3 (Violation at Internal Node)", {90, 15, 10, 7, 12, 11});
    runTest("Test 4 (All Equal Elements)", {5, 5, 5, 5});
    runTest("Test 5 (Single Element)", {42});
    runTest("Test 6 (Empty Array)", {});

    return 0;
}
