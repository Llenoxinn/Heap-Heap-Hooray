# Nearly Sorted Array (k-Sorted Array)

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Nearly Sorted Algorithm](https://www.geeksforgeeks.org/dsa/nearly-sorted-algorithm/)

---

## Problem Statement

Given an array `arr` of `n` integers where every element is at most `k` positions away from its target position in the fully sorted array (i.e., if an element belongs at index `i` in the sorted order, its current index is in `[i - k, i + k]`), sort the array in ascending order efficiently.

**Example:**
- Input: `arr = [6, 5, 3, 2, 8, 10, 9]`, `k = 3`
- Output: `[2, 3, 5, 6, 8, 9, 10]`

---

## Pattern Recognition & Intuition

- **Why think "Heap"?**
  Think of a **sliding window of uncertainty**. The element that belongs at index `0` of the sorted array *must* be somewhere within the first `k + 1` elements (`arr[0..k]`), because no element further than index `k` can be at most `k` steps away from index `0`. Once you place the smallest element at index `0`, the element for index `1` must be among the remaining elements in the window plus `arr[k + 1]`. Repeatedly pulling the minimum from a moving window of size `k + 1` is the textbook signature of a Heap!
- **Min-Heap vs Max-Heap Decision Rule:**
  We are sorting in **ascending order**, so at every step we need the **smallest** available candidate inside the current window. That calls for a **Min-Heap** of size at most `k + 1`. (If sorting descending, we would use a Max-Heap.)
- **Why it beats naive approaches:**
  - General sorting (`std::sort` / Merge Sort): $O(n \log n)$ — throws away the `k`-sorted guarantee.
  - Insertion Sort: $O(n \cdot k)$ — fast when `k` is tiny, slow when `k` is moderate (e.g., $k = \sqrt{n}$ or $n / 4$).
  - Min-Heap sliding window: $O(n \log k)$ — strictly better than both whenever $1 \ll k \ll n$.
- **Key Invariant:**
  Before extracting the element for sorted position `writeIdx`, the Min-Heap holds the exact `k + 1` unprocessed candidates from `arr[0 .. writeIdx + k]` (or all remaining elements once we pass the end of `arr`). Therefore, `minHeap.getMin()` is guaranteed to be the true global minimum among all remaining elements.
- **Edge Cases:**
  - `k == 0`: Array is already sorted; return immediately in $O(1)$.
  - `k >= n - 1`: `min(k + 1, n)` caps the initial window at `n`, gracefully falling back to standard Heapsort.
  - Empty or 1-element array: Nothing to sort.
  - Negative numbers and duplicates: Heap comparisons work identically.

---

## Step-by-Step Trace

Tracing **Test 1** from `main()`: `arr = [6, 5, 3, 2, 8, 10, 9]`, `n = 7`, `k = 3`.
`windowSize = min(k + 1, n) = 4`.

1. **Seed the Min-Heap with the first `4` elements (`arr[0..3]`):**
   - Insert `6` → Heap: `[6]`
   - Insert `5` → Heap: `[5, 6]`
   - Insert `3` → Heap: `[3, 6, 5]`
   - Insert `2` → Heap: `[2, 3, 5, 6]`

2. **Slide the window (`i = 4` to `6`), extracting min then inserting `arr[i]`:**
   - `i = 4` (`arr[4] = 8`):
     - `extractMin()` = `2` → `arr[0] = 2`. Heap: `[3, 6, 5]`
     - `insert(8)` → Heap: `[3, 6, 5, 8]`
   - `i = 5` (`arr[5] = 10`):
     - `extractMin()` = `3` → `arr[1] = 3`. Heap: `[5, 6, 8]`
     - `insert(10)` → Heap: `[5, 6, 8, 10]`
   - `i = 6` (`arr[6] = 9`):
     - `extractMin()` = `5` → `arr[2] = 5`. Heap: `[6, 10, 8]`
     - `insert(9)` → Heap: `[6, 9, 8, 10]`

3. **Drain remaining elements from the Min-Heap:**
   - `extractMin()` = `6` → `arr[3] = 6`. Heap: `[8, 9, 10]`
   - `extractMin()` = `8` → `arr[4] = 8`. Heap: `[9, 10]`
   - `extractMin()` = `9` → `arr[5] = 9`. Heap: `[10]`
   - `extractMin()` = `10` → `arr[6] = 10`. Heap: `[]`

**Final Sorted Array:** `[2, 3, 5, 6, 8, 9, 10]`

---

## Time & Space Complexity

- **Time Complexity:** $O(n \log k)$
  - Inserting the first `min(k + 1, n)` elements takes $O(k \log k)$.
  - For each of the remaining `n - (k + 1)` elements, one `extractMin` and one `insert` each cost $O(\log k)$.
  - Draining the final `k + 1` elements takes $O(k \log k)$.
  - Total: $O(n \log k)$.
- **Space Complexity:** $O(k)$
  - The Min-Heap never stores more than `k + 1` elements at any point. Sorting happens in-place in `arr`.

---

## C++ Implementation

See [01-nearly-sorted-array.cpp](01-nearly-sorted-array.cpp).

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

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

public:
    MinHeap() {
        items.push_back(0);
        size = 0;
    }

    void insert(int x) {
        items.push_back(x);
        size++;
        heapifyUp(size);
    }

    int getMin() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    int extractMin() {
        if (size == 0) {
            return -1;
        }
        int minVal = items[1];
        items[1] = items[size];
        items.pop_back();
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return minVal;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

void sortNearlySorted(vector<int>& arr, int k) {
    int n = (int)arr.size();
    if (n <= 1 || k <= 0) {
        return;
    }

    MinHeap minHeap;
    int windowSize = min(k + 1, n);

    for (int i = 0; i < windowSize; i++) {
        minHeap.insert(arr[i]);
    }

    int writeIdx = 0;
    for (int i = windowSize; i < n; i++) {
        arr[writeIdx++] = minHeap.extractMin();
        minHeap.insert(arr[i]);
    }

    while (!minHeap.isEmpty()) {
        arr[writeIdx++] = minHeap.extractMin();
    }
}

void printArray(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(string label, vector<int> arr, int k) {
    cout << label << " (k = " << k << ")\n";
    cout << "  Input : ";
    printArray(arr);
    cout << "\n";
    sortNearlySorted(arr, k);
    cout << "  Sorted: ";
    printArray(arr);
    cout << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Example)", {6, 5, 3, 2, 8, 10, 9}, 3);
    runTest("Test 2 (Larger Array)", {10, 9, 8, 7, 4, 70, 60, 50}, 4);
    runTest("Test 3 (Edge Case k = 0)", {1, 2, 3, 4, 5}, 0);
    runTest("Test 4 (Edge Case k >= n)", {5, 1, 4, 2, 3}, 10);
    runTest("Test 5 (Negatives & Duplicates)", {-3, -1, -5, 2, 0, -1}, 2);
    return 0;
}
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the solution to match `Heap-Implementation-OPTIMIZED.cpp` without any STL containers (`std::vector`, `<algorithm>`, `std::string`):
- `MinHeap` stores elements in a 1-indexed fixed-size array `int items[MAX]` (`#define MAX 100`) with `int size` tracking the active heap boundary (`size < MAX - 1` capacity guard on `insert`, plus `display()`).
- `sortNearlySorted(int arr[], int n, int k)` operates directly on a raw `int arr[]` buffer of length `n`, computing `windowSize` with a ternary expression instead of `std::min`.
- `main()` declares fixed C-style test arrays locally with no global variables, `const`, `static`, or `constexpr`.

See [01-nearly-sorted-array-array.cpp](01-nearly-sorted-array-array.cpp).

```cpp
#include <iostream>
#define MAX 100
using namespace std;

class MinHeap {
private:
    int items[MAX];
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

public:
    MinHeap() {
        items[0] = 0;
        size = 0;
    }

    bool insert(int x) {
        if (size < MAX - 1) {
            items[++size] = x;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    int getMin() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    int extractMin() {
        if (size == 0) {
            return -1;
        }
        int minVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return minVal;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

void sortNearlySorted(int arr[], int n, int k) {
    if (n <= 1 || k <= 0) {
        return;
    }

    MinHeap minHeap;
    int windowSize = (k + 1 < n) ? (k + 1) : n;

    for (int i = 0; i < windowSize; i++) {
        minHeap.insert(arr[i]);
    }

    int writeIdx = 0;
    for (int i = windowSize; i < n; i++) {
        arr[writeIdx++] = minHeap.extractMin();
        minHeap.insert(arr[i]);
    }

    while (!minHeap.isEmpty()) {
        arr[writeIdx++] = minHeap.extractMin();
    }
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

void runTest(int arr[], int n, int k) {
    cout << "  Input : ";
    printArray(arr, n);
    cout << "\n";
    sortNearlySorted(arr, n, k);
    cout << "  Sorted: ";
    printArray(arr, n);
    cout << "\n\n";
}

int main() {
    cout << "Test 1 (Primary Example) (k = 3)\n";
    int a1[] = {6, 5, 3, 2, 8, 10, 9};
    runTest(a1, 7, 3);

    cout << "Test 2 (Larger Array) (k = 4)\n";
    int a2[] = {10, 9, 8, 7, 4, 70, 60, 50};
    runTest(a2, 8, 4);

    cout << "Test 3 (Edge Case k = 0) (k = 0)\n";
    int a3[] = {1, 2, 3, 4, 5};
    runTest(a3, 5, 0);

    cout << "Test 4 (Edge Case k >= n) (k = 10)\n";
    int a4[] = {5, 1, 4, 2, 3};
    runTest(a4, 5, 10);

    cout << "Test 5 (Negatives & Duplicates) (k = 2)\n";
    int a5[] = {-3, -1, -5, 2, 0, -1};
    runTest(a5, 6, 2);

    return 0;
}
```

---

## LeetCode Pattern Takeaways

Whenever a problem guarantees **bounded displacement** (`k` steps away from target position, or sliding window of fixed lookahead `k + 1`), use a fixed-size **Min-Heap** (or Max-Heap) of size `k + 1` to stream elements in $O(n \log k)$ instead of paying $O(n \log n)$ for a full sort.

- **LeetCode 23 — Merge k Sorted Lists:** Generalized version where `k` sorted streams feed one Min-Heap of size `k`.
- **LeetCode 378 — Kth Smallest Element in a Sorted Matrix:** Bounded frontier Min-Heap of size `min(n, k)`.
- **LeetCode 1094 / 253 — Car Pooling / Meeting Rooms II:** Sweep-line with a Min-Heap tracking active intervals.
