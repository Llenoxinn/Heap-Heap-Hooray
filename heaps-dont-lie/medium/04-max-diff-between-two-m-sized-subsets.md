# Max Difference Between Two m-Sized Subsets

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Difference Between Maximum Sum and Minimum Sum of m Elements](https://www.geeksforgeeks.org/dsa/difference-maximum-sum-minimum-sum-n-m-elementsin-review/)

---

## Problem Statement

Given an array `arr` of `n` integers and an integer `m` (`1 <= m <= n`), find the maximum possible difference between the sum of two `m`-sized subsets of `arr`:

$$\max(\text{Sum}(S_1) - \text{Sum}(S_2))$$

where $|S_1| = |S_2| = m$.

**Example:**
- Input: `arr = [1, 2, 3, 4, 5]`, `m = 2`
- Output: `6` (maximum subset `{4, 5}` has sum `9`; minimum subset `{1, 2}` has sum `3`; `9 - 3 = 6`)

---

## Pattern Recognition & Intuition

- **Why think "Heap"?**
  To maximize `Sum(S1) - Sum(S2)`, we must simultaneously:
  1. **Maximize `Sum(S1)`** → pick the **`m` largest** elements of `arr`.
  2. **Minimize `Sum(S2)`** → pick the **`m` smallest** elements of `arr`.
  That means this problem cleanly reduces to **two Top-K problems** running side by side in a single pass!
- **Min-Heap vs Max-Heap Decision Rule:**
  - Use a **Min-Heap** of size `m` (`largestM`) to collect the `m` largest elements.
  - Use a **Max-Heap** of size `m` (`smallestM`) to collect the `m` smallest elements.
  *(Dual-heap pattern: opposite heap for each extreme!)*
- **Why it beats naive approaches:**
  - Brute force over all $\binom{n}{m}$ subsets: exponential.
  - Sorting the whole array: $O(n \log n)$ time.
  - Dual fixed-size `m` heaps: $O(n \log m)$ time and $O(m)$ space — optimal when `m << n`, and computes both sums in a single linear scan!
- **Key Invariant:**
  After scanning element `arr[i]` (once `i >= m - 1`):
  - `largestM` holds the `m` largest elements seen in `arr[0..i]`.
  - `smallestM` holds the `m` smallest elements seen in `arr[0..i]`.
- **Edge Cases:**
  - `m <= 0` or `m > n`: return `0`.
  - `m == n`: both subsets use all `n` elements, so `maxSum == minSum` and diff is `0`.
  - Negative numbers: subtracting a negative `minSum` naturally increases the difference (e.g., `10 - (-4) = 14`).
  - Large sums: accumulate into `long long` to avoid 32-bit overflow.

---

## Step-by-Step Trace

Tracing **Test 1** from `main()`: `arr = [1, 2, 3, 4, 5]`, `m = 2`.

| Step | `arr[i]` | `largestM` (Min-Heap, size `2`) | `smallestM` (Max-Heap, size `2`) |
| --- | --- | --- | --- |
| `i = 0` | `1` | `[1]` | `[1]` |
| `i = 1` | `2` | `[1, 2]` | `[2, 1]` |
| `i = 2` | `3` | `3 > 1` → evict `1`, insert `3` → `[2, 3]` | `3 >= 2` → skip → `[2, 1]` |
| `i = 3` | `4` | `4 > 2` → evict `2`, insert `4` → `[3, 4]` | `4 >= 2` → skip → `[2, 1]` |
| `i = 4` | `5` | `5 > 3` → evict `3`, insert `5` → `[4, 5]` | `5 >= 2` → skip → `[2, 1]` |

- Drain `largestM`: `maxSum = 4 + 5 = 9`
- Drain `smallestM`: `minSum = 2 + 1 = 3`
- Result: `maxSum - minSum = 9 - 3 = 6`

---

## Time & Space Complexity

- **Time Complexity:** $O(n \log m)$
  - Each of the `n` elements does at most one eviction + insertion on a Min-Heap of size `m` and one on a Max-Heap of size `m`, costing $O(\log m)$ each.
  - Draining both heaps takes $O(m \log m)$.
- **Space Complexity:** $O(m)$
  - Both heaps hold at most `m` integers at any time.

---

## C++ Implementation

See [04-max-diff-between-two-m-sized-subsets.cpp](04-max-diff-between-two-m-sized-subsets.cpp).

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

long long maxDiffBetweenTwoMSizedSubsets(vector<int>& arr, int m) {
    int n = (int)arr.size();
    if (m <= 0 || m > n) {
        return 0;
    }

    MinHeap largestM;
    MaxHeap smallestM;

    for (int i = 0; i < n; i++) {
        if (largestM.getSize() < m) {
            largestM.insert(arr[i]);
        } else if (arr[i] > largestM.getMin()) {
            largestM.extractMin();
            largestM.insert(arr[i]);
        }

        if (smallestM.getSize() < m) {
            smallestM.insert(arr[i]);
        } else if (arr[i] < smallestM.getMax()) {
            smallestM.extractMax();
            smallestM.insert(arr[i]);
        }
    }

    long long maxSum = 0;
    while (!largestM.isEmpty()) {
        maxSum += largestM.extractMin();
    }

    long long minSum = 0;
    while (!smallestM.isEmpty()) {
        minSum += smallestM.extractMax();
    }

    return maxSum - minSum;
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

void runTest(string label, vector<int> arr, int m) {
    cout << label << " (m = " << m << ")\n";
    cout << "  Input   : ";
    printArray(arr);
    cout << "\n  Max Diff: " << maxDiffBetweenTwoMSizedSubsets(arr, m) << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Example)", {1, 2, 3, 4, 5}, 2);
    runTest("Test 2 (Unsorted Array)", {5, 8, 11, 40, 15}, 2);
    runTest("Test 3 (Negatives & m = 1)", {10, -4, 7, 2}, 1);
    runTest("Test 4 (Edge Case m = n)", {3, 1, 4}, 3);
    runTest("Test 5 (All Equal)", {5, 5, 5, 5}, 2);
    return 0;
}
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version implements both `MinHeap` and `MaxHeap` using fixed-size 1-indexed raw arrays matching `Heap-Implementation-OPTIMIZED.cpp`, with zero STL containers (`std::vector`, `<algorithm>`, `std::string`):
- `MinHeap` and `MaxHeap` use `int items[MAX]` (`#define MAX 100`) and `int size`, implementing `heapifyUp`, `minHeapify`/`maxHeapify`, `insert`, `getMin`/`getMax`, `extractMin`/`extractMax`, `isEmpty`, `getSize`, and `display`.
- `maxDiffBetweenTwoMSizedSubsets(int arr[], int n, int m)` streams the raw array `arr[0..n-1]` through both bounded heaps to compute `maxSum - minSum` in `long long`.

See [04-max-diff-between-two-m-sized-subsets-array.cpp](04-max-diff-between-two-m-sized-subsets-array.cpp).

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

class MaxHeap {
private:
    int items[MAX];
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

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

long long maxDiffBetweenTwoMSizedSubsets(int arr[], int n, int m) {
    if (m <= 0 || m > n) {
        return 0;
    }

    MinHeap largestM;
    MaxHeap smallestM;

    for (int i = 0; i < n; i++) {
        if (largestM.getSize() < m) {
            largestM.insert(arr[i]);
        } else if (arr[i] > largestM.getMin()) {
            largestM.extractMin();
            largestM.insert(arr[i]);
        }

        if (smallestM.getSize() < m) {
            smallestM.insert(arr[i]);
        } else if (arr[i] < smallestM.getMax()) {
            smallestM.extractMax();
            smallestM.insert(arr[i]);
        }
    }

    long long maxSum = 0;
    while (!largestM.isEmpty()) {
        maxSum += largestM.extractMin();
    }

    long long minSum = 0;
    while (!smallestM.isEmpty()) {
        minSum += smallestM.extractMax();
    }

    return maxSum - minSum;
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

void runTest(int arr[], int n, int m) {
    cout << "  Input   : ";
    printArray(arr, n);
    cout << "\n  Max Diff: " << maxDiffBetweenTwoMSizedSubsets(arr, n, m) << "\n\n";
}

int main() {
    cout << "Test 1 (Primary Example) (m = 2)\n";
    int a1[] = {1, 2, 3, 4, 5};
    runTest(a1, 5, 2);

    cout << "Test 2 (Unsorted Array) (m = 2)\n";
    int a2[] = {5, 8, 11, 40, 15};
    runTest(a2, 5, 2);

    cout << "Test 3 (Negatives & m = 1) (m = 1)\n";
    int a3[] = {10, -4, 7, 2};
    runTest(a3, 4, 1);

    cout << "Test 4 (Edge Case m = n) (m = 3)\n";
    int a4[] = {3, 1, 4};
    runTest(a4, 3, 3);

    cout << "Test 5 (All Equal) (m = 2)\n";
    int a5[] = {5, 5, 5, 5};
    runTest(a5, 4, 2);

    return 0;
}
```

---

## LeetCode Pattern Takeaways

Pairing a Min-Heap and a Max-Heap lets you track **both ends of a distribution** simultaneously in $O(n \log m)$:
- **LeetCode 2163 — Minimum Difference in Sums After Removal of Elements:** Uses prefix Max-Heap (smallest `n` elements) and suffix Min-Heap (largest `n` elements).
- **LeetCode 2558 / 502 — IPO / Take Gifts:** Uses dual heaps or heap-based selection of extreme subsets.
- **LeetCode 295 — Find Median from Data Stream:** Classic dual-heap (Max-Heap for lower half, Min-Heap for upper half).
