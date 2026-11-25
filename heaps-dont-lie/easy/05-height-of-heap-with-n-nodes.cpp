#include <climits>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 1-indexed MinHeap class used to verify structural heap height directly.
class MinHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr) {
        int x = items[curr];
        while (curr / 2 > 0 && x < items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        int x = items[curr];
        while (curr * 2 <= size) {
            int child = curr * 2;
            if (child + 1 <= size && items[child + 1] < items[child]) {
                child++;
            }
            if (items[child] < x) {
                items[curr] = items[child];
                curr = child;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MinHeap() {
        items.push_back(0); // sentinel at index 0 for 1-based indexing
        size = 0;
    }

    void insert(int x) {
        size++;
        if (size < (int)items.size()) {
            items[size] = x;
        } else {
            items.push_back(x);
        }
        heapifyUp(size);
    }

    int getMin() {
        if (size == 0) return INT_MAX;
        return items[1];
    }

    int extractMin() {
        if (size == 0) return INT_MAX;
        int minVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) minHeapify(1);
        return minVal;
    }

    bool isEmpty() { return size == 0; }
    int getSize() { return size; }
};

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
    if (n <= 0) return -1;

    MinHeap h;
    for (int val = n; val >= 1; val--) {
        h.insert(val);
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

void runTest(string label, int n) {
    int hLevel = heightLevelByLevel(n);
    int hWalk = heightViaHeapParentWalk(n);
    int hMath = heightFormula(n);

    cout << label << " (N = " << n << ")\n";
    cout << "  Level-by-level O(log N): " << hLevel << " edges";
    if (hLevel >= 0) cout << " (" << (hLevel + 1) << " levels)";
    cout << "\n";
    cout << "  Heap parent-walk       : " << hWalk << " edges\n";
    cout << "  O(1) formula           : " << hMath << " edges\n\n";
}

int main() {
    runTest("Test 1 (Single Node)", 1);
    runTest("Test 2 (2 Nodes)", 2);
    runTest("Test 3 (Full 2-Level Heap)", 3);
    runTest("Test 4 (First Node of Level 2)", 4);
    runTest("Test 5 (Classic Example: N = 6)", 6);
    runTest("Test 6 (Full 3-Level Heap)", 7);
    runTest("Test 7 (First Node of Level 3)", 8);
    runTest("Test 8 (Full 4-Level Heap)", 15);
    runTest("Test 9 (Large Heap)", 1000);
    runTest("Test 10 (Empty Heap)", 0);

    return 0;
}
