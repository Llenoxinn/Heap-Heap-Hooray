#include <climits>
#include <cmath>
#include <iostream>
#define MAX 1024
using namespace std;

// 1-indexed fixed-capacity MinHeap matching Heap-Implementation-OPTIMIZED.cpp.
class MinHeap {
private:
    int items[MAX];
    int size;

    void heapifyUp(int curr);
    void minHeapify(int curr);

public:
    MinHeap();
    bool insert(int x);
    int getMin();
    int extractMin();
    bool isEmpty();
    int getSize();
    void display();
};

MinHeap::MinHeap() {
    size = 0;
}

void MinHeap::heapifyUp(int curr) {
    int x = items[curr];
    while (curr / 2 > 0 && x < items[curr / 2]) {
        items[curr] = items[curr / 2];
        curr = curr / 2;
    }
    items[curr] = x;
}

void MinHeap::minHeapify(int curr) {
    int x = items[curr];
    while (curr * 2 <= size) {
        int s = curr * 2;
        if (s + 1 <= size && items[s] > items[s + 1]) {
            s++;
        }
        if (items[s] < x) {
            items[curr] = items[s];
            curr = s;
        } else {
            break;
        }
    }
    items[curr] = x;
}

bool MinHeap::insert(int x) {
    if (size < MAX - 1) {
        items[++size] = x;
        heapifyUp(size);
        return true;
    }
    return false;
}

int MinHeap::getMin() {
    if (size == 0) return INT_MAX;
    return items[1];
}

int MinHeap::extractMin() {
    if (size == 0) return INT_MAX;
    int minVal = items[1];
    items[1] = items[size];
    size--;
    if (size > 0) minHeapify(1);
    return minVal;
}

bool MinHeap::isEmpty() { return size == 0; }
int MinHeap::getSize() { return size; }

void MinHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << items[i] << " ";
    }
    cout << endl;
}

// Approach 1: Level-by-level simulation using complete binary tree capacities.
// Level 0 holds 1 node, level 1 holds 2, level 2 holds 4, ..., level h holds 2^h.
// Returns edge-based height (0 for N = 1, -1 for N <= 0).
int heightLevelByLevel(int n) {
    if (n <= 0) return -1;

    int height = 0;
    int remaining = n;
    int levelCapacity = 1;

    while (remaining > levelCapacity) {
        remaining -= levelCapacity;
        levelCapacity *= 2;
        height++;
    }
    return height;
}

// Approach 2: Build an actual 1-indexed MinHeap with N nodes and walk from
// the deepest leaf (index N) up to the root (index 1) via parent = curr / 2.
int heightViaHeapParentWalk(int n) {
    if (n <= 0 || n >= MAX) return -1;

    MinHeap h;
    for (int val = n; val >= 1; val--) {
        if (!h.insert(val)) return -1;
    }

    int edges = 0;
    int curr = h.getSize();
    while (curr > 1) {
        curr /= 2;
        edges++;
    }
    return edges;
}

// Approach 3: O(1) closed-form calculation: ceil(log2(N + 1)) - 1 == floor(log2(N)).
int heightFormula(int n) {
    if (n <= 0) return -1;
    return (int)ceil(log2((double)(n + 1))) - 1;
}

void runTest(int n) {
    int hLevel = heightLevelByLevel(n);
    int hWalk = heightViaHeapParentWalk(n);
    int hMath = heightFormula(n);

    cout << "  Level-by-level O(log N): " << hLevel << " edges";
    if (hLevel >= 0) cout << " (" << (hLevel + 1) << " levels)";
    cout << "\n";
    cout << "  Heap parent-walk       : " << hWalk << " edges\n";
    cout << "  O(1) formula           : " << hMath << " edges\n\n";
}

int main() {
    cout << "Test 1 (Single Node) (N = 1)\n";
    runTest(1);

    cout << "Test 2 (2 Nodes) (N = 2)\n";
    runTest(2);

    cout << "Test 3 (Full 2-Level Heap) (N = 3)\n";
    runTest(3);

    cout << "Test 4 (First Node of Level 2) (N = 4)\n";
    runTest(4);

    cout << "Test 5 (Classic Example: N = 6) (N = 6)\n";
    runTest(6);

    cout << "Test 6 (Full 3-Level Heap) (N = 7)\n";
    runTest(7);

    cout << "Test 7 (First Node of Level 3) (N = 8)\n";
    runTest(8);

    cout << "Test 8 (Full 4-Level Heap) (N = 15)\n";
    runTest(15);

    cout << "Test 9 (Large Heap) (N = 1000)\n";
    runTest(1000);

    cout << "Test 10 (Empty Heap) (N = 0)\n";
    runTest(0);

    return 0;
}
