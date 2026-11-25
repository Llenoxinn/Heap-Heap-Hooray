# 03. Heap Sort (Ascending Order)

- **Difficulty:** Easy
- **Reference:** [GeeksforGeeks — Heap Sort](https://www.geeksforgeeks.org/dsa/heap-sort/)

---

## Problem Statement

Given an unsorted integer array `arr` of size `n`, sort it **in-place in ascending (non-decreasing) order** using the Heap Sort algorithm.

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
Selection Sort finds the maximum in `O(N)` per pass, giving `O(N^2)` total. A **Heap** upgrades that "find max and remove it" step from `O(N)` to `O(log N)` — turning `O(N^2)` into **`O(N log N)` worst-case** while still sorting **in-place**!

### Min-Heap vs Max-Heap Decision Rule
- **Want Ascending Order in-place? Use a `MaxHeap` (`maxHeapify`)!**
  - Why? When you extract the root `arr[0]`, the easiest place to put it without shifting the whole array is the **back** (`arr[n - 1]`, then `arr[n - 2]`, ...).
  - Since the back of an ascending array holds the **largest** elements, we want the **maximum** at `arr[0]` at every step -> **Max-Heap**.
  - (If you used a Min-Heap and placed minimums at the back, you'd end up with *decreasing* order — which is Problem 04!)

### Why this beats naive approaches & other sorts
- **vs. Selection Sort (`O(N^2)`):** Heap Sort is literally "Selection Sort with a Max-Heap".
- **vs. Quick Sort (`O(N^2)` worst-case):** Heap Sort guarantees `O(N log N)` even on adversarial inputs (already sorted, reverse sorted, all duplicates).
- **vs. Merge Sort (`O(N)` extra space):** Heap Sort runs **in-place** without allocating a temporary `O(N)` buffer.
- **vs. Inserting `N` items into a separate heap:** Building the heap bottom-up (`i = n/2 - 1` down to `0`) runs in **`O(N)`** instead of `O(N log N)`, and requires zero extra array allocation!

### Key Invariants & Edge Cases
- **Loop Invariant (Phase 2, before iteration `i`):**
  1. `arr[0 .. i]` is a valid Max-Heap.
  2. `arr[i+1 .. n-1]` is already sorted in its final position, and every element there is `>=` every element in `arr[0 .. i]`.
- **Edge cases:**
  - `n == 0` or `n == 1`: already sorted; return early before `n / 2 - 1` / `n - 1`.
  - Negatives and duplicates: `>` comparisons handle both cleanly. (Note: Heap Sort is **not stable** — equal elements may swap relative order.)

---

## Step-by-Step Trace

Using Test 1 from `main()`: `arr = [4, 10, 3, 5, 1]`, `n = 5`.

### Phase 1: `buildMaxHeap` (`i = n/2 - 1 = 1` down to `0`)

1. **`i = 1` (`arr[1] = 10`):**
   - `left = 3 (5)`, `right = 4 (1)`. `10 >= 5` and `10 >= 1` -> no swap.
   - Array: `[4, 10, 3, 5, 1]`
2. **`i = 0` (`arr[0] = 4`):**
   - `left = 1 (10)`, `right = 2 (3)` -> `largest = 1 (10)`. Swap `arr[0]` and `arr[1]`:
     `[10, 4, 3, 5, 1]`
   - Recurse `maxHeapify(arr, 5, 1)` on `arr[1] = 4`:
     `left = 3 (5)`, `right = 4 (1)` -> `largest = 3 (5)`. Swap `arr[1]` and `arr[3]`:
     `[10, 5, 3, 4, 1]`
   - Recurse `maxHeapify(arr, 5, 3)`: leaf -> done.

Max-Heap built: **`[10, 5, 3, 4, 1]`**.

### Phase 2: Extract Max & Shrink (`i = 4` down to `1`)

| Iteration `i` | Swap `arr[0] <-> arr[i]` | Heapify `maxHeapify(arr, i, 0)` | Array `[heap \| sorted suffix]` |
| --- | --- | --- | --- |
| `i = 4` | `10 <-> 1` -> `[1, 5, 3, 4 \| 10]` | `1` sinks past `5` then `4` | `[5, 4, 3, 1 \| 10]` |
| `i = 3` | `5 <-> 1` -> `[1, 4, 3 \| 5, 10]` | `1` sinks past `4` | `[4, 1, 3 \| 5, 10]` |
| `i = 2` | `4 <-> 3` -> `[3, 1 \| 4, 5, 10]` | `3 > 1`, no swap | `[3, 1 \| 4, 5, 10]` |
| `i = 1` | `3 <-> 1` -> `[1 \| 3, 4, 5, 10]` | heap size `1`, no-op | `[1 \| 3, 4, 5, 10]` |

**Final Sorted Array:** `[1, 3, 4, 5, 10]`.

---

## Time & Space Complexity

- **Time Complexity:**
  - `buildMaxHeap`: **`O(N)`** (tight bound via Floyd's analysis — most nodes are near the bottom and sift down only 0 or 1 levels).
  - Extraction loop: `(N - 1)` calls to `maxHeapify` of cost `O(log N)` each -> **`O(N log N)`**.
  - **Total:** **`O(N log N)`** in best, average, and worst cases.
- **Space Complexity:** **`O(log N)`** auxiliary space from the recursion call stack of `maxHeapify` (or **`O(1)`** if `maxHeapify` is written iteratively with a `while` loop, as in `Heap-Implementation-OPTIMIZED.cpp`).

---

## C++ Implementation

Source file: [03-heap-sort.cpp](03-heap-sort.cpp)

```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Restores the Max-Heap property for subtree rooted at index i within heap size n.
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

// Builds a Max-Heap bottom-up in O(N) time.
void buildMaxHeap(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, n, i);
    }
}

// Sorts arr in ascending order in-place using a Max-Heap.
void heapSort(vector<int>& arr) {
    int n = (int)arr.size();
    if (n <= 1) return;

    buildMaxHeap(arr);

    // Move current maximum (root at index 0) to the end of the unsorted prefix,
    // then restore Max-Heap on the shrunk prefix [0 .. i - 1].
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        maxHeapify(arr, i, 0);
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

    heapSort(arr);

    cout << "  After : ";
    printArray(arr);
    cout << "\n\n";
}

int main() {
    runTest("Test 1 (Classic Example)", {4, 10, 3, 5, 1});
    runTest("Test 2 (Larger Unsorted Array)", {12, 11, 13, 5, 6, 7});
    runTest("Test 3 (Reverse Sorted)", {5, 4, 3, 2, 1});
    runTest("Test 4 (Already Sorted)", {1, 2, 3, 4, 5});
    runTest("Test 5 (Duplicates & Negatives)", {4, -2, 4, 0, -2, 1});
    runTest("Test 6 (Single Element & Empty)", {9});
    runTest("Test 7 (Empty Array)", {});

    return 0;
}
```

---

## LeetCode Pattern Takeaways

Understanding `buildMaxHeap` + in-place extraction powers:
- **LeetCode 912 — Sort an Array** (requires guaranteed `O(N log N)` worst-case time without `std::sort`, where Quicksort without random pivot TLEs on adversarial tests — Heap Sort passes!).
- **Introsort (`std::sort` in C++ STL):** Uses Quicksort by default, and **falls back to Heap Sort** whenever recursion depth exceeds `2 * log2(N)` to guarantee `O(N log N)` worst-case!
- **Partial Heap Sort:** Need only the top `k` largest elements in sorted order? Run `buildMaxHeap` (`O(N)`) and run the extraction loop only `k` times (`O(N + k log N)`).
