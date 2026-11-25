# 04. Heap Sort for Decreasing Order (Using Min-Heap)

- **Difficulty:** Easy
- **Reference:** [GeeksforGeeks — Heap Sort for Decreasing Order Using Min Heap](https://www.geeksforgeeks.org/dsa/heap-sort-for-decreasing-order-using-min-heap/)

---

## Problem Statement

Given an unsorted integer array `arr` of size `n`, sort it **in-place in decreasing (non-increasing) order** using a **Min-Heap** (`minHeapify`).

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
Just like standard ascending Heap Sort, we want guaranteed `O(N log N)` worst-case time and in-place sorting without allocating a second array.

### Min-Heap vs Max-Heap Decision Rule
- **Ascending Order -> `MaxHeap`:** puts largest elements at the back.
- **Decreasing Order -> `MinHeap` (`minHeapify`):** puts **smallest** elements at the back!
  - Because `arr[0]` always holds the minimum of the remaining unsorted prefix `arr[0 .. i]`, swapping `arr[0]` with `arr[i]` places the smallest remaining element at index `i`.
  - As `i` marches backward from `n - 1` down to `1`, the suffix fills up from smallest to larger (`..., third-smallest, second-smallest, smallest`), leaving the entire array sorted from **largest to smallest**!

### Common Bug Watchout (`heapSort.cpp` Lesson!)
When converting `maxHeapify` to `minHeapify`, it is very easy to rename `largest` to `smallest` and **forget to flip `>` to `<`** in the child comparisons:
- `maxHeapify`: `if (left < n && arr[left] > arr[largest])`
- `minHeapify`: `if (left < n && arr[left] < arr[smallest])`  <-- **must be `<`**!

### Key Invariants & Edge Cases
- **Loop Invariant (Phase 2, before iteration `i`):**
  1. `arr[0 .. i]` is a valid Min-Heap.
  2. `arr[i+1 .. n-1]` is sorted in decreasing order, and every element there is `<=` every element in `arr[0 .. i]`.
- **Edge cases:**
  - `n <= 1`: return early so `n / 2 - 1` and `n - 1` never go negative on empty arrays.
  - Negatives and duplicates: `<` handles both naturally.

---

## Step-by-Step Trace

Using Test 1 from `main()`: `arr = [4, 10, 3, 5, 1]`, `n = 5` (same input as Problem 03 so you can compare side by side!).

### Phase 1: `buildMinHeap` (`i = n/2 - 1 = 1` down to `0`)

1. **`i = 1` (`arr[1] = 10`):**
   - `left = 3 (5)`, `right = 4 (1)` -> `smallest = 4 (1)`. Swap `arr[1]` and `arr[4]`:
     `[4, 1, 3, 5, 10]`
   - Recurse `minHeapify(arr, 5, 4)`: leaf -> done.
2. **`i = 0` (`arr[0] = 4`):**
   - `left = 1 (1)`, `right = 2 (3)` -> `smallest = 1 (1)`. Swap `arr[0]` and `arr[1]`:
     `[1, 4, 3, 5, 10]`
   - Recurse `minHeapify(arr, 5, 1)` on `arr[1] = 4`:
     `left = 3 (5)`, `right = 4 (10)`. `4 < 5` and `4 < 10` -> `smallest = 1`, no swap!

Min-Heap built: **`[1, 4, 3, 5, 10]`**.

### Phase 2: Extract Min & Shrink (`i = 4` down to `1`)

| Iteration `i` | Swap `arr[0] <-> arr[i]` | Heapify `minHeapify(arr, i, 0)` | Array `[heap \| sorted suffix]` |
| --- | --- | --- | --- |
| `i = 4` | `1 <-> 10` -> `[10, 4, 3, 5 \| 1]` | `10` swaps with smaller child `3` | `[3, 4, 10, 5 \| 1]` |
| `i = 3` | `3 <-> 5` -> `[5, 4, 10 \| 3, 1]` | `5` swaps with smaller child `4` | `[4, 5, 10 \| 3, 1]` |
| `i = 2` | `4 <-> 10` -> `[10, 5 \| 4, 3, 1]` | `10` swaps with `5` | `[5, 10 \| 4, 3, 1]` |
| `i = 1` | `5 <-> 10` -> `[10 \| 5, 4, 3, 1]` | heap size `1`, no-op | `[10 \| 5, 4, 3, 1]` |

**Final Sorted Array:** `[10, 5, 4, 3, 1]`.

---

## Time & Space Complexity

- **Time Complexity:** **`O(N log N)`** (`O(N)` for `buildMinHeap` + `(N - 1) * O(log N)` for extractions) in best, average, and worst cases.
- **Space Complexity:** **`O(log N)`** auxiliary space from the recursive `minHeapify` call stack (or **`O(1)`** with an iterative `minHeapify`).

---

## C++ Implementation

Source file: [04-heap-sort-decreasing-order.cpp](04-heap-sort-decreasing-order.cpp)

```cpp
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
```

---

## LeetCode Pattern Takeaways

Flipping `maxHeapify` <-> `minHeapify` is the exact mechanic behind custom comparators in heap problems:
- **LeetCode 506 — Relative Ranks** (ranking elements in descending order).
- **LeetCode 1337 / 1338 / 1464** (descending order selection / top-down extraction).
- **General Rule:** Whenever you want the *opposite* ordering at the back of an array (or the opposite eviction policy at the root), flip the comparison sign inside `heapify` (`<` vs `>`).
