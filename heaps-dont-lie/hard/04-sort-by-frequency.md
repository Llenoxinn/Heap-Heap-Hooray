# Sort by Frequency

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — Sort Elements by Frequency](https://www.geeksforgeeks.org/dsa/sort-elements-by-frequency/)

---

## 1. Problem Statement

Given an integer array `arr`, sort its elements in **decreasing order of their frequency**. If two distinct elements appear the same number of times, the **smaller element must come first**.

---

## 2. Pattern Recognition & Intuition

### Why think "Heap" here?
Sorting by frequency is a two-stage pipeline:
1. **Aggregate counts** using a hash map (`val -> freq`).
2. **Order unique `(val, freq)` entries** by custom priority: primary key `freq` descending, secondary key `val` ascending.

Whenever you aggregate frequencies first and then need items pulled out in custom priority order (especially when tie-breakers or partial top-$k$ extraction are involved), **Hash Map + Custom Heap** is the canonical pattern.

### Min-Heap vs. Max-Heap Decision Rule
- We want the **highest frequency** elements output **first**, so we build a **Max-Heap** keyed by:
  1. Higher `freq` wins (`a.freq > b.freq`).
  2. On a frequency tie, **smaller `val` wins** (`a.val < b.val`).
- Notice how a custom comparator function `hasHigherPriority(a, b)` cleanly separates "what does higher priority mean?" from the mechanical `heapifyUp` / `maxHeapify` logic.

### Why this beats naive approaches
- **Checking frequencies per element during array sort:** Repeatedly scanning or looking up frequencies inside a naive $O(N^2)$ loop is slow.
- **Sorting all $N$ elements directly with map lookup:** Costs $O(N \log N)$ comparisons with a hash lookup on every single swap.
- **Hash Map + Max-Heap over $U$ unique values:** Heap size is $U \le N$, so heap operations cost $O(U \log U)$ — significantly faster when there are many duplicates ($U \ll N$).

### Key Invariants & Edge Cases
- **Uniqueness invariant:** Each unique element enters the `MaxHeap` **exactly once**, paired with its total count. When popped, we emit `top.val` `top.freq` times consecutively so identical elements are grouped together.
- **All unique elements (`U = N`):** Every frequency is `1`, so the tie-breaker (`a.val < b.val`) degrades gracefully to a normal ascending sort.
- **Single element or empty array:** Handled without special cases.
- **Negative numbers:** `unordered_map` supports arbitrary negative/large integers without needing fixed bucket offsets.

---

## 3. Step-by-Step Trace

Using **Test 1** from `main()`: `arr = [5, 5, 4, 6, 4]`

1. **Build `freqMap`:**
   - `5 -> 2`
   - `4 -> 2`
   - `6 -> 1`

2. **Insert unique `(val, freq)` items into `MaxHeap`:**
   - Insert `(5, 2)` $\rightarrow$ heap: `[(5, 2)]`
   - Insert `(4, 2)` $\rightarrow$ tie on `freq == 2`, `4 < 5`, so `(4, 2)` bubbles up $\rightarrow$ heap: `[(4, 2), (5, 2)]`
   - Insert `(6, 1)` $\rightarrow$ `freq 1 < 2`, stays at leaf $\rightarrow$ heap: `[(4, 2), (5, 2), (6, 1)]`

3. **Extract from `MaxHeap` until empty:**

| Step | `extractMax()` | Why it won root | Append to `result` | Remaining Heap |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `(4, 2)` | Highest `freq=2`, smaller `val` than `5` | `4, 4` | `[(5, 2), (6, 1)]` |
| 2 | `(5, 2)` | Highest remaining `freq=2` | `5, 5` | `[(6, 1)]` |
| 3 | `(6, 1)` | Only remaining entry `freq=1` | `6` | `[]` |

Final Output: `[4, 4, 5, 5, 6]`.

---

## 4. Time & Space Complexity

- **Time Complexity:** $O(N + U \log U)$, where $U$ is the number of distinct elements ($U \le N$).
  - Building `freqMap`: $O(N)$ average.
  - Inserting $U$ items and extracting $U$ items from `MaxHeap`: $O(U \log U)$.
  - Writing $N$ elements to `result`: $O(N)$.
- **Space Complexity:** $O(N)$
  - `freqMap` and `MaxHeap` take $O(U)$ space; the output array takes $O(N)$ space.

---

## 5. C++ Implementation

See [04-sort-by-frequency.cpp](04-sort-by-frequency.cpp).

```cpp
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

struct FreqItem {
    int val;
    int freq;
};

bool hasHigherPriority(FreqItem a, FreqItem b) {
    if (a.freq != b.freq) {
        return a.freq > b.freq; // Higher frequency first
    }
    return a.val < b.val; // Tie-breaker: smaller value first
}

class MaxHeap {
private:
    vector<FreqItem> items;
    int size;

    void heapifyUp(int curr) {
        FreqItem x = items[curr];
        while (curr / 2 > 0 && hasHigherPriority(x, items[curr / 2])) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        FreqItem x = items[curr];
        while (curr * 2 <= size) {
            int best = curr * 2;
            if (best + 1 <= size && hasHigherPriority(items[best + 1], items[best])) {
                best++;
            }
            if (hasHigherPriority(items[best], x)) {
                items[curr] = items[best];
                curr = best;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        items.push_back({0, 0});
        size = 0;
    }

    bool insert(FreqItem item) {
        if (size + 1 < (int)items.size()) {
            items[++size] = item;
        } else {
            items.push_back(item);
            size++;
        }
        heapifyUp(size);
        return true;
    }

    FreqItem getMax() {
        if (size == 0) {
            return {0, 0};
        }
        return items[1];
    }

    FreqItem extractMax() {
        if (size == 0) {
            return {0, 0};
        }
        FreqItem top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return top;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<int> sortByFrequency(vector<int>& arr) {
    unordered_map<int, int> freqMap;
    for (int i = 0; i < (int)arr.size(); i++) {
        freqMap[arr[i]]++;
    }

    MaxHeap heap;
    for (auto it = freqMap.begin(); it != freqMap.end(); it++) {
        heap.insert({it->first, it->second});
    }

    vector<int> result;
    while (!heap.isEmpty()) {
        FreqItem top = heap.extractMax();
        for (int c = 0; c < top.freq; c++) {
            result.push_back(top.val);
        }
    }

    return result;
}

void printArray(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) {
            cout << ", ";
        }
    }
    cout << "]\n";
}

int main() {
    vector<int> arr1 = {5, 5, 4, 6, 4};
    vector<int> res1 = sortByFrequency(arr1);
    cout << "Test 1 ([5, 5, 4, 6, 4]): ";
    printArray(res1);

    vector<int> arr2 = {9, 9, 9, 2, 5};
    vector<int> res2 = sortByFrequency(arr2);
    cout << "Test 2 ([9, 9, 9, 2, 5]): ";
    printArray(res2);

    vector<int> arr3 = {2, 5, 2, 8, 5, 6, 8, 8};
    vector<int> res3 = sortByFrequency(arr3);
    cout << "Test 3 ([2, 5, 2, 8, 5, 6, 8, 8]): ";
    printArray(res3);

    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

The **Frequency Map + Custom-Comparator Heap** pattern solves:
- **LC 451 — Sort Characters By Frequency** (exact same pattern on `char`)
- **LC 347 — Top K Frequent Elements** (Min-Heap of size $k$ over `freqMap`)
- **LC 692 — Top K Frequent Words** (Min-Heap of size $k$ with lexicographical tie-breaker)
- **LC 1636 — Sort Array by Increasing Frequency** (Min-Heap by `freq`, tie-break larger `val` first)
- **LC 767 — Reorganize String** (Max-Heap by remaining frequency, alternating top two characters)

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the frequency-sorting solution to fixed-size raw arrays (`#define MAX 100`, `FreqItem items[MAX]`, `int size`), matching `Heap-Implementation-OPTIMIZED.cpp` (`heapifyUp`, `maxHeapify`, `insert`, `getMax`, `extractMax`, `remove`, `isEmpty`, `getSize`, `display`), without `std::unordered_map`, `std::vector`, `<algorithm>`, global variables, or `const`/`static`/`constexpr`:
- Instead of `unordered_map`, `sortByFrequency` copies the input into a fixed `int sorted[MAX]` array, sorts it with a manual `quickSortInts`, and groups consecutive equal elements in a single linear pass to push `(val, freq)` entries into `MaxHeap`.
- This preserves support for arbitrary negative or large integers without bounded bucket offsets.

See [04-sort-by-frequency-array.cpp](04-sort-by-frequency-array.cpp).

```cpp
#include <iostream>
#define MAX 100
using namespace std;

struct FreqItem {
    int val;
    int freq;
};

bool hasHigherPriority(FreqItem a, FreqItem b) {
    if (a.freq != b.freq) {
        return a.freq > b.freq; // Higher frequency first
    }
    return a.val < b.val; // Tie-breaker: smaller value first
}

void swapInts(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int partitionInts(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swapInts(arr[i], arr[j]);
        }
    }
    swapInts(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortInts(int arr[], int low, int high) {
    if (low < high) {
        int pi = partitionInts(arr, low, high);
        quickSortInts(arr, low, pi - 1);
        quickSortInts(arr, pi + 1, high);
    }
}

class MaxHeap {
private:
    FreqItem items[MAX];
    int size;

    void heapifyUp(int curr) {
        FreqItem x = items[curr];
        while (curr / 2 > 0 && hasHigherPriority(x, items[curr / 2])) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        FreqItem x = items[curr];
        while (curr * 2 <= size) {
            int best = curr * 2;
            if (best + 1 <= size && hasHigherPriority(items[best + 1], items[best])) {
                best++;
            }
            if (hasHigherPriority(items[best], x)) {
                items[curr] = items[best];
                curr = best;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        size = 0;
    }

    bool insert(FreqItem item) {
        if (size < MAX - 1) {
            items[++size] = item;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    FreqItem getMax() {
        if (size == 0) {
            return {0, 0};
        }
        return items[1];
    }

    FreqItem extractMax() {
        if (size == 0) {
            return {0, 0};
        }
        FreqItem top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return top;
    }

    bool remove() {
        if (size == 0) {
            return false;
        }
        extractMax();
        return true;
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
            cout << "(" << items[i].val << ":" << items[i].freq << ") ";
        }
        cout << endl;
    }
};

void sortByFrequency(int arr[], int n, int result[], int& resultSize) {
    int sorted[MAX];
    for (int i = 0; i < n; i++) {
        sorted[i] = arr[i];
    }
    quickSortInts(sorted, 0, n - 1);

    MaxHeap heap;
    int i = 0;
    while (i < n) {
        int j = i;
        while (j < n && sorted[j] == sorted[i]) {
            j++;
        }
        heap.insert({sorted[i], j - i});
        i = j;
    }

    resultSize = 0;
    while (!heap.isEmpty()) {
        FreqItem top = heap.extractMax();
        for (int c = 0; c < top.freq; c++) {
            result[resultSize++] = top.val;
        }
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
    cout << "]\n";
}

int main() {
    int arr1[5] = {5, 5, 4, 6, 4};
    int res1[MAX];
    int resSize1 = 0;
    sortByFrequency(arr1, 5, res1, resSize1);
    cout << "Test 1 ([5, 5, 4, 6, 4]): ";
    printArray(res1, resSize1);

    int arr2[5] = {9, 9, 9, 2, 5};
    int res2[MAX];
    int resSize2 = 0;
    sortByFrequency(arr2, 5, res2, resSize2);
    cout << "Test 2 ([9, 9, 9, 2, 5]): ";
    printArray(res2, resSize2);

    int arr3[8] = {2, 5, 2, 8, 5, 6, 8, 8};
    int res3[MAX];
    int resSize3 = 0;
    sortByFrequency(arr3, 8, res3, resSize3);
    cout << "Test 3 ([2, 5, 2, 8, 5, 6, 8, 8]): ";
    printArray(res3, resSize3);

    return 0;
}
```
