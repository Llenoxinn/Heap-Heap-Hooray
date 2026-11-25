# 02. K'th Smallest and K'th Largest Element in an Unsorted Array

- **Difficulty:** Easy
- **References:**
  - [GeeksforGeeks — K'th Smallest Element in Unsorted Array](https://www.geeksforgeeks.org/dsa/kth-smallest-largest-element-in-unsorted-array/)
  - [GeeksforGeeks — K'th Largest Element in an Array](https://www.geeksforgeeks.org/dsa/kth-largest-element-in-an-array/)

---

## Problem Statement

Given an unsorted integer array `arr` of size `n` and an integer `k` (`1 <= k <= n`):
1. Find the **k'th smallest** element in `arr` (1-indexed, i.e., `k = 1` is the minimum).
2. Find the **k'th largest** element in `arr` (1-indexed, i.e., `k = 1` is the maximum).

Solve **both** using a size-`k` heap built from scratch (no full sort).

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
Whenever a problem asks for the **Top-K**, **K'th smallest**, or **K'th largest** elements from an array or stream, you do *not* need the total sorted order of all `N` elements — you only need to track the `k` best candidates seen so far and quickly evict the worst candidate among those `k`.

### Min-Heap vs Max-Heap Decision Rule (The "Opposite Heap" Trick!)
This is the #1 insight students confuse at first, so lock it in:
- **Want the K'th Smallest? Use a `MaxHeap` of size `k`!**
  - Why? We want to keep the **`k` smallest** elements seen so far. Whenever a new element arrives and our bag already has `k` items, who gets kicked out? The **largest** one in our bag of `k`! A `MaxHeap` gives us that largest candidate at `getMax()` in `O(1)` and evicts it in `O(log k)`.
  - At the end, the heap holds the `k` smallest elements, and its root (`getMax()`) is the largest of those `k` — which is by definition the **k'th smallest** element!
- **Want the K'th Largest? Use a `MinHeap` of size `k`!**
  - Why? We want to keep the **`k` largest** elements seen so far. Whenever a new element arrives and our bag has `k` items, who gets kicked out? The **smallest** one in our bag of `k`! A `MinHeap` exposes that gatekeeper at `getMin()` in `O(1)` and evicts it in `O(log k)`.
  - At the end, the heap holds the `k` largest elements, and its root (`getMin()`) is the smallest of those `k` — the **k'th largest** element!

### Why this beats naive approaches
- **Sorting (`O(N log N)` time, `O(N)` or `O(log N)` space):** Wastes work sorting the other `N - k` elements you don't care about, and cannot handle streaming data without re-sorting.
- **Size-`N` Heap (`O(N + k log N)`):** Uses `O(N)` space to store everything.
- **Size-`k` Heap (`O(N log k)` time, `O(k)` space):** When `k << N` (e.g., top 10 out of 10 million), `O(k)` memory is tiny, `log k` is fast, and it works naturally on **infinite streams**.

### Key Invariants & Edge Cases
- **Invariant for `kthSmallest`:** After processing `i >= k` elements, `MaxHeap` has size `k` and contains the `k` smallest elements of `arr[0..i]`.
- **Invariant for `kthLargest`:** After processing `i >= k` elements, `MinHeap` has size `k` and contains the `k` largest elements of `arr[0..i]`.
- **Edge cases:**
  - `k = 1`: heap size is 1 (just tracks current min or max).
  - `k = n`: heap holds all `n` elements; root is max (for `kthSmallest`) or min (for `kthLargest`).
  - Duplicates: problem asks for k'th element in sorted order *with duplicates counted* (standard LeetCode 215 semantics), which the size-`k` heap handles automatically.
  - Invalid `k` (`k < 1` or `k > n`): guard early and return sentinel `INT_MIN`.

---

## Step-by-Step Trace

Using Test 1 from `main()`: `arr = [7, 10, 4, 3, 20, 15]`, `k = 3`.

### Part A: K'th Smallest (`k = 3`) with `MaxHeap` (1-indexed)

| Step | Element `x` | Action | `MaxHeap` state (`items[1..size]`) | `getMax()` |
| --- | --- | --- | --- | --- |
| `i = 0` | `7` | `size < 3` -> `insert(7)` | `[7]` | `7` |
| `i = 1` | `10` | `size < 3` -> `insert(10)` | `[10, 7]` | `10` |
| `i = 2` | `4` | `size < 3` -> `insert(4)` | `[10, 7, 4]` | `10` |
| `i = 3` | `3` | `3 < 10` -> `extractMax()`, `insert(3)` | `[7, 4, 3]` | `7` |
| `i = 4` | `20` | `20 < 7` is false -> skip | `[7, 4, 3]` | `7` |
| `i = 5` | `15` | `15 < 7` is false -> skip | `[7, 4, 3]` | `7` |

**Result:** `maxH.getMax() = 7`. (Sorted array is `[3, 4, 7, 10, 15, 20]` -> 3rd smallest is `7`!)

### Part B: K'th Largest (`k = 3`) with `MinHeap` (1-indexed)

| Step | Element `x` | Action | `MinHeap` state (`items[1..size]`) | `getMin()` |
| --- | --- | --- | --- | --- |
| `i = 0` | `7` | `size < 3` -> `insert(7)` | `[7]` | `7` |
| `i = 1` | `10` | `size < 3` -> `insert(10)` | `[7, 10]` | `7` |
| `i = 2` | `4` | `size < 3` -> `insert(4)` | `[4, 10, 7]` | `4` |
| `i = 3` | `3` | `3 > 4` is false -> skip | `[4, 10, 7]` | `4` |
| `i = 4` | `20` | `20 > 4` -> `extractMin()`, `insert(20)` | `[7, 10, 20]` | `7` |
| `i = 5` | `15` | `15 > 7` -> `extractMin()`, `insert(15)` | `[10, 20, 15]` | `10` |

**Result:** `minH.getMin() = 10`. (Sorted array is `[3, 4, 7, 10, 15, 20]` -> 3rd largest is `10`!)

---

## Time & Space Complexity

- **Time Complexity:** `O(N log k)` — each of the `N` elements causes at most one `extract` (`O(log k)`) and one `insert` (`O(log k)`), while the heap never exceeds size `k`.
- **Space Complexity:** `O(k)` — only `k` elements are stored in the heap at any moment.

*(Bonus note: Quickselect achieves `O(N)` average time / `O(N^2)` worst case and doesn't work on streams; the size-`k` heap guarantees `O(N log k)` worst-case time and works on streams!)*

---

## C++ Implementation

Source file: [02-kth-smallest-and-kth-largest.cpp](02-kth-smallest-and-kth-largest.cpp)

```cpp
#include <climits>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 1-indexed MaxHeap class built from scratch.
class MaxHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr) {
        int x = items[curr];
        while (curr / 2 > 0 && x > items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        int x = items[curr];
        while (curr * 2 <= size) {
            int child = curr * 2;
            if (child + 1 <= size && items[child + 1] > items[child]) {
                child++;
            }
            if (items[child] > x) {
                items[curr] = items[child];
                curr = child;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
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

    int getMax() {
        if (size == 0) return INT_MIN;
        return items[1];
    }

    int extractMax() {
        if (size == 0) return INT_MIN;
        int maxVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) maxHeapify(1);
        return maxVal;
    }

    bool isEmpty() { return size == 0; }
    int getSize() { return size; }
};

// 1-indexed MinHeap class built from scratch.
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

// K'th Smallest using a MaxHeap of size k:
// keeps the k smallest elements seen so far; root is the k'th smallest.
int kthSmallest(vector<int>& arr, int k) {
    int n = (int)arr.size();
    if (k < 1 || k > n) return INT_MIN;

    MaxHeap maxH;
    for (int i = 0; i < n; i++) {
        if (maxH.getSize() < k) {
            maxH.insert(arr[i]);
        } else if (arr[i] < maxH.getMax()) {
            maxH.extractMax();
            maxH.insert(arr[i]);
        }
    }
    return maxH.getMax();
}

// K'th Largest using a MinHeap of size k:
// keeps the k largest elements seen so far; root is the k'th largest.
int kthLargest(vector<int>& arr, int k) {
    int n = (int)arr.size();
    if (k < 1 || k > n) return INT_MIN;

    MinHeap minH;
    for (int i = 0; i < n; i++) {
        if (minH.getSize() < k) {
            minH.insert(arr[i]);
        } else if (arr[i] > minH.getMin()) {
            minH.extractMin();
            minH.insert(arr[i]);
        }
    }
    return minH.getMin();
}

void printArray(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) cout << ", ";
    }
    cout << "]";
}

void runTest(string label, vector<int> arr, int k) {
    cout << label << " (k = " << k << ")\n";
    cout << "  Array        : ";
    printArray(arr);
    cout << "\n";

    int small = kthSmallest(arr, k);
    int large = kthLargest(arr, k);

    if (small == INT_MIN) {
        cout << "  Result       : Invalid k (out of bounds)\n\n";
    } else {
        cout << "  K'th Smallest: " << small << "\n";
        cout << "  K'th Largest : " << large << "\n\n";
    }
}

int main() {
    runTest("Test 1 (Classic Unsorted Array)", {7, 10, 4, 3, 20, 15}, 3);
    runTest("Test 2 (LeetCode 215 Example)", {3, 2, 1, 5, 6, 4}, 2);
    runTest("Test 3 (With Duplicates)", {3, 2, 3, 1, 2, 4, 5, 5, 6}, 4);
    runTest("Test 4 (k = 1 Extremes)", {12, 5, 787, 1, 23}, 1);
    runTest("Test 5 (k = n Extremes)", {12, 5, 787, 1, 23}, 5);
    runTest("Test 6 (Invalid k)", {4, 1, 9}, 0);

    return 0;
}
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version removes `<vector>` and `<string>` completely and directly mirrors `Heap-Implementation-OPTIMIZED.cpp` and `heapSort.cpp`:
- Both `MaxHeap` and `MinHeap` store elements in a fixed-capacity 1-indexed array `int items[MAX]` (`#define MAX 100`) alongside `int size`, exposing `heapifyUp`, `maxHeapify`/`minHeapify`, `insert`, `getMax`/`getMin`, `extractMax`/`extractMin`, `isEmpty`, `getSize`, and `display`.
- `insert(int x)` enforces a manual capacity check (`if (size < MAX - 1)`) before writing `items[++size] = x`, returning `bool` to signal overflow.
- `kthSmallest(int arr[], int n, int k)` and `kthLargest(int arr[], int n, int k)` accept raw `int arr[]` + `int n` parameters and check `k >= MAX` in addition to `k < 1 || k > n`.

Source file: [02-kth-smallest-and-kth-largest-array.cpp](02-kth-smallest-and-kth-largest-array.cpp)

```cpp
#include <climits>
#include <iostream>
#define MAX 100
using namespace std;

// 1-indexed fixed-capacity MaxHeap matching Heap-Implementation-OPTIMIZED.cpp.
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
        if (s + 1 <= size && items[s + 1] > items[s]) {
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
    if (size == 0) return INT_MIN;
    return items[1];
}

int MaxHeap::extractMax() {
    if (size == 0) return INT_MIN;
    int maxVal = items[1];
    items[1] = items[size];
    size--;
    if (size > 0) maxHeapify(1);
    return maxVal;
}

bool MaxHeap::isEmpty() { return size == 0; }
int MaxHeap::getSize() { return size; }

void MaxHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << items[i] << " ";
    }
    cout << endl;
}

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

// K'th Smallest using a MaxHeap of size k:
// keeps the k smallest elements seen so far; root is the k'th smallest.
int kthSmallest(int arr[], int n, int k) {
    if (k < 1 || k > n || k >= MAX) return INT_MIN;

    MaxHeap maxH;
    for (int i = 0; i < n; i++) {
        if (maxH.getSize() < k) {
            if (!maxH.insert(arr[i])) return INT_MIN;
        } else if (arr[i] < maxH.getMax()) {
            maxH.extractMax();
            maxH.insert(arr[i]);
        }
    }
    return maxH.getMax();
}

// K'th Largest using a MinHeap of size k:
// keeps the k largest elements seen so far; root is the k'th largest.
int kthLargest(int arr[], int n, int k) {
    if (k < 1 || k > n || k >= MAX) return INT_MIN;

    MinHeap minH;
    for (int i = 0; i < n; i++) {
        if (minH.getSize() < k) {
            if (!minH.insert(arr[i])) return INT_MIN;
        } else if (arr[i] > minH.getMin()) {
            minH.extractMin();
            minH.insert(arr[i]);
        }
    }
    return minH.getMin();
}

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i + 1 < n) cout << ", ";
    }
    cout << "]";
}

void runTest(int arr[], int n, int k) {
    cout << "  Array        : ";
    printArray(arr, n);
    cout << "\n";

    int small = kthSmallest(arr, n, k);
    int large = kthLargest(arr, n, k);

    if (small == INT_MIN) {
        cout << "  Result       : Invalid k (out of bounds)\n\n";
    } else {
        cout << "  K'th Smallest: " << small << "\n";
        cout << "  K'th Largest : " << large << "\n\n";
    }
}

int main() {
    cout << "Test 1 (Classic Unsorted Array) (k = 3)\n";
    int t1[] = {7, 10, 4, 3, 20, 15};
    runTest(t1, 6, 3);

    cout << "Test 2 (LeetCode 215 Example) (k = 2)\n";
    int t2[] = {3, 2, 1, 5, 6, 4};
    runTest(t2, 6, 2);

    cout << "Test 3 (With Duplicates) (k = 4)\n";
    int t3[] = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    runTest(t3, 9, 4);

    cout << "Test 4 (k = 1 Extremes) (k = 1)\n";
    int t4[] = {12, 5, 787, 1, 23};
    runTest(t4, 5, 1);

    cout << "Test 5 (k = n Extremes) (k = 5)\n";
    int t5[] = {12, 5, 787, 1, 23};
    runTest(t5, 5, 5);

    cout << "Test 6 (Invalid k) (k = 0)\n";
    int t6[] = {4, 1, 9};
    runTest(t6, 3, 0);

    return 0;
}
```

---

## LeetCode Pattern Takeaways

The **fixed-size `k` opposite-heap pattern** is one of the top 3 most tested heap patterns in interviews:
- **LeetCode 215 — Kth Largest Element in an Array** (exact `MinHeap` of size `k`).
- **LeetCode 703 — Kth Largest Element in a Stream** (persistent `MinHeap` of size `k`).
- **LeetCode 973 — K Closest Points to Origin** (`MaxHeap` of size `k` keyed by distance).
- **LeetCode 347 — Top K Frequent Elements** (`MinHeap` of size `k` keyed by frequency).
- **LeetCode 692 — Top K Frequent Words** (`MinHeap` of size `k` with custom tie-breaker).
