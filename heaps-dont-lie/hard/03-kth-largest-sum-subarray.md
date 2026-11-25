# K-th Largest Sum Subarray

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — K-th Largest Sum Contiguous Subarray](https://www.geeksforgeeks.org/dsa/k-th-largest-sum-contiguous-subarray/)

---

## 1. Problem Statement

Given an integer array `arr` of length `n` (which may contain positive, negative, and zero values) and an integer `k` ($1 \le k \le \frac{n(n+1)}{2}$), find the **$k$-th largest sum** among all contiguous non-empty subarrays of `arr`.

---

## 2. Pattern Recognition & Intuition

### Why think "Heap" here?
There are $\frac{n(n+1)}{2}$ contiguous subarrays. Generating and sorting all of them wastes $O(N^2)$ memory just to read one rank (`k`).

Whenever a problem asks for the **$k$-th largest (or smallest)** out of a large stream of candidates (here, all subarray sums generated on the fly), use a **fixed-size heap of capacity $k$**.

### Min-Heap vs. Max-Heap Decision Rule
This is one of the most important heap rules in interviews:
- To find the **$k$-th LARGEST**, keep a **Min-Heap of size $k$**!
  - Why? The Min-Heap holds the **$k$ largest sums seen so far**.
  - Its root (`getMin()`) is the **weakest (smallest) member of the top-$k$ club** — which is by definition the $k$-th largest overall!
  - Whenever a new sum arrives that is strictly greater than `getMin()`, evict `getMin()` and insert the new sum.
- Using a Max-Heap instead would require pushing all $O(N^2)$ sums and popping $k$ times, wasting $O(N^2)$ space.

### Prefix Sums + Min-Heap
Using a prefix sum array `prefix` where `prefix[0] = 0` and `prefix[i + 1] = prefix[i] + arr[i]`:
- Any contiguous subarray `arr[i..j-1]` has sum `prefix[j] - prefix[i]` in $O(1)$ time.
- Notice we cannot skip negative numbers or stop early when summing because a negative prefix can still be followed by large positives, or all numbers can be negative.

### Key Invariants & Edge Cases
- **Size invariant:** `topK.getSize()` grows up to `k` and never exceeds `k`.
- **Club invariant:** Once `topK.getSize() == k`, `topK` always contains the $k$ largest subarray sums among all subarrays processed so far.
- **All-negative arrays:** Because the first `k` sums are inserted unconditionally regardless of sign, negative sums (like `-3` in `[-3, -1, -4]`) are handled naturally without needing `-INF` sentinels.
- **$k = 1$ or $k = \frac{n(n+1)}{2}$:** Works identically; when $k = 1$, the Min-Heap of size 1 tracks the maximum subarray sum.

---

## 3. Step-by-Step Trace

Using **Test 1** from `main()`: `arr = [20, -5, -1]`, `k = 3`.

1. **Build Prefix Sums:**
   - `prefix = [0, 20, 15, 14]`

2. **Enumerate Subarrays `(i, j)` (`0 <= i < j <= 3`) & Maintain Min-Heap `topK` (max size `3`):**

| Subarray `arr[i..j-1]` | `prefix[j] - prefix[i]` | Action | `topK` Min-Heap Contents | Current `getMin()` |
| :--- | :--- | :--- | :--- | :--- |
| `[20]` (`i=0, j=1`) | `20 - 0 = 20` | Size < 3 $\rightarrow$ insert `20` | `[20]` | `20` |
| `[20, -5]` (`i=0, j=2`) | `15 - 0 = 15` | Size < 3 $\rightarrow$ insert `15` | `[15, 20]` | `15` |
| `[20, -5, -1]` (`i=0, j=3`) | `14 - 0 = 14` | Size < 3 $\rightarrow$ insert `14` | `[14, 20, 15]` | `14` |
| `[-5]` (`i=1, j=2`) | `15 - 20 = -5` | `-5 <= getMin() (14)` $\rightarrow$ ignore | `[14, 20, 15]` | `14` |
| `[-5, -1]` (`i=1, j=3`) | `14 - 20 = -6` | `-6 <= getMin() (14)` $\rightarrow$ ignore | `[14, 20, 15]` | `14` |
| `[-1]` (`i=2, j=3`) | `14 - 15 = -1` | `-1 <= getMin() (14)` $\rightarrow$ ignore | `[14, 20, 15]` | `14` |

Sorted all sums descending: `20, 15, 14, -1, -5, -6`.
3rd largest = `topK.getMin()` = **`14`**.

---

## 4. Time & Space Complexity

- **Time Complexity:** $O(N^2 \log k)$
  - There are $\frac{N(N+1)}{2}$ subarrays. Each subarray performs at most one `extractMin` + `insert` on a heap of size $k$, costing $O(\log k)$.
- **Space Complexity:** $O(N + k)$
  - $O(N)$ for the `prefix` sum array and $O(k)$ for the `MinHeap` (`topK`). (Note: computing `subSum` on the fly inside the inner loop can reduce prefix storage to $O(1)$, yielding $O(k)$ space total.)

---

## 5. C++ Implementation

See [03-kth-largest-sum-subarray.cpp](03-kth-largest-sum-subarray.cpp).

```cpp
#include <iostream>
#include <vector>
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
            int smallest = curr * 2;
            if (smallest + 1 <= size && items[smallest + 1] < items[smallest]) {
                smallest++;
            }
            if (items[smallest] < x) {
                items[curr] = items[smallest];
                curr = smallest;
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

    bool insert(int val) {
        if (size + 1 < (int)items.size()) {
            items[++size] = val;
        } else {
            items.push_back(val);
            size++;
        }
        heapifyUp(size);
        return true;
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
};

int kthLargestSumSubarray(vector<int>& arr, int k) {
    int n = (int)arr.size();
    vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + arr[i];
    }

    MinHeap topK;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j <= n; j++) {
            int subSum = prefix[j] - prefix[i];
            if (topK.getSize() < k) {
                topK.insert(subSum);
            } else if (subSum > topK.getMin()) {
                topK.extractMin();
                topK.insert(subSum);
            }
        }
    }

    return topK.getMin();
}

int main() {
    vector<int> arr1 = {20, -5, -1};
    int k1 = 3;
    cout << "Test 1 ([20, -5, -1], k=3): "
         << kthLargestSumSubarray(arr1, k1) << "\n";

    vector<int> arr2 = {10, -10, 20, -40};
    int k2 = 6;
    cout << "Test 2 ([10, -10, 20, -40], k=6): "
         << kthLargestSumSubarray(arr2, k2) << "\n";

    vector<int> arr3 = {-3, -1, -4};
    int k3 = 2;
    cout << "Test 3 ([-3, -1, -4], k=2): "
         << kthLargestSumSubarray(arr3, k3) << "\n";

    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

The **Prefix Sum + Fixed-Size Min-Heap for $k$-th Largest** pattern applies to:
- **LC 215 — Kth Largest Element in an Array** (classic Min-Heap of size $k$)
- **LC 703 — Kth Largest Element in a Stream** (persistent Min-Heap of size $k$)
- **LC 1738 — Find Kth Largest XOR Coordinate Value** (2D prefix XOR + Min-Heap of size $k$)
- **LC 2386 — Find the K-Sum of an Array** (advance from max subsequence sum with a Min-Heap of reductions — harder follow-up when $N$ is up to $10^5$)
- **LC 1439 — Find the Kth Smallest Sum of a Matrix With Sorted Rows** (bounded $k$-th sum search with a heap)

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts `MinHeap` and the prefix-sum array to fixed-size raw C++ arrays (`#define MAX 100`, `int items[MAX]`, `int prefix[MAX]`, `int size`), matching `Heap-Implementation-OPTIMIZED.cpp` (`heapifyUp`, `minHeapify`, `insert`, `getMin`, `extractMin`, `remove`, `isEmpty`, `getSize`, `display`) without any STL containers or `const`/`static`/`constexpr` qualifiers.

See [03-kth-largest-sum-subarray-array.cpp](03-kth-largest-sum-subarray-array.cpp).

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
            int smallest = curr * 2;
            if (smallest + 1 <= size && items[smallest + 1] < items[smallest]) {
                smallest++;
            }
            if (items[smallest] < x) {
                items[curr] = items[smallest];
                curr = smallest;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MinHeap() {
        size = 0;
    }

    bool insert(int val) {
        if (size < MAX - 1) {
            items[++size] = val;
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

    bool remove() {
        if (size == 0) {
            return false;
        }
        extractMin();
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
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

int kthLargestSumSubarray(int arr[], int n, int k) {
    int prefix[MAX];
    prefix[0] = 0;
    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + arr[i];
    }

    MinHeap topK;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j <= n; j++) {
            int subSum = prefix[j] - prefix[i];
            if (topK.getSize() < k) {
                topK.insert(subSum);
            } else if (subSum > topK.getMin()) {
                topK.extractMin();
                topK.insert(subSum);
            }
        }
    }

    return topK.getMin();
}

int main() {
    int arr1[3] = {20, -5, -1};
    int k1 = 3;
    cout << "Test 1 ([20, -5, -1], k=3): "
         << kthLargestSumSubarray(arr1, 3, k1) << "\n";

    int arr2[4] = {10, -10, 20, -40};
    int k2 = 6;
    cout << "Test 2 ([10, -10, 20, -40], k=6): "
         << kthLargestSumSubarray(arr2, 4, k2) << "\n";

    int arr3[3] = {-3, -1, -4};
    int k3 = 2;
    cout << "Test 3 ([-3, -1, -4], k=2): "
         << kthLargestSumSubarray(arr3, 3, k3) << "\n";

    return 0;
}
```
