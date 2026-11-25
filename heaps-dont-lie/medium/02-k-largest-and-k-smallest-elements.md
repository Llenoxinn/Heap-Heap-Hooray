# K Largest and K Smallest Elements

- **Difficulty:** Medium
- **References:**
  - [GeeksforGeeks — K Largest Elements in an Array](https://www.geeksforgeeks.org/dsa/k-largestor-smallest-elements-in-an-array/)
  - [GeeksforGeeks — K Smallest Elements in an Array](https://www.geeksforgeeks.org/dsa/find-k-smallest-elements-in-an-array/)

---

## Problem Statement

Given an unsorted array `arr` of `n` integers and an integer `k`:
1. **K Largest Elements:** Find the `k` largest elements in the array (returned in descending order).
2. **K Smallest Elements:** Find the `k` smallest elements in the array (returned in ascending order).

**Example:**
- Input: `arr = [1, 23, 12, 9, 30, 2, 50]`, `k = 3`
- K Largest: `[50, 30, 23]`
- K Smallest: `[1, 2, 9]`

---

## Pattern Recognition & Intuition

- **Why think "Heap"?**
  Whenever you see **"Top K"** (`k` largest, `k` smallest, `k`th largest/smallest), you do **not** need the relative order of all `n` elements — you only need to maintain a VIP club of size `k` and know who the weakest member of that club is so you can evict them when a stronger candidate arrives.
- **Min-Heap vs Max-Heap Decision Rule (The "Opposite Heap" Rule!):**
  This is the #1 rule to memorize for Top-K heap problems:
  - To find the **K Largest** elements → use a **Min-Heap** of size `k`.
    *Why?* The root is the **smallest of the `k` largest** elements (the "gatekeeper"). Any new element `x <= root` cannot possibly belong in the top `k`, so we skip it in $O(1)$. If `x > root`, we evict the root (`extractMin`) and insert `x`.
  - To find the **K Smallest** elements → use a **Max-Heap** of size `k`.
    *Why?* The root is the **largest of the `k` smallest** elements (the "gatekeeper"). Any new element `x >= root` is too big to enter the bottom `k`, so we skip it in $O(1)$. If `x < root`, we evict the root (`extractMax`) and insert `x`.
- **Why it beats naive approaches:**
  - Full sorting: $O(n \log n)$ time and potentially $O(n)$ space, even if $k = 3$ and $n = 10^7$.
  - Putting all `n` elements in a heap and popping `k` times: $O(n + k \log n)$ time, $O(n)$ space.
  - Fixed-size `k` opposite-heap: $O(n \log k)$ time and only **$O(k)$ space**, which also works cleanly on **infinite streams**!
- **Key Invariant:**
  After processing `arr[0..i]` (for `i >= k - 1`):
  - The Min-Heap holds the exact `k` largest elements seen in `arr[0..i]`.
  - The Max-Heap holds the exact `k` smallest elements seen in `arr[0..i]`.
- **Edge Cases:**
  - `k <= 0` or empty array: return empty vector.
  - `k >= n`: cap `limit = min(k, n)` so we return all `n` elements sorted.
  - Duplicates and negative numbers: strict inequality (`>` for K Largest, `<` for K Smallest) avoids unnecessary heap churn when `arr[i] == root`.

---

## Step-by-Step Trace

Tracing **Test 1** from `main()`: `arr = [1, 23, 12, 9, 30, 2, 50]`, `k = 3`.

### Part A: K Largest (Min-Heap of size `3`)
1. `x = 1`: size `< 3` → `insert(1)`. Heap: `[1]`
2. `x = 23`: size `< 3` → `insert(23)`. Heap: `[1, 23]`
3. `x = 12`: size `< 3` → `insert(12)`. Heap: `[1, 23, 12]`
4. `x = 9`: `9 > getMin() (1)` → `extractMin()`, `insert(9)`. Heap: `[9, 23, 12]`
5. `x = 30`: `30 > getMin() (9)` → `extractMin()`, `insert(30)`. Heap: `[12, 23, 30]`
6. `x = 2`: `2 <= getMin() (12)` → skip! Heap: `[12, 23, 30]`
7. `x = 50`: `50 > getMin() (12)` → `extractMin()`, `insert(50)`. Heap: `[23, 50, 30]`

Drain Min-Heap: `23, 30, 50` → reverse → **`[50, 30, 23]`**.

### Part B: K Smallest (Max-Heap of size `3`)
1. `x = 1`: size `< 3` → `insert(1)`. Heap: `[1]`
2. `x = 23`: size `< 3` → `insert(23)`. Heap: `[23, 1]`
3. `x = 12`: size `< 3` → `insert(12)`. Heap: `[23, 1, 12]`
4. `x = 9`: `9 < getMax() (23)` → `extractMax()`, `insert(9)`. Heap: `[12, 1, 9]`
5. `x = 30`: `30 >= getMax() (12)` → skip! Heap: `[12, 1, 9]`
6. `x = 2`: `2 < getMax() (12)` → `extractMax()`, `insert(2)`. Heap: `[9, 1, 2]`
7. `x = 50`: `50 >= getMax() (9)` → skip! Heap: `[9, 1, 2]`

Drain Max-Heap: `9, 2, 1` → reverse → **`[1, 2, 9]`**.

---

## Time & Space Complexity

- **Time Complexity:** $O(n \log k)$ for each pass (`findKLargest` and `findKSmallest`).
  - Each of the `n` elements performs at most one `extract` and one `insert` on a heap of size at most `k` ($O(\log k)$).
  - Draining and reversing `k` elements takes $O(k \log k)$.
- **Space Complexity:** $O(k)$ auxiliary space for the heap (plus $O(k)$ for the output array).

---

## C++ Implementation

See [02-k-largest-and-k-smallest-elements.cpp](02-k-largest-and-k-smallest-elements.cpp).

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

public:
    MaxHeap() {
        items.push_back(0);
        size = 0;
    }

    void insert(int x) {
        items.push_back(x);
        size++;
        heapifyUp(size);
    }

    int getMax() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    int extractMax() {
        if (size == 0) {
            return -1;
        }
        int maxVal = items[1];
        items[1] = items[size];
        items.pop_back();
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return maxVal;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<int> findKLargest(vector<int>& arr, int k) {
    vector<int> result;
    if (k <= 0 || arr.empty()) {
        return result;
    }
    int limit = min(k, (int)arr.size());
    MinHeap minHeap;

    for (int i = 0; i < (int)arr.size(); i++) {
        if (minHeap.getSize() < limit) {
            minHeap.insert(arr[i]);
        } else if (arr[i] > minHeap.getMin()) {
            minHeap.extractMin();
            minHeap.insert(arr[i]);
        }
    }

    while (!minHeap.isEmpty()) {
        result.push_back(minHeap.extractMin());
    }
    reverse(result.begin(), result.end());
    return result;
}

vector<int> findKSmallest(vector<int>& arr, int k) {
    vector<int> result;
    if (k <= 0 || arr.empty()) {
        return result;
    }
    int limit = min(k, (int)arr.size());
    MaxHeap maxHeap;

    for (int i = 0; i < (int)arr.size(); i++) {
        if (maxHeap.getSize() < limit) {
            maxHeap.insert(arr[i]);
        } else if (arr[i] < maxHeap.getMax()) {
            maxHeap.extractMax();
            maxHeap.insert(arr[i]);
        }
    }

    while (!maxHeap.isEmpty()) {
        result.push_back(maxHeap.extractMax());
    }
    reverse(result.begin(), result.end());
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
    cout << "]";
}

void runTest(string label, vector<int> arr, int k) {
    cout << label << " (k = " << k << ")\n";
    cout << "  Input     : ";
    printArray(arr);
    cout << "\n";

    vector<int> largest = findKLargest(arr, k);
    vector<int> smallest = findKSmallest(arr, k);

    cout << "  K Largest : ";
    printArray(largest);
    cout << "\n  K Smallest: ";
    printArray(smallest);
    cout << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Example)", {1, 23, 12, 9, 30, 2, 50}, 3);
    runTest("Test 2 (k = 4)", {7, 10, 4, 3, 20, 15}, 4);
    runTest("Test 3 (Duplicates & Negatives)", {-5, 12, -5, 0, 8, 12}, 3);
    runTest("Test 4 (Edge Case k = 1)", {42, 7, 19, -3, 88}, 1);
    runTest("Test 5 (Edge Case k >= n)", {3, 1, 2}, 5);
    return 0;
}
```

---

## LeetCode Pattern Takeaways

The **fixed-size opposite heap** is one of the highest-frequency interview patterns:
- **LeetCode 215 — Kth Largest Element in an Array:** Identical Min-Heap of size `k`; answer is `minHeap.getMin()`.
- **LeetCode 703 — Kth Largest Element in a Stream:** Maintain the exact same `MinHeap` of size `k` across stream updates.
- **LeetCode 347 — Top K Frequent Elements:** Min-Heap of size `k` keyed by `(frequency, value)`.
- **LeetCode 973 — K Closest Points to Origin:** Max-Heap of size `k` keyed by squared Euclidean distance.
