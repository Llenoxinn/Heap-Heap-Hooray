#include <iostream>
#include <vector>
using namespace std;

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

void convertMinToMaxHeap(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, n, i);
    }
}

bool isMaxHeap(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i <= n / 2 - 1; i++) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[i] < arr[left]) return false;
        if (right < n && arr[i] < arr[right]) return false;
    }
    return true;
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    vector<int> h1 = {3, 5, 9, 6, 8, 20, 10, 12, 18, 9};
    cout << "Test 1 Min-Heap: ";
    printVector(h1);
    convertMinToMaxHeap(h1);
    cout << "\nTest 1 Max-Heap: ";
    printVector(h1);
    cout << " (valid Max-Heap: " << (isMaxHeap(h1) ? "true" : "false") << ")\n\n";

    vector<int> h2 = {1, 3, 6, 5, 9, 8};
    cout << "Test 2 Min-Heap: ";
    printVector(h2);
    convertMinToMaxHeap(h2);
    cout << "\nTest 2 Max-Heap: ";
    printVector(h2);
    cout << " (valid Max-Heap: " << (isMaxHeap(h2) ? "true" : "false") << ")\n";

    return 0;
}
