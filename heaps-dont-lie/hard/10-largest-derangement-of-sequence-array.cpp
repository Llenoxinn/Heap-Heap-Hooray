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

void MaxHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << items[i] << " ";
    }
    cout << endl;
}

bool largestDerangement(int seq[], int n, int res[]) {
    if (n <= 1) {
        return false;
    }

    MaxHeap pq;
    for (int i = 0; i < n; i++) {
        pq.insert(seq[i]);
    }

    for (int i = 0; i < n; i++) {
        int d = pq.extractMax();
        if (d != seq[i] || i == n - 1) {
            res[i] = d;
        } else {
            int nextMax = pq.extractMax();
            res[i] = nextMax;
            pq.insert(d);
        }
    }

    // If the final position collided with seq[n - 1], swap the last two elements
    if (res[n - 1] == seq[n - 1]) {
        int temp = res[n - 1];
        res[n - 1] = res[n - 2];
        res[n - 2] = temp;
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

void runTest(int seq[], int n) {
    cout << "Input:  ";
    printArray(seq, n);
    cout << "\n";

    int ans[MAX];
    if (!largestDerangement(seq, n, ans)) {
        cout << "Output: No derangement possible\n\n";
        return;
    }

    cout << "Output: ";
    printArray(ans, n);
    bool valid = true;
    for (int i = 0; i < n; i++) {
        if (ans[i] == seq[i]) {
            valid = false;
        }
    }
    cout << " (valid derangement: " << (valid ? "YES" : "NO") << ")\n\n";
}

int main() {
    int seq1[] = {5, 4, 3, 2, 1};
    cout << "=== Test Case 1: Reverse-sorted sequence (triggers last-two swap) ===\n";
    runTest(seq1, 5);

    int seq2[] = {56, 21, 42, 67, 23, 74};
    cout << "=== Test Case 2: Unsorted distinct sequence ===\n";
    runTest(seq2, 6);

    int seq3[] = {1, 2};
    cout << "=== Test Case 3: Smallest valid sequence ===\n";
    runTest(seq3, 2);

    int seq4[] = {9};
    cout << "=== Test Case 4: Single element (impossible) ===\n";
    runTest(seq4, 1);

    return 0;
}
