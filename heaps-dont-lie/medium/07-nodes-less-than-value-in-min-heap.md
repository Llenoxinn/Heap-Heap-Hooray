# Nodes Less Than a Value in a Min-Heap

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Print All Nodes Less Than a Value x in a Min Heap](https://www.geeksforgeeks.org/dsa/print-all-nodes-less-than-a-value-x-in-a-min-heap/)

---

## Problem Statement

Given a **Min-Heap** and a threshold value `x`, find (and print) all nodes in the Min-Heap whose values are strictly less than `x` (`node < x`).

**Example:**
- Input: Min-Heap built from `[2, 3, 15, 5, 4, 45, 80, 6, 150, 77, 120]`, `x = 10`
- Output: `[2, 3, 5, 6, 4]` (preorder order of all nodes `< 10`)

---

## Pattern Recognition & Intuition

- **Why think "Heap" (and Heap Pruning)?**
  If you ignored the heap property and scanned the underlying array from index `1` to `size`, you would spend $O(n)$ time even if only 3 nodes were smaller than `x` (or 0 nodes!).
  Because this is a **Min-Heap**, every parent is `<=` all descendants in its entire subtree. That gives us a powerful **branch-and-bound pruning rule**:
  > If `items[curr] >= x`, then **every** node in the subtree rooted at `curr` is also `>= x` — we can stop recursing into that subtree immediately!
- **Min-Heap vs Max-Heap Decision Rule:**
  This pruning trick works for `< x` *specifically* on a **Min-Heap** (because `parent <= children` guarantees subtree lower bounds). Symmetrically, finding nodes `> x` with pruning would require a **Max-Heap**.
- **Why it beats naive approaches:**
  - Linear scan over the heap array: always $O(n)$.
  - Preorder DFS with heap pruning: visits only the `k` matching nodes plus at most `k + 1` boundary children where `items[child] >= x`, achieving **$O(k)$ time**! When $k \ll n$ (or $k = 0$), it avoids touching almost the entire heap.
- **Key Invariant:**
  For any index `curr`, all values in the subtree rooted at `curr` are $\ge \text{items}[\text{curr}]$. Thus skipping a branch whenever `items[curr] >= x` never misses a valid node `< x`.
- **Edge Cases:**
  - `x <= root`: Root itself is pruned immediately — returns `[]` in $O(1)$!
  - Empty heap: `curr > size` returns immediately.
  - `x` larger than all elements: visits all `n` nodes in $O(n)$.
  - Note on `getNodesLessThan(x)`: the output order is preorder traversal of the pruned subtree, not sorted order.

---

## Step-by-Step Trace

Tracing **Test 1** from `main()`:
After inserting `{2, 3, 15, 5, 4, 45, 80, 6, 150, 77, 120}`, the 1-indexed Min-Heap array is:

`[2, 3, 15, 5, 4, 45, 80, 6, 150, 77, 120]` (indices `1..11`)

Tree view:
```text
               2 (idx 1)
           /               \
        3 (idx 2)         15 (idx 3)
       /       \         /          \
   5 (4)      4 (5)   45 (6)      80 (7)
   /   \      /   \
6(8) 150(9) 77(10) 120(11)
```

Query `x = 10` starting at `curr = 1`:
1. `curr = 1` (`2 < 10`): **collect `2`**, recurse left (`2`) and right (`3`).
2. `curr = 2` (`3 < 10`): **collect `3`**, recurse left (`4`) and right (`5`).
3. `curr = 4` (`5 < 10`): **collect `5`**, recurse left (`8`) and right (`9`).
4. `curr = 8` (`6 < 10`): **collect `6`**, left `16 > 11` (stop), right `17 > 11` (stop).
5. `curr = 9` (`150 >= 10`): **PRUNE!** Return immediately.
6. `curr = 5` (`4 < 10`): **collect `4`**, recurse left (`10`) and right (`11`).
7. `curr = 10` (`77 >= 10`): **PRUNE!**
8. `curr = 11` (`120 >= 10`): **PRUNE!**
9. `curr = 3` (`15 >= 10`): **PRUNE!** (Skips `15`, `45`, `80` in one check!)

**Result:** `[2, 3, 5, 6, 4]` (only 9 nodes inspected out of 11, and in a million-node heap with 5 nodes `< 10`, at most 11 nodes would be inspected!).

---

## Time & Space Complexity

- **Time Complexity:** $O(k)$ where `k` is the number of nodes strictly less than `x`.
  - Every recursive call that does *not* prune outputs one valid node (exactly `k` calls).
  - Each of those `k` nodes has at most 2 children, so at most `2k + 1` recursive calls happen in total (actually at most `k + 1` pruned leaf-boundary checks in a binary tree).
  - Worst case (when `x` exceeds all elements): $O(n)$.
- **Space Complexity:** $O(h) = O(\log n)$ recursion stack space (since a binary heap is a complete binary tree of height $\lfloor \log_2 n \rfloor$), plus $O(k)$ for the returned vector.

---

## C++ Implementation

See [07-nodes-less-than-value-in-min-heap.cpp](07-nodes-less-than-value-in-min-heap.cpp).

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

    void collectLessThan(int curr, int x, vector<int>& out) {
        if (curr > size) {
            return;
        }
        if (items[curr] >= x) {
            return;
        }
        out.push_back(items[curr]);
        collectLessThan(2 * curr, x, out);
        collectLessThan(2 * curr + 1, x, out);
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

    vector<int> getNodesLessThan(int x) {
        vector<int> out;
        collectLessThan(1, x, out);
        return out;
    }

    void displayHeapArray() {
        cout << "[";
        for (int i = 1; i <= size; i++) {
            cout << items[i];
            if (i < size) {
                cout << ", ";
            }
        }
        cout << "]";
    }
};

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

void runQuery(MinHeap& heap, string label, int x) {
    vector<int> ans = heap.getNodesLessThan(x);
    cout << label << " (x = " << x << ") -> ";
    printArray(ans);
    cout << "\n";
}

int main() {
    MinHeap heap;
    vector<int> values = {2, 3, 15, 5, 4, 45, 80, 6, 150, 77, 120};
    for (int i = 0; i < (int)values.size(); i++) {
        heap.insert(values[i]);
    }

    cout << "Min-Heap array (1-indexed level order): ";
    heap.displayHeapArray();
    cout << "\n\n";

    runQuery(heap, "Test 1 (Primary Example)", 10);
    runQuery(heap, "Test 2 (Root Pruned Immediately)", 2);
    runQuery(heap, "Test 3 (Threshold 50)", 50);
    runQuery(heap, "Test 4 (Threshold 200 - All Nodes)", 200);

    return 0;
}
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts `MinHeap` and the pruned DFS query to fixed-size arrays matching `Heap-Implementation-OPTIMIZED.cpp` without `std::vector` or `std::string`:
- `MinHeap` uses `int items[MAX]` (`#define MAX 100`) and `int size`, implementing 1-indexed `heapifyUp`, `minHeapify`, `insert`, `getMin`, `extractMin`, `isEmpty`, `getSize`, `display`, and `displayHeapArray`.
- `collectLessThan(int curr, int x, int out[], int& outCount)` and `getNodesLessThan(int x, int out[])` write matching values directly into a raw `int out[]` array and return `outCount`.

See [07-nodes-less-than-value-in-min-heap-array.cpp](07-nodes-less-than-value-in-min-heap-array.cpp).

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

    void collectLessThan(int curr, int x, int out[], int& outCount) {
        if (curr > size) {
            return;
        }
        if (items[curr] >= x) {
            return;
        }
        out[outCount++] = items[curr];
        collectLessThan(2 * curr, x, out, outCount);
        collectLessThan(2 * curr + 1, x, out, outCount);
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

    int getNodesLessThan(int x, int out[]) {
        int outCount = 0;
        collectLessThan(1, x, out, outCount);
        return outCount;
    }

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << items[i] << " ";
        }
        cout << endl;
    }

    void displayHeapArray() {
        cout << "[";
        for (int i = 1; i <= size; i++) {
            cout << items[i];
            if (i < size) {
                cout << ", ";
            }
        }
        cout << "]";
    }
};

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

void runQuery(MinHeap& heap, int x) {
    int ans[MAX];
    int count = heap.getNodesLessThan(x, ans);
    cout << " (x = " << x << ") -> ";
    printArray(ans, count);
    cout << "\n";
}

int main() {
    MinHeap heap;
    int values[] = {2, 3, 15, 5, 4, 45, 80, 6, 150, 77, 120};
    for (int i = 0; i < 11; i++) {
        heap.insert(values[i]);
    }

    cout << "Min-Heap array (1-indexed level order): ";
    heap.displayHeapArray();
    cout << "\n\n";

    cout << "Test 1 (Primary Example)";
    runQuery(heap, 10);

    cout << "Test 2 (Root Pruned Immediately)";
    runQuery(heap, 2);

    cout << "Test 3 (Threshold 50)";
    runQuery(heap, 50);

    cout << "Test 4 (Threshold 200 - All Nodes)";
    runQuery(heap, 200);

    return 0;
}
```

---

## LeetCode Pattern Takeaways

Understanding **internal heap structure invariants** (`parent <= subtree`) unlocks output-sensitive $O(k)$ queries without popping elements:
- **Range / Threshold Queries on Heaps:** Any time a Min-Heap must stay intact while querying values `< x`, prune with DFS rather than popping and re-inserting.
- **LeetCode 378 / 373 — Kth Smallest in Sorted Matrix / Pairs:** Uses the same monotonic boundary pruning principle (`>= target` terminates exploration).
- **LeetCode 653 / 938 — Range Sum of BST:** Analogous structural pruning (`val < low` prunes left, `val > high` prunes right).
