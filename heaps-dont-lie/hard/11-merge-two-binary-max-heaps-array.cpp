#include <climits>
#include <iostream>
#define MAX 100
using namespace std;

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
    void buildFromArray(int arr[], int n);
    int copyToArray(int out[]);
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
        if (s + 1 <= size && items[s] < items[s + 1]) {
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
    if (size == 0) {
        return INT_MIN;
    }
    return items[1];
}

int MaxHeap::extractMax() {
    if (size == 0) {
        return INT_MIN;
    }
    int maxVal = items[1];
    items[1] = items[size];
    size--;
    if (size > 0) {
        maxHeapify(1);
    }
    return maxVal;
}

bool MaxHeap::isEmpty() {
    return size == 0;
}

int MaxHeap::getSize() {
    return size;
}

void MaxHeap::buildFromArray(int arr[], int n) {
    if (n > MAX - 1) {
        n = MAX - 1;
    }
    for (int i = 0; i < n; i++) {
        items[i + 1] = arr[i];
    }
    size = n;
    for (int i = size / 2; i >= 1; i--) {
        maxHeapify(i);
    }
}

int MaxHeap::copyToArray(int out[]) {
    for (int i = 0; i < size; i++) {
        out[i] = items[i + 1];
    }
    return size;
}

void MaxHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << items[i] << " ";
    }
    cout << endl;
}

int mergeTwoMaxHeaps(int a[], int na, int b[], int nb, int merged[]) {
    int combined[MAX];
    int total = 0;
    for (int i = 0; i < na && total < MAX - 1; i++) {
        combined[total++] = a[i];
    }
    for (int i = 0; i < nb && total < MAX - 1; i++) {
        combined[total++] = b[i];
    }

    MaxHeap mergedHeap;
    mergedHeap.buildFromArray(combined, total);
    return mergedHeap.copyToArray(merged);
}

bool isValidMaxHeap(int arr[], int n) {
    for (int i = 0; i <= n / 2 - 1; i++) {
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

void runTest(int a[], int na, int b[], int nb) {
    cout << "Heap A: ";
    printArray(a, na);
    cout << "\nHeap B: ";
    printArray(b, nb);
    cout << "\n";

    int merged[MAX];
    int nm = mergeTwoMaxHeaps(a, na, b, nb, merged);
    cout << "Merged MaxHeap: ";
    printArray(merged, nm);
    cout << " (valid max-heap: " << (isValidMaxHeap(merged, nm) ? "YES" : "NO") << ")\n";

    MaxHeap drain;
    drain.buildFromArray(merged, nm);
    cout << "Extracted descending:";
    while (!drain.isEmpty()) {
        cout << " " << drain.extractMax();
    }
    cout << "\n\n";
}

int main() {
    int a1[] = {10, 5, 6, 2};
    int b1[] = {12, 7, 9};
    cout << "=== Test Case 1: Classic Two Max Heaps ===\n";
    runTest(a1, 4, b1, 3);

    int a2[] = {8, 4, 8, 1};
    int b2[] = {8, 3, 2};
    cout << "=== Test Case 2: Duplicates Across Heaps ===\n";
    runTest(a2, 4, b2, 3);

    int a3[1] = {0};
    int b3[] = {20, 15, 10, 5};
    cout << "=== Test Case 3: One Empty Heap ===\n";
    runTest(a3, 0, b3, 4);

    return 0;
}
