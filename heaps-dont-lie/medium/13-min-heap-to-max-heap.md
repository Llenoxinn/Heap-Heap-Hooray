# 13. Convert Min Heap to Max Heap

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Convert min Heap to max Heap](https://www.geeksforgeeks.org/dsa/convert-min-heap-to-max-heap/)

---

## Problem Statement

Given an array representation of a **Min-Heap** (0-indexed, size `N`), convert it **in-place** into a valid **Max-Heap** in $O(N)$ time.

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
At first glance, students often try to:
1. Extract elements one by one from the Min-Heap and insert them into a Max-Heap ($O(N \log N)$ time and $O(N)$ extra space), OR
2. Sort the array in descending order ($O(N \log N)$ time — since any descending array is trivially a Max-Heap).

Both are suboptimal! Why? Because the input is **already a complete binary tree** in array form. To turn *any* arbitrary complete binary tree into a Max-Heap, you don't care that it used to be a Min-Heap — you simply run **bottom-up `buildMaxHeap`** (`maxHeapify` from the last internal node `N / 2 - 1` up to `0`)!

### Min-Heap vs Max-Heap Decision Rule
- Bottom-up `maxHeapify` works because every leaf (`i >= N / 2`) is already a valid 1-node Max-Heap.
- Moving from `i = N / 2 - 1` down to `0`, both subtrees `2*i + 1` and `2*i + 2` are guaranteed to already be Max-Heaps when we call `maxHeapify(arr, n, i)`.
- Why is bottom-up `buildMaxHeap` **$O(N)$** instead of $O(N \log N)$? Because half the nodes are leaves ($0$ work), a quarter sift down at most $1$ level, an eighth at most $2$ levels, and $\sum_{h=0}^{\infty} \frac{h}{2^{h+1}} = 1$, so the total work is bounded by $2N = O(N)$!

### Key Invariants & Edge Cases
- **0-Indexed Children:** `left = 2 * i + 1`, `right = 2 * i + 2`, last non-leaf `n / 2 - 1`.
- **In-Place Property:** No auxiliary heap or array is needed; swaps happen directly in `arr`.
- **Edge Cases:** `N <= 1` (`n / 2 - 1 < 0`, nothing to do), duplicate values (using `>` ensures stability and valid Max-Heap `parent >= child`), negative numbers.

---

## Step-by-Step Trace

Using **Test 1** from `main()`:
- Input Min-Heap: `[3, 5, 9, 6, 8, 20, 10, 12, 18, 9]` (`N = 10`, internal nodes `i = 4` down to `0`)

1. **`i = 4` (`arr[4] = 8`, left child `arr[9] = 9`):**
   - `9 > 8` -> swap `arr[4]` and `arr[9]`.
   - Array: `[3, 5, 9, 6, 9, 20, 10, 12, 18, 8]`

2. **`i = 3` (`arr[3] = 6`, children `arr[7] = 12`, `arr[8] = 18`):**
   - Largest child is `18` (`arr[8]`) -> swap `arr[3]` and `arr[8]`.
   - Array: `[3, 5, 9, 18, 9, 20, 10, 12, 6, 8]`

3. **`i = 2` (`arr[2] = 9`, children `arr[5] = 20`, `arr[6] = 10`):**
   - Largest child is `20` (`arr[5]`) -> swap `arr[2]` and `arr[5]`.
   - Array: `[3, 5, 20, 18, 9, 9, 10, 12, 6, 8]`

4. **`i = 1` (`arr[1] = 5`, children `arr[3] = 18`, `arr[4] = 9`):**
   - Swap `arr[1]` (`5`) and `arr[3]` (`18`), then sift `5` down at `i = 3` (children `arr[7] = 12`, `arr[8] = 6`) -> swap `arr[3]` (`5`) and `arr[7]` (`12`).
   - Array: `[3, 18, 20, 12, 9, 9, 10, 5, 6, 8]`

5. **`i = 0` (`arr[0] = 3`, children `arr[1] = 18`, `arr[2] = 20`):**
   - Swap `arr[0]` (`3`) and `arr[2]` (`20`), then sift `3` down at `i = 2` (children `arr[5] = 9`, `arr[6] = 10`) -> swap `arr[2]` (`3`) and `arr[6]` (`10`).
   - Array: `[20, 18, 10, 12, 9, 9, 3, 5, 6, 8]`

**Final Output:** `[20, 18, 10, 12, 9, 9, 3, 5, 6, 8]` (valid Max-Heap!)

---

## Time & Space Complexity

- **Time Complexity:** $O(N)$
  - Tight bound from Floyd's bottom-up heap construction algorithm ($\sum_{h=0}^{\lfloor \log_2 N \rfloor} \lceil N / 2^{h+1} \rceil \cdot h \le N$).
- **Space Complexity:** $O(1)$ extra space (ignoring $O(\log N)$ call stack recursion in `maxHeapify`, which can also be written iteratively as in `Heap-Implementation-OPTIMIZED.cpp`).

---

## C++ Implementation

Source file: [13-min-heap-to-max-heap.cpp](13-min-heap-to-max-heap.cpp)

```cpp
#include <iostream>
#include <vector>
using namespace std;

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

void convertMinToMaxHeap(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, n, i);
    }
}

bool isMaxHeap(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i <= n / 2 - 1; i++) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[i] < arr[left]) return false;
        if (right < n && arr[i] < arr[right]) return false;
    }
    return true;
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    vector<int> h1 = {3, 5, 9, 6, 8, 20, 10, 12, 18, 9};
    cout << "Test 1 Min-Heap: ";
    printVector(h1);
    convertMinToMaxHeap(h1);
    cout << "\nTest 1 Max-Heap: ";
    printVector(h1);
    cout << " (valid Max-Heap: " << (isMaxHeap(h1) ? "true" : "false") << ")\n\n";

    vector<int> h2 = {1, 3, 6, 5, 9, 8};
    cout << "Test 2 Min-Heap: ";
    printVector(h2);
    convertMinToMaxHeap(h2);
    cout << "\nTest 2 Max-Heap: ";
    printVector(h2);
    cout << " (valid Max-Heap: " << (isMaxHeap(h2) ? "true" : "false") << ")\n";

    return 0;
}
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the solution to pure fixed-size raw C++ arrays (`#define MAX 100`, `int arr[], int n` parameters) without `<vector>` or any STL containers:
- All helper functions (`maxHeapify`, `convertMinToMaxHeap`, `isMaxHeap`, `printArray`) operate directly on `int arr[], int n`.
- `maxHeapify(int arr[], int n, int i)` uses the iterative shift-down pattern from `Heap-Implementation-OPTIMIZED.cpp`, keeping auxiliary space strictly $O(1)$.

Source file: [13-min-heap-to-max-heap-array.cpp](13-min-heap-to-max-heap-array.cpp)

```cpp
#include <iostream>
#define MAX 100
using namespace std;

void maxHeapify(int arr[], int n, int i) {
    int x = arr[i];
    int curr = i;
    while (2 * curr + 1 < n) {
        int child = 2 * curr + 1;
        if (child + 1 < n && arr[child + 1] > arr[child]) {
            child++;
        }
        if (arr[child] > x) {
            arr[curr] = arr[child];
            curr = child;
        } else {
            break;
        }
    }
    arr[curr] = x;
}

void convertMinToMaxHeap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        maxHeapify(arr, n, i);
    }
}

bool isMaxHeap(int arr[], int n) {
    for (int i = 0; i <= n / 2 - 1; i++) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[i] < arr[left]) return false;
        if (right < n && arr[i] < arr[right]) return false;
    }
    return true;
}

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    int h1[MAX] = {3, 5, 9, 6, 8, 20, 10, 12, 18, 9};
    int n1 = 10;
    cout << "Test 1 Min-Heap: ";
    printArray(h1, n1);
    convertMinToMaxHeap(h1, n1);
    cout << "\nTest 1 Max-Heap: ";
    printArray(h1, n1);
    cout << " (valid Max-Heap: " << (isMaxHeap(h1, n1) ? "true" : "false") << ")\n\n";

    int h2[MAX] = {1, 3, 6, 5, 9, 8};
    int n2 = 6;
    cout << "Test 2 Min-Heap: ";
    printArray(h2, n2);
    convertMinToMaxHeap(h2, n2);
    cout << "\nTest 2 Max-Heap: ";
    printArray(h2, n2);
    cout << " (valid Max-Heap: " << (isMaxHeap(h2, n2) ? "true" : "false") << ")\n";

    return 0;
}
```

---

## LeetCode Pattern Takeaways

Recognizing when **$O(N)$ bottom-up `heapify`** beats $O(N \log N)$ repeated insertion is crucial for:
- **Heap Sort** (`heapSort.cpp`) — starts with `O(N)` `buildMaxHeap`/`buildMinHeap`
- **LeetCode 215** — *Kth Largest Element in an Array* (`O(N + K log N)` via bottom-up heapify + `K` pops)
- **LeetCode 1046** — *Last Stone Weight* (can initialize heap in `O(N)`)
- **LeetCode 1337** — *The K Weakest Rows in a Matrix*
