# Connect n Ropes with Minimum Cost

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Connect n Ropes with Minimum Cost](https://www.geeksforgeeks.org/dsa/connect-n-ropes-minimum-cost/)

---

## Problem Statement

Given `n` ropes of different lengths stored in an array `ropes`, connect all `n` ropes into a single rope. The cost to connect two ropes is equal to the sum of their lengths. Find the **minimum total cost** to connect all the ropes.

**Example:**
- Input: `ropes = [4, 3, 2, 6]`
- Output: `29`
  - Connect `2` and `3` → cost `5`, ropes become `[4, 5, 6]`
  - Connect `4` and `5` → cost `9`, ropes become `[6, 9]`
  - Connect `6` and `9` → cost `15`, ropes become `[15]`
  - Total cost = `5 + 9 + 15 = 29`

---

## Pattern Recognition & Intuition

- **Why think "Heap"?**
  Notice what happens when you merge two ropes early: their lengths get **re-counted** in every subsequent merge along their path to the final root! Specifically, if rope `i` participates in `d_i` merges, total cost is $\sum \text{length}_i \cdot d_i$.
  To minimize that weighted sum, the **shortest ropes** should be merged earliest (so larger ropes accumulate smaller depths `d_i`). After every merge, a new combined rope enters the pool, so we need a structure that dynamically gives us the **two smallest ropes** in $O(\log n)$ — a **Min-Heap**!
- **Min-Heap vs Max-Heap Decision Rule:**
  Always pick the **two shortest** ropes at each step → **Min-Heap**. (Using a Max-Heap would compute the *maximum* cost instead.)
- **Why it beats naive approaches:**
  - Sorting once doesn't work because the newly merged rope `(a + b)` may be shorter or longer than remaining ropes, so order changes dynamically.
  - Re-sorting or linear scanning on every merge takes $O(n^2)$ or $O(n^2 \log n)$.
  - Min-Heap maintains dynamic sorted minimum access in $O(n \log n)$ total time.
- **Key Invariant:**
  At the start of every iteration of `while (minHeap.getSize() > 1)`, the Min-Heap contains the exact multiset of current rope lengths, and `totalCost` equals the optimal cost paid to reach this state.
- **Edge Cases:**
  - `n == 0` or `n == 1`: 0 connections needed → cost is `0`.
  - All ropes equal length (e.g., `[5, 5, 5, 5]`): optimal merge forms a balanced binary tree (`10 + 10 + 20 = 40`), whereas greedy sequential chaining (`10 + 15 + 20 = 45`) would be suboptimal — the Min-Heap automatically discovers the balanced merge!
  - Large lengths/sums: store values as `long long` in the Min-Heap to prevent 32-bit overflow.

---

## Step-by-Step Trace

Tracing **Test 1** from `main()`: `ropes = [4, 3, 2, 6]`.

1. **Insert all ropes into Min-Heap:**
   - Heap: `[2, 3, 4, 6]`
   - `totalCost = 0`

2. **Step 1 (`size = 4 > 1`):**
   - `first = extractMin()` = `2`
   - `second = extractMin()` = `3`
   - `merged = 2 + 3 = 5`
   - `totalCost += 5` → `totalCost = 5`
   - `insert(5)` → Heap: `[4, 5, 6]`

3. **Step 2 (`size = 3 > 1`):**
   - `first = extractMin()` = `4`
   - `second = extractMin()` = `5`
   - `merged = 4 + 5 = 9`
   - `totalCost += 9` → `totalCost = 14`
   - `insert(9)` → Heap: `[6, 9]`

4. **Step 3 (`size = 2 > 1`):**
   - `first = extractMin()` = `6`
   - `second = extractMin()` = `9`
   - `merged = 6 + 9 = 15`
   - `totalCost += 15` → `totalCost = 29`
   - `insert(15)` → Heap: `[15]`

5. **`size = 1` → loop ends.** Return **`29`**.

---

## Time & Space Complexity

- **Time Complexity:** $O(n \log n)$
  - Building the Min-Heap from `n` elements takes $O(n \log n)$ via repeated inserts (or $O(n)$ with bottom-up `minHeapify`).
  - There are `n - 1` merge steps; each performs 2 `extractMin` and 1 `insert` operations at $O(\log n)$ each.
- **Space Complexity:** $O(n)$
  - The Min-Heap stores at most `n` rope lengths.

---

## C++ Implementation

See [08-connect-n-ropes-minimum-cost.cpp](08-connect-n-ropes-minimum-cost.cpp).

```cpp
#include <iostream>
#include <vector>
using namespace std;

class MinHeap {
private:
    vector<long long> items;
    int size;

    void heapifyUp(int curr) {
        long long x = items[curr];
        while (curr / 2 > 0 && x < items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        long long x = items[curr];
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

    void insert(long long x) {
        items.push_back(x);
        size++;
        heapifyUp(size);
    }

    long long getMin() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    long long extractMin() {
        if (size == 0) {
            return -1;
        }
        long long minVal = items[1];
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

long long minCostToConnectRopes(vector<int>& ropes) {
    if ((int)ropes.size() <= 1) {
        return 0;
    }

    MinHeap minHeap;
    for (int i = 0; i < (int)ropes.size(); i++) {
        minHeap.insert((long long)ropes[i]);
    }

    long long totalCost = 0;
    while (minHeap.getSize() > 1) {
        long long first = minHeap.extractMin();
        long long second = minHeap.extractMin();
        long long merged = first + second;
        totalCost += merged;
        minHeap.insert(merged);
    }

    return totalCost;
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

void runTest(string label, vector<int> ropes) {
    cout << label << "\n";
    cout << "  Ropes   : ";
    printArray(ropes);
    cout << "\n  Min Cost: " << minCostToConnectRopes(ropes) << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Example)", {4, 3, 2, 6});
    runTest("Test 2 (Sorted 1..5)", {1, 2, 3, 4, 5});
    runTest("Test 3 (Equal Ropes)", {5, 5, 5, 5});
    runTest("Test 4 (Single Rope)", {10});
    runTest("Test 5 (Empty Input)", {});
    return 0;
}
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts `MinHeap` and `minCostToConnectRopes` to fixed-size raw arrays matching `Heap-Implementation-OPTIMIZED.cpp`, with no `std::vector` or `std::string`:
- `MinHeap` stores `long long items[MAX]` (`#define MAX 100`) and `int size`, implementing 1-indexed `heapifyUp`, `minHeapify`, `insert`, `getMin`, `extractMin`, `isEmpty`, `getSize`, and `display`.
- `minCostToConnectRopes(int ropes[], int n)` accepts a raw `int ropes[]` array and length `n`, returning `0` when `n <= 1` and greedily merging the two smallest ropes using the fixed-capacity heap.

See [08-connect-n-ropes-minimum-cost-array.cpp](08-connect-n-ropes-minimum-cost-array.cpp).

```cpp
#include <iostream>
#define MAX 100
using namespace std;

class MinHeap {
private:
    long long items[MAX];
    int size;

    void heapifyUp(int curr) {
        long long x = items[curr];
        while (curr / 2 > 0 && x < items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        long long x = items[curr];
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

    bool insert(long long x) {
        if (size < MAX - 1) {
            items[++size] = x;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    long long getMin() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    long long extractMin() {
        if (size == 0) {
            return -1;
        }
        long long minVal = items[1];
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

long long minCostToConnectRopes(int ropes[], int n) {
    if (n <= 1) {
        return 0;
    }

    MinHeap minHeap;
    for (int i = 0; i < n; i++) {
        minHeap.insert((long long)ropes[i]);
    }

    long long totalCost = 0;
    while (minHeap.getSize() > 1) {
        long long first = minHeap.extractMin();
        long long second = minHeap.extractMin();
        long long merged = first + second;
        totalCost += merged;
        minHeap.insert(merged);
    }

    return totalCost;
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

void runTest(int ropes[], int n) {
    cout << "  Ropes   : ";
    printArray(ropes, n);
    cout << "\n  Min Cost: " << minCostToConnectRopes(ropes, n) << "\n\n";
}

int main() {
    cout << "Test 1 (Primary Example)\n";
    int r1[] = {4, 3, 2, 6};
    runTest(r1, 4);

    cout << "Test 2 (Sorted 1..5)\n";
    int r2[] = {1, 2, 3, 4, 5};
    runTest(r2, 5);

    cout << "Test 3 (Equal Ropes)\n";
    int r3[] = {5, 5, 5, 5};
    runTest(r3, 4);

    cout << "Test 4 (Single Rope)\n";
    int r4[] = {10};
    runTest(r4, 1);

    cout << "Test 5 (Empty Input)\n";
    int r5[1] = {0};
    runTest(r5, 0);

    return 0;
}
```

---

## LeetCode Pattern Takeaways

"Connect `n` ropes" is literally **LeetCode 1167 — Minimum Cost to Connect Sticks**, and is mathematically identical to optimal Huffman merging:
- **LeetCode 1167 — Minimum Cost to Connect Sticks:** Identical problem statement and Min-Heap greedy solution.
- **LeetCode 1000 — Minimum Cost to Merge Stones:** Note the critical twist! When you can only merge **adjacent** elements (`K` at a time), greedy heap merging fails and you must use **Interval DP** instead. Always check whether you can merge *any* two items (Heap) vs only *adjacent* items (Interval DP)!
- **LeetCode 1046 — Last Stone Weight:** Same repeated two-element extraction & re-insertion loop, using a Max-Heap.
