# 01. Check if an Array Represents a Binary Heap

- **Difficulty:** Easy
- **Reference:** [GeeksforGeeks — How to check if a given array represents a Binary Heap?](https://www.geeksforgeeks.org/dsa/how-to-check-if-a-given-array-represents-a-binary-heap/)

---

## Problem Statement

Given an integer array `arr` of size `n` representing the level-order traversal of a complete binary tree (0-indexed), determine whether `arr` satisfies:

1. The **Max-Heap** property (`arr[i] >= arr[2*i + 1]` and `arr[i] >= arr[2*i + 2]` for every valid child),
2. The **Min-Heap** property (`arr[i] <= arr[2*i + 1]` and `arr[i] <= arr[2*i + 2]` for every valid child),
3. **Both** (e.g., all elements equal, single element, or empty), or **Neither**.

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
A binary heap has two defining invariants:
1. **Structural invariant (Shape):** It is a *complete binary tree* stored in level-order in an array. Because *any* contiguous array of length `n` already represents a complete binary tree in level-order, the structural property holds automatically!
2. **Ordering invariant (Heap property):** Every parent must dominate its children (`parent >= child` for Max-Heap, `parent <= child` for Min-Heap).

Therefore, checking if an array is a heap reduces purely to verifying the **parent-child inequality at every internal (non-leaf) node**.

### Min-Heap vs Max-Heap Decision Rule
- Check **Max-Heap**: fail immediately if any parent is strictly smaller than either child (`arr[i] < arr[left]` or `arr[i] < arr[right]`).
- Check **Min-Heap**: fail immediately if any parent is strictly larger than either child (`arr[i] > arr[left]` or `arr[i] > arr[right]`).
- Notice that equality (`==`) is valid for **both** Min-Heap and Max-Heap!

### Why this beats naive approaches
- **Naive approach:** Build a separate heap from `arr` and compare arrays, or verify every node against all descendants (`O(N^2)`).
- **Optimal insight:** The heap property is *transitive*. If every parent satisfies the inequality with its immediate left and right children, it automatically satisfies the inequality with all deeper descendants. Furthermore, leaves (indices `n/2` through `n - 1`) have no children, so we only need to check indices `0` through `n/2 - 1` — just half the array!

### Edge Cases
- **Empty array (`n = 0`) & single element (`n = 1`):** `n / 2 - 1 = -1`, so the loop does not execute and both `isMaxHeap` and `isMinHeap` return `true` (vacuously true).
- **All equal elements (e.g. `[5, 5, 5, 5]`):** Valid **both** as a Min-Heap and as a Max-Heap.
- **Even `n`:** The last internal node `i = n/2 - 1` has a left child (`2*i + 1 = n - 1`) but **no right child** (`2*i + 2 = n`). Always guard with `right < n` before indexing `arr[right]`.

---

## Step-by-Step Trace

### Example 1 (Valid Max-Heap): `arr = [90, 15, 10, 7, 12, 2]`, `n = 6`

Internal nodes are indices `0` to `n/2 - 1 = 2`.

| Step (`i`) | Node `arr[i]` | Left Child (`2*i+1`) | Right Child (`2*i+2`) | Max-Heap Check (`parent >= children`) | Min-Heap Check (`parent <= children`) |
| --- | --- | --- | --- | --- | --- |
| `i = 0` | `90` | `arr[1] = 15` | `arr[2] = 10` | `90 >= 15` and `90 >= 10` (Pass) | `90 <= 15` (Fail -> `isMinHeap = false`) |
| `i = 1` | `15` | `arr[3] = 7` | `arr[4] = 12` | `15 >= 7` and `15 >= 12` (Pass) | *(already false)* |
| `i = 2` | `10` | `arr[5] = 2` | `idx 6 >= 6` (none) | `10 >= 2` (Pass) | *(already false)* |

**Result:** `isMaxHeap = true`, `isMinHeap = false` -> **Max-Heap**.

### Example 3 (Violation at Internal Node): `arr = [90, 15, 10, 7, 12, 11]`, `n = 6`

- `i = 0`: `90 >= 15` and `90 >= 10` (Max-Heap OK; Min-Heap fails at `90 > 15`).
- `i = 1`: `15 >= 7` and `15 >= 12` (Max-Heap OK).
- `i = 2`: `arr[2] = 10`, `left = arr[5] = 11`. Since `10 < 11`, `isMaxHeap` immediately returns `false`.

**Result:** **Neither Min-Heap nor Max-Heap**.

---

## Time & Space Complexity

- **Time Complexity:** `O(N)` — we visit at most `n / 2` internal nodes and perform `O(1)` comparisons per node. (Best case `O(1)` when a violation occurs at the root `i = 0`.)
- **Space Complexity:** `O(1)` auxiliary space — iterative loop uses only a few integer indices.

---

## C++ Implementation

Source file: [01-check-if-array-is-heap.cpp](01-check-if-array-is-heap.cpp)

```cpp
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
```

---

## LeetCode Pattern Takeaways

Recognizing and validating the **level-order parent/child index invariants** (`2*i + 1`, `2*i + 2` for 0-indexed; `2*i`, `2*i + 1` for 1-indexed) and **internal-node boundary (`n/2 - 1`)** shows up directly in:

- **LeetCode 958 — Check Completeness of a Binary Tree** (verifying the structural half of the heap invariant: no gaps in level-order).
- **LeetCode 215 / 703 / 1046** (debugging or implementing custom heapify logic without `priority_queue`).
- **Bottom-up Heap Construction (Floyd's `buildHeap`)** — starts at the exact same last internal node `n/2 - 1` because leaves `n/2 .. n - 1` are already valid 1-node heaps.
