# 11. Merge Two Binary Max Heaps

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — Merge Two Binary Max Heaps](https://www.geeksforgeeks.org/dsa/merge-two-binary-max-heaps/)

---

## 1. Problem Statement

Given two binary max-heaps `a` (of size `N`) and `b` (of size `M`) represented as arrays, merge them into a single valid binary max-heap of size `N + M`.

---

## 2. Pattern Recognition & Intuition

### Why Think "Bottom-Up Heapify" Instead of Repeated Insertion?
When combining two heaps of sizes `N` and `M`:

- **Naive approach (Repeated `insert`):** Keep heap `a` (`N` elements) and insert all `M` elements from `b` one by one using `heapifyUp`. That takes `O(M log(N + M))`, or `O((N + M) log(N + M))` if starting from scratch.
- **Optimal approach (Concatenate + Bottom-Up `buildHeap`):** Concatenate `a` and `b` into a single array of length `K = N + M`, then run **Floyd's bottom-up `maxHeapify`** from the last internal node `K / 2 - 1` down to `0`.
  - Half the nodes in a complete binary tree are leaves (height `0`) and need **zero** work.
  - A quarter are at height `1` (`<= 1` swap), an eighth at height `2` (`<= 2` swaps), etc.
  - The geometric series `sum(h * K / 2^(h+1))` converges to **`O(N + M)` linear time**!

### Min-Heap vs. Max-Heap Decision Rule
- Both inputs and the required output satisfy the **Max-Heap** property (`items[i] >= items[2*i + 1]` and `items[i] >= items[2*i + 2]`), so we use `maxHeapify`.

### Key Invariants & Edge Cases
1. **Subtree invariant during bottom-up `buildFromArray`:** Before calling `maxHeapify(i)`, both `2*i + 1` (left subtree) and `2*i + 2` (right subtree) are already valid max-heaps. Sifting `items[i]` down restores the max-heap invariant for the entire subtree rooted at `i`.
2. **Edge cases:**
   - One or both heaps empty (`N = 0` or `M = 0`): `n / 2 - 1 < 0`, loop skips cleanly.
   - Duplicate elements across `a` and `b`: using `>` in comparisons keeps duplicates valid.
   - Multiple valid max-heaps may exist for the same multiset of numbers (e.g., swapping two children), and all satisfy the max-heap property.

---

## 3. Step-by-Step Trace (Test Case 1 from `main()`)

**Input:**
- `a = [10, 5, 6, 2]` (`N = 4`)
- `b = [12, 7, 9]` (`M = 3`)

**Step 1 — Concatenate:**
`combined = [10, 5, 6, 2, 12, 7, 9]` (`K = 7`, indices `0..6`).
Last internal node index: `7 / 2 - 1 = 2`.

```text
Tree before heapify:
         10 (0)
       /        \
     5 (1)      6 (2)
    /    \     /    \
  2 (3) 12(4) 7(5)  9(6)
```

| Step | Index `i` | Node Value | Children | Action | Array State |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `i = 2` | `6` | `7` (idx 5), `9` (idx 6) | Largest child is `9` (idx 6) > `6` -> swap idx 2 and 6 | `[10, 5, 9, 2, 12, 7, 6]` |
| 2 | `i = 1` | `5` | `2` (idx 3), `12` (idx 4) | Largest child is `12` (idx 4) > `5` -> swap idx 1 and 4 | `[10, 12, 9, 2, 5, 7, 6]` |
| 3 | `i = 0` | `10` | `12` (idx 1), `9` (idx 2) | Largest child is `12` (idx 1) > `10` -> swap idx 0 and 1; then at idx 1, `10` > children `2, 5` -> stop | **`[12, 10, 9, 2, 5, 7, 6]`** |

**Resulting Max-Heap:** `[12, 10, 9, 2, 5, 7, 6]`
Verify: `12 >= 10, 9`; `10 >= 2, 5`; `9 >= 7, 6`. Valid Max-Heap!

---

## 4. Time & Space Complexity

- **Time Complexity:** `O(N + M)` for concatenation + Floyd's bottom-up heap construction (`buildFromArray`).
- **Space Complexity:** `O(N + M)` for the merged array. (If `a` already had spare capacity, `O(1)` auxiliary beyond the output array.)

---

## 5. C++ Implementation

Source file: [11-merge-two-binary-max-heaps.cpp](11-merge-two-binary-max-heaps.cpp)

```cpp
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

class MaxHeap {
private:
    vector<int> items;

    void heapifyUp(int curr);
    void maxHeapify(int curr);

public:
    MaxHeap();
    void insert(int x);
    int getMax();
    int extractMax();
    bool isEmpty();
    int getSize();
    void buildFromArray(vector<int>& arr);
    vector<int> getArray();
};

MaxHeap::MaxHeap() {
}

void MaxHeap::heapifyUp(int curr) {
    int x = items[curr];
    while (curr > 0) {
        int parent = (curr - 1) / 2;
        if (x > items[parent]) {
            items[curr] = items[parent];
            curr = parent;
        } else {
            break;
        }
    }
    items[curr] = x;
}

void MaxHeap::maxHeapify(int curr) {
    int n = (int)items.size();
    int x = items[curr];
    while (2 * curr + 1 < n) {
        int largest = 2 * curr + 1;
        int right = largest + 1;
        if (right < n && items[right] > items[largest]) {
            largest = right;
        }
        if (items[largest] > x) {
            items[curr] = items[largest];
            curr = largest;
        } else {
            break;
        }
    }
    items[curr] = x;
}

void MaxHeap::insert(int x) {
    items.push_back(x);
    heapifyUp((int)items.size() - 1);
}

int MaxHeap::getMax() {
    if (items.empty()) {
        return INT_MIN;
    }
    return items[0];
}

int MaxHeap::extractMax() {
    if (items.empty()) {
        return INT_MIN;
    }
    int maxVal = items[0];
    items[0] = items[(int)items.size() - 1];
    items.pop_back();
    if (!items.empty()) {
        maxHeapify(0);
    }
    return maxVal;
}

bool MaxHeap::isEmpty() {
    return items.empty();
}

int MaxHeap::getSize() {
    return (int)items.size();
}

void MaxHeap::buildFromArray(vector<int>& arr) {
    items = arr;
    int n = (int)items.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(i);
    }
}

vector<int> MaxHeap::getArray() {
    return items;
}

vector<int> mergeTwoMaxHeaps(vector<int>& a, vector<int>& b) {
    vector<int> combined;
    combined.reserve(a.size() + b.size());
    for (int i = 0; i < (int)a.size(); i++) {
        combined.push_back(a[i]);
    }
    for (int i = 0; i < (int)b.size(); i++) {
        combined.push_back(b[i]);
    }

    MaxHeap mergedHeap;
    mergedHeap.buildFromArray(combined);
    return mergedHeap.getArray();
}

bool isValidMaxHeap(vector<int>& arr) {
    int n = (int)arr.size();
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

void runTest(string label, vector<int> a, vector<int> b) {
    cout << "=== " << label << " ===\n";
    cout << "Heap A: ";
    printVector(a);
    cout << "\nHeap B: ";
    printVector(b);
    cout << "\n";

    vector<int> merged = mergeTwoMaxHeaps(a, b);
    cout << "Merged MaxHeap: ";
    printVector(merged);
    cout << " (valid max-heap: " << (isValidMaxHeap(merged) ? "YES" : "NO") << ")\n";

    MaxHeap drain;
    drain.buildFromArray(merged);
    cout << "Extracted descending:";
    while (!drain.isEmpty()) {
        cout << " " << drain.extractMax();
    }
    cout << "\n\n";
}

int main() {
    runTest("Test Case 1: Classic Two Max Heaps", {10, 5, 6, 2}, {12, 7, 9});
    runTest("Test Case 2: Duplicates Across Heaps", {8, 4, 8, 1}, {8, 3, 2});
    runTest("Test Case 3: One Empty Heap", {}, {20, 15, 10, 5});
    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

Remembering that **offline `buildHeap` is `O(N)` while online `N` inserts is `O(N log N)`** pays off across many problems:

- **LC 1046 — Last Stone Weight:** Heapify the initial stones array in `O(N)` before running the simulation loop.
- **LC 215 — Kth Largest Element in an Array:** Building a Max-Heap in `O(N)` + `k` pops gives `O(N + k log N)`, faster than `O(N log N)` sorting when `k` is small.
- **LC 912 — Sort an Array (Heap Sort):** Uses `O(N)` bottom-up `maxHeapify` as Phase 1 before `O(N log N)` extraction.
- **LC 2558 — Take Gifts From the Richest Pile:** `O(N)` heapify + `k` rounds of extract/re-insert.

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the solution to fixed-size C++ arrays matching `Heap-Implementation-OPTIMIZED.cpp` (`#define MAX 100`, `int items[MAX]`, `int size`, 1-indexed `MaxHeap` with `heapifyUp`, `maxHeapify`, `insert`, `getMax`, `extractMax`, `isEmpty`, `getSize`, and `display`):
- `MaxHeap` stores elements 1-indexed in `int items[MAX]` and runs Floyd's `O(N + M)` bottom-up `buildFromArray(int arr[], int n)` from `size / 2` down to `1`, then exports the 0-indexed heap array via `copyToArray(int out[])`.
- `mergeTwoMaxHeaps(int a[], int na, int b[], int nb, int merged[])` concatenates the raw input arrays into a fixed `int combined[MAX]` buffer and returns the merged element count.
- Avoids `std::vector`, `std::string`, globals, `const`, `static`, and `constexpr`.

Source file: [11-merge-two-binary-max-heaps-array.cpp](11-merge-two-binary-max-heaps-array.cpp)

```cpp
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
```
