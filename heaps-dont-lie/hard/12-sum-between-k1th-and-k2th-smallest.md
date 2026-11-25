# 12. Sum of All Elements Between k1-th and k2-th Smallest Elements

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — Sum of all elements between k1'th and k2'th smallest elements](https://www.geeksforgeeks.org/dsa/sum-elements-k1th-k2th-smallest-elements/)

---

## 1. Problem Statement

Given an integer array `arr` of size `N` and two 1-based rank integers `k1` and `k2` (`1 <= k1 < k2 <= N`), find the sum of all elements that lie **strictly between** the `k1`-th smallest element and the `k2`-th smallest element in sorted order.

---

## 2. Pattern Recognition & Intuition

### Why Think "Heap" Here?
You only care about the smallest `k2 - 1` elements — you do **not** need to sort the entire array!

- **Naive approach (Full Sort):** Sort `arr` in `O(N log N)` and sum `arr[k1 .. k2 - 2]`. This wastes effort sorting elements larger than the `k2`-th smallest.
- **Min-Heap approach (implemented below):**
  1. Build a **Min-Heap** over `arr` in `O(N)` using bottom-up `minHeapify`.
  2. Call `extractMin()` `k1` times to discard the `1`-st through `k1`-th smallest elements.
  3. Call `extractMin()` `k2 - k1 - 1` more times and accumulate their sum.
  - Total time: `O(N + k2 log N)`. When `k2 << N`, this beats `O(N log N)`.
- **Alternative Max-Heap optimization (for streaming / tiny `k2`):** Maintain a **Max-Heap** of size at most `k2 - 1`. Push each element, and whenever `size > k2 - 1`, pop the max. At the end, pop and sum `k2 - k1 - 1` elements (`O(N log k2)` time, `O(k2)` space).

### Min-Heap vs. Max-Heap Decision Rule
- Extracting ranks in ascending order (`1..k1`, then `k1+1..k2-1`) maps directly to a **Min-Heap** with `O(N)` bottom-up construction.
- Bounded-memory top-K streaming maps to a **Max-Heap** of size `k2 - 1`.

### Key Invariants & Edge Cases
1. **Rank invariant:** After `i` calls to `extractMin()`, the next `extractMin()` returns the `(i + 1)`-th smallest element in 1-based sorted order.
2. **Edge cases:**
   - `k1 >= k2 - 1` (e.g., `k1 = 2, k2 = 3`): Zero elements exist strictly between them -> return `0`.
   - `k1 < 1` or `k2 > N`: Out-of-bounds guard -> return `0`.
   - **Duplicates:** In positional rank order (like `[1, 2, 4, 4, 4, 6]` with `k1 = 1, k2 = 5`), positions `2, 3, 4` are `2, 4, 4`, summing to `10`.
   - **Large sums:** Use `long long` for the accumulator to avoid 32-bit `int` overflow.

---

## 3. Step-by-Step Trace (Test Case 1 from `main()`)

**Input:** `arr = [20, 8, 22, 4, 12, 10, 14]`, `k1 = 3`, `k2 = 6`
*(Sorted reference: `[4, 8, 10, 12, 14, 20, 22]` -> 3rd smallest = `10`, 6th smallest = `20`)*

1. **Bottom-up `buildFromArray` (`O(N)`):**
   1-indexed Min-Heap becomes `[X, 4, 8, 10, 20, 12, 22, 14]` (`size = 7`).

| Phase | Step | Extracted Element | Rank | Action | Running `total` |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Discard `k1 = 3` | 1 | `4` | 1st | Discard | `0` |
| Discard `k1 = 3` | 2 | `8` | 2nd | Discard | `0` |
| Discard `k1 = 3` | 3 | `10` | 3rd (`k1`) | Discard | `0` |
| Sum `k2 - k1 - 1 = 2` | 4 | `12` | 4th | Add to `total` | `12` |
| Sum `k2 - k1 - 1 = 2` | 5 | `14` | 5th | Add to `total` | **`26`** |

Stop! Next element would be `20` (6th = `k2`-th smallest).
**Final Output:** `26`.

---

## 4. Time & Space Complexity

- **Time Complexity:** `O(N + k2 log N)` — `O(N)` to build the Min-Heap bottom-up, plus `(k2 - 1)` `extractMin()` operations at `O(log N)` each.
- **Space Complexity:** `O(N)` for the Min-Heap storage.

---

## 5. C++ Implementation

Source file: [12-sum-between-k1th-and-k2th-smallest.cpp](12-sum-between-k1th-and-k2th-smallest.cpp)

```cpp
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

class MinHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr);
    void minHeapify(int curr);

public:
    MinHeap();
    void insert(int x);
    int getMin();
    int extractMin();
    bool isEmpty();
    int getSize();
    void buildFromArray(vector<int>& arr);
};

MinHeap::MinHeap() {
    items.push_back(0);
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

void MinHeap::insert(int x) {
    items.push_back(x);
    size++;
    heapifyUp(size);
}

int MinHeap::getMin() {
    if (size == 0) {
        return INT_MAX;
    }
    return items[1];
}

int MinHeap::extractMin() {
    if (size == 0) {
        return INT_MAX;
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

bool MinHeap::isEmpty() {
    return size == 0;
}

int MinHeap::getSize() {
    return size;
}

void MinHeap::buildFromArray(vector<int>& arr) {
    items.clear();
    items.push_back(0);
    for (int i = 0; i < (int)arr.size(); i++) {
        items.push_back(arr[i]);
    }
    size = (int)arr.size();
    for (int i = size / 2; i >= 1; i--) {
        minHeapify(i);
    }
}

long long sumBetweenK1AndK2(vector<int>& arr, int k1, int k2) {
    int n = (int)arr.size();
    if (k1 >= k2 - 1 || k1 < 1 || k2 > n) {
        return 0;
    }

    MinHeap pq;
    pq.buildFromArray(arr);

    // Discard the smallest k1 elements
    for (int i = 0; i < k1; i++) {
        pq.extractMin();
    }

    // Sum the next (k2 - k1 - 1) elements strictly between k1-th and k2-th
    long long total = 0;
    int count = k2 - k1 - 1;
    for (int i = 0; i < count; i++) {
        total += pq.extractMin();
    }

    return total;
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(string label, vector<int> arr, int k1, int k2) {
    cout << "=== " << label << " ===\n";
    cout << "Array: ";
    printVector(arr);
    cout << ", k1 = " << k1 << ", k2 = " << k2 << "\n";
    long long ans = sumBetweenK1AndK2(arr, k1, k2);
    cout << "Sum between " << k1 << "th and " << k2 << "th smallest: " << ans << "\n\n";
}

int main() {
    runTest("Test Case 1: Classic 7-element example", {20, 8, 22, 4, 12, 10, 14}, 3, 6);
    runTest("Test Case 2: 6-element example", {10, 2, 50, 12, 48, 13}, 2, 6);
    runTest("Test Case 3: Adjacent ranks (no elements strictly between)", {5, 1, 9, 3, 7}, 2, 3);
    runTest("Test Case 4: Array with duplicates", {4, 2, 4, 1, 6, 4}, 1, 5);
    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

This is the **Top-K / Order-Statistics via Heap** pattern:

- **LC 215 — Kth Largest Element in an Array:** Same Min-Heap vs. Max-Heap trade-off (`O(N log k)` vs `O(N + k log N)`).
- **LC 1508 — Range Sum of Sorted Subarray Sums:** Generate/stream subarray sums and sum between `left` and `right` ranks.
- **LC 347 — Top K Frequent Elements:** Frequency map + bounded Min-Heap of size `k` (or `O(N)` heapify + `k` pops).
- **LC 973 — K Closest Points to Origin:** Bounded Max-Heap of size `k` or `O(N)` Min-Heapify + `k` pops.

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the solution to fixed-size C++ arrays matching `Heap-Implementation-OPTIMIZED.cpp` (`#define MAX 100`, `int items[MAX]`, `int size`, 1-indexed `MinHeap` with `heapifyUp`, `minHeapify`, `insert`, `getMin`, `extractMin`, `isEmpty`, `getSize`, and `display`):
- `MinHeap` replaces `std::vector<int>` with `int items[MAX]` and builds the heap bottom-up in `O(N)` directly from a raw `int arr[]` and length `int n`.
- `sumBetweenK1AndK2(int arr[], int n, int k1, int k2)` discards `k1` elements and sums the next `k2 - k1 - 1` extracted minimums into a `long long` accumulator.
- Avoids `std::vector`, `std::string`, globals, `const`, `static`, and `constexpr`.

Source file: [12-sum-between-k1th-and-k2th-smallest-array.cpp](12-sum-between-k1th-and-k2th-smallest-array.cpp)

```cpp
#include <climits>
#include <iostream>
#define MAX 100
using namespace std;

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
    void buildFromArray(int arr[], int n);
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
    if (size == 0) {
        return INT_MAX;
    }
    return items[1];
}

int MinHeap::extractMin() {
    if (size == 0) {
        return INT_MAX;
    }
    int minVal = items[1];
    items[1] = items[size];
    size--;
    if (size > 0) {
        minHeapify(1);
    }
    return minVal;
}

bool MinHeap::isEmpty() {
    return size == 0;
}

int MinHeap::getSize() {
    return size;
}

void MinHeap::buildFromArray(int arr[], int n) {
    if (n > MAX - 1) {
        n = MAX - 1;
    }
    for (int i = 0; i < n; i++) {
        items[i + 1] = arr[i];
    }
    size = n;
    for (int i = size / 2; i >= 1; i--) {
        minHeapify(i);
    }
}

void MinHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << items[i] << " ";
    }
    cout << endl;
}

long long sumBetweenK1AndK2(int arr[], int n, int k1, int k2) {
    if (k1 >= k2 - 1 || k1 < 1 || k2 > n) {
        return 0;
    }

    MinHeap pq;
    pq.buildFromArray(arr, n);

    // Discard the smallest k1 elements
    for (int i = 0; i < k1; i++) {
        pq.extractMin();
    }

    // Sum the next (k2 - k1 - 1) elements strictly between k1-th and k2-th
    long long total = 0;
    int count = k2 - k1 - 1;
    for (int i = 0; i < count; i++) {
        total += pq.extractMin();
    }

    return total;
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

void runTest(int arr[], int n, int k1, int k2) {
    cout << "Array: ";
    printArray(arr, n);
    cout << ", k1 = " << k1 << ", k2 = " << k2 << "\n";
    long long ans = sumBetweenK1AndK2(arr, n, k1, k2);
    cout << "Sum between " << k1 << "th and " << k2 << "th smallest: " << ans << "\n\n";
}

int main() {
    int arr1[] = {20, 8, 22, 4, 12, 10, 14};
    cout << "=== Test Case 1: Classic 7-element example ===\n";
    runTest(arr1, 7, 3, 6);

    int arr2[] = {10, 2, 50, 12, 48, 13};
    cout << "=== Test Case 2: 6-element example ===\n";
    runTest(arr2, 6, 2, 6);

    int arr3[] = {5, 1, 9, 3, 7};
    cout << "=== Test Case 3: Adjacent ranks (no elements strictly between) ===\n";
    runTest(arr3, 5, 2, 3);

    int arr4[] = {4, 2, 4, 1, 6, 4};
    cout << "=== Test Case 4: Array with duplicates ===\n";
    runTest(arr4, 6, 1, 5);

    return 0;
}
```
