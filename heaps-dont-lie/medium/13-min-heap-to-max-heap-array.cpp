#include <iostream>
#define MAX 100
using namespace std;

void maxHeapify(int arr[], int n, int i) {
    int x = arr[i];
    int curr = i;
    while (2 * curr + 1 < n) {
        int child = 2 * curr + 1;
        if (child + 1 < n && arr[child + 1] > arr[child]) {
            child++;
        }
        if (arr[child] > x) {
            arr[curr] = arr[child];
            curr = child;
        } else {
            break;
        }
    }
    arr[curr] = x;
}

void convertMinToMaxHeap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, n, i);
    }
}

bool isMaxHeap(int arr[], int n) {
    for (int i = 0; i <= n / 2 - 1; i++) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[i] < arr[left]) return false;
        if (right < n && arr[i] < arr[right]) return false;
    }
    return true;
}

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    int h1[MAX] = {3, 5, 9, 6, 8, 20, 10, 12, 18, 9};
    int n1 = 10;
    cout << "Test 1 Min-Heap: ";
    printArray(h1, n1);
    convertMinToMaxHeap(h1, n1);
    cout << "\nTest 1 Max-Heap: ";
    printArray(h1, n1);
    cout << " (valid Max-Heap: " << (isMaxHeap(h1, n1) ? "true" : "false") << ")\n\n";

    int h2[MAX] = {1, 3, 6, 5, 9, 8};
    int n2 = 6;
    cout << "Test 2 Min-Heap: ";
    printArray(h2, n2);
    convertMinToMaxHeap(h2, n2);
    cout << "\nTest 2 Max-Heap: ";
    printArray(h2, n2);
    cout << " (valid Max-Heap: " << (isMaxHeap(h2, n2) ? "true" : "false") << ")\n";

    return 0;
}
