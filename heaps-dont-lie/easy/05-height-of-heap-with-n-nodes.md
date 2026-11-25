# 05. Height of a Complete Binary Tree (Heap) with N Nodes

- **Difficulty:** Easy
- **Reference:** [GeeksforGeeks — Height of a complete binary tree (or Heap) with N nodes](https://www.geeksforgeeks.org/dsa/height-complete-binary-tree-heap-n-nodes/)

---

## Problem Statement

Given `N` nodes in a binary heap (which is always a **complete binary tree**), find the **height** of the heap.

> **Height Convention Note:** In GeeksforGeeks and most competitive programming problems, the height of a tree is the **number of edges** on the longest root-to-leaf path:
> - `N = 1` (root only) -> height `= 0` edges (`1` level)
> - `N = 6` -> height `= 2` edges (`3` levels)
> - `N = 0` (empty) -> `-1` edges (`0` levels)
>
> *(If a problem defines height by **number of nodes/levels**, simply add `1` -> `floor(log2(N)) + 1`.)*

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
A binary heap is *never* skewed like an arbitrary binary search tree! Because a heap enforces the **complete binary tree** shape invariant:
- Every level `0, 1, 2, ..., h - 1` is **100% full** before a single node is placed on level `h`.
- Level `k` holds exactly `2^k` nodes when full.

### Deriving the Exact Math (Level-by-Level Heap Property)
Let's count the total nodes in a complete binary tree of edge-height `h`:
- **Minimum nodes** for height `h` (just 1 node on level `h`):
  $$N_{\min} = (1 + 2 + 4 + \dots + 2^{h-1}) + 1 = (2^h - 1) + 1 = 2^h$$
- **Maximum nodes** for height `h` (level `h` is completely full):
  $$N_{\max} = 1 + 2 + 4 + \dots + 2^h = 2^{h+1} - 1$$

So any heap of height `h` satisfies:
$$2^h \le N \le 2^{h+1} - 1 \quad \Longleftrightarrow \quad 2^h < N + 1 \le 2^{h+1}$$

Taking $\log_2$ across the inequalities gives **two equivalent closed-form formulas** for all integers $N \ge 1$:
- $h = \lfloor \log_2(N) \rfloor$
- $h = \lceil \log_2(N + 1) \rceil - 1$

### Min-Heap vs Max-Heap Decision Rule
Does Min-Heap vs Max-Heap change the height? **No!** Both Min-Heap and Max-Heap share the exact same **complete binary tree** shape for a given `N`. In our code, we also build an actual 1-indexed `MinHeap` of `N` nodes and walk from index `N` up to root `1` (`curr /= 2`) to confirm the structural edge count matches the formula.

### Why this beats naive approaches
- **Naive approach:** Build the tree/heap and run a full `O(N)` BFS or DFS traversal to measure depth.
- **Level-by-level / Parent-walk (`O(log N)` time, `O(1)` space):** Subtract `1, 2, 4, ...` per level or divide `N` by `2` until reaching `1`.
- **Closed-form formula (`O(1)` time, `O(1)` space):** `ceil(log2(N + 1)) - 1` answers instantly without touching a single node!

### Edge Cases
- **`N <= 0`:** Empty heap has `-1` edge height (must guard before `log2(0)`!).
- **Powers of 2 (`N = 2, 4, 8, 16`):** Just crossed into a new level! Height jumps by 1 (`N = 3 -> 1`, `N = 4 -> 2`).
- **One less than a power of 2 (`N = 1, 3, 7, 15`):** Last node of a full level (`2^(h+1) - 1`).

---

## Step-by-Step Trace

Using Test 5 from `main()`: `N = 6`.

### 1. Level-by-Level Derivation (`heightLevelByLevel(6)`)

| Step | `remaining` before | `levelCapacity` (`2^height`) | Condition `remaining > levelCapacity` | Action | `height` after |
| --- | --- | --- | --- | --- | --- |
| Start | `6` | `1` (Level 0) | `6 > 1` (True) | Level 0 full (`1` node). `remaining = 5`, `cap = 2` | `1` |
| Step 2 | `5` | `2` (Level 1) | `5 > 2` (True) | Level 1 full (`2` nodes). `remaining = 3`, `cap = 4` | `2` |
| Step 3 | `3` | `4` (Level 2) | `3 > 4` (False) | Stop! `3` nodes sit on Level 2 | **`2`** |

### 2. 1-Indexed Heap Parent Walk (`heightViaHeapParentWalk(6)`)
The deepest node in a 6-node heap is at 1-indexed position `curr = 6`:
- `curr = 6 -> curr / 2 = 3` (`edges = 1`)
- `curr = 3 -> curr / 2 = 1` (`edges = 2`)
- `curr == 1` (root reached!) -> **`2` edges**.

### 3. `O(1)` Formula (`heightFormula(6)`)
$$\lceil \log_2(6 + 1) \rceil - 1 = \lceil \log_2(7) \rceil - 1 = \lceil 2.807 \rceil - 1 = 3 - 1 = 2$$

All three approaches agree: **`2` edges (`3` levels)**.

---

## Time & Space Complexity

| Approach | Time Complexity | Space Complexity | Notes |
| --- | --- | --- | --- |
| `heightLevelByLevel(N)` | `O(log N)` | `O(1)` | Exact integer arithmetic; no floating-point issues |
| `heightViaHeapParentWalk(N)` | `O(N)` (to build) / `O(log N)` (walk) | `O(N)` | Demonstrates the property on a real `MinHeap` |
| `heightFormula(N)` | `O(1)` | `O(1)` | Closed-form `ceil(log2(N + 1)) - 1` |

---

## C++ Implementation

Source file: [05-height-of-heap-with-n-nodes.cpp](05-height-of-heap-with-n-nodes.cpp)

```cpp
#include <climits>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 1-indexed MinHeap class used to verify structural heap height directly.
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

// Approach 1: Level-by-level simulation using complete binary tree capacities.
// Level 0 holds 1 node, level 1 holds 2, level 2 holds 4, ..., level h holds 2^h.
// Returns edge-based height (0 for N = 1, -1 for N <= 0).
int heightLevelByLevel(int n) {
    if (n <= 0) return -1;

    int height = 0;
    int remaining = n;
    int levelCapacity = 1;

    while (remaining > levelCapacity) {
        remaining -= levelCapacity;
        levelCapacity *= 2;
        height++;
    }
    return height;
}

// Approach 2: Build an actual 1-indexed MinHeap with N nodes and walk from
// the deepest leaf (index N) up to the root (index 1) via parent = curr / 2.
int heightViaHeapParentWalk(int n) {
    if (n <= 0) return -1;

    MinHeap h;
    for (int val = n; val >= 1; val--) {
        h.insert(val);
    }

    int edges = 0;
    int curr = h.getSize();
    while (curr > 1) {
        curr /= 2;
        edges++;
    }
    return edges;
}

// Approach 3: O(1) closed-form calculation: ceil(log2(N + 1)) - 1 == floor(log2(N)).
int heightFormula(int n) {
    if (n <= 0) return -1;
    return (int)ceil(log2((double)(n + 1))) - 1;
}

void runTest(string label, int n) {
    int hLevel = heightLevelByLevel(n);
    int hWalk = heightViaHeapParentWalk(n);
    int hMath = heightFormula(n);

    cout << label << " (N = " << n << ")\n";
    cout << "  Level-by-level O(log N): " << hLevel << " edges";
    if (hLevel >= 0) cout << " (" << (hLevel + 1) << " levels)";
    cout << "\n";
    cout << "  Heap parent-walk       : " << hWalk << " edges\n";
    cout << "  O(1) formula           : " << hMath << " edges\n\n";
}

int main() {
    runTest("Test 1 (Single Node)", 1);
    runTest("Test 2 (2 Nodes)", 2);
    runTest("Test 3 (Full 2-Level Heap)", 3);
    runTest("Test 4 (First Node of Level 2)", 4);
    runTest("Test 5 (Classic Example: N = 6)", 6);
    runTest("Test 6 (Full 3-Level Heap)", 7);
    runTest("Test 7 (First Node of Level 3)", 8);
    runTest("Test 8 (Full 4-Level Heap)", 15);
    runTest("Test 9 (Large Heap)", 1000);
    runTest("Test 10 (Empty Heap)", 0);

    return 0;
}
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version removes `<vector>` and `<string>` completely and directly matches `Heap-Implementation-OPTIMIZED.cpp`:
- `MinHeap` stores nodes in a fixed-capacity 1-indexed array `int items[MAX]` (`#define MAX 1024`, enough to hold `N = 1000` in Test 9) with `int size`, `heapifyUp`, `minHeapify`, `insert`, `getMin`, `extractMin`, `isEmpty`, `getSize`, and `display`.
- `insert(int x)` checks `if (size < MAX - 1)` before writing `items[++size] = x`, and `heightViaHeapParentWalk(int n)` performs manual capacity guards (`n <= 0 || n >= MAX` and checking `!h.insert(val)`).
- Only `<climits>`, `<cmath>`, and `<iostream>` are included, with zero STL containers.

Source file: [05-height-of-heap-with-n-nodes-array.cpp](05-height-of-heap-with-n-nodes-array.cpp)

```cpp
#include <climits>
#include <cmath>
#include <iostream>
#define MAX 1024
using namespace std;

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

// Approach 1: Level-by-level simulation using complete binary tree capacities.
// Level 0 holds 1 node, level 1 holds 2, level 2 holds 4, ..., level h holds 2^h.
// Returns edge-based height (0 for N = 1, -1 for N <= 0).
int heightLevelByLevel(int n) {
    if (n <= 0) return -1;

    int height = 0;
    int remaining = n;
    int levelCapacity = 1;

    while (remaining > levelCapacity) {
        remaining -= levelCapacity;
        levelCapacity *= 2;
        height++;
    }
    return height;
}

// Approach 2: Build an actual 1-indexed MinHeap with N nodes and walk from
// the deepest leaf (index N) up to the root (index 1) via parent = curr / 2.
int heightViaHeapParentWalk(int n) {
    if (n <= 0 || n >= MAX) return -1;

    MinHeap h;
    for (int val = n; val >= 1; val--) {
        if (!h.insert(val)) return -1;
    }

    int edges = 0;
    int curr = h.getSize();
    while (curr > 1) {
        curr /= 2;
        edges++;
    }
    return edges;
}

// Approach 3: O(1) closed-form calculation: ceil(log2(N + 1)) - 1 == floor(log2(N)).
int heightFormula(int n) {
    if (n <= 0) return -1;
    return (int)ceil(log2((double)(n + 1))) - 1;
}

void runTest(int n) {
    int hLevel = heightLevelByLevel(n);
    int hWalk = heightViaHeapParentWalk(n);
    int hMath = heightFormula(n);

    cout << "  Level-by-level O(log N): " << hLevel << " edges";
    if (hLevel >= 0) cout << " (" << (hLevel + 1) << " levels)";
    cout << "\n";
    cout << "  Heap parent-walk       : " << hWalk << " edges\n";
    cout << "  O(1) formula           : " << hMath << " edges\n\n";
}

int main() {
    cout << "Test 1 (Single Node) (N = 1)\n";
    runTest(1);

    cout << "Test 2 (2 Nodes) (N = 2)\n";
    runTest(2);

    cout << "Test 3 (Full 2-Level Heap) (N = 3)\n";
    runTest(3);

    cout << "Test 4 (First Node of Level 2) (N = 4)\n";
    runTest(4);

    cout << "Test 5 (Classic Example: N = 6) (N = 6)\n";
    runTest(6);

    cout << "Test 6 (Full 3-Level Heap) (N = 7)\n";
    runTest(7);

    cout << "Test 7 (First Node of Level 3) (N = 8)\n";
    runTest(8);

    cout << "Test 8 (Full 4-Level Heap) (N = 15)\n";
    runTest(15);

    cout << "Test 9 (Large Heap) (N = 1000)\n";
    runTest(1000);

    cout << "Test 10 (Empty Heap) (N = 0)\n";
    runTest(0);

    return 0;
}
```

---

## LeetCode Pattern Takeaways

The **complete binary tree height = `floor(log2(N))`** invariant is why heap operations are fast and underpins:
- **LeetCode 222 — Count Complete Tree Nodes:** Uses the leftmost path height `h = floor(log2(N))` vs rightmost path height to count nodes in `O((log N)^2)` instead of `O(N)`.
- **Why Heap `insert` & `extract` are `O(log N)`:** Every percolate up/down travels at most the height of the heap (`floor(log2(N))` steps).
- **Floyd's `O(N)` `buildHeap` proof:** Uses the exact level-by-level capacity (`2^k` nodes at depth `k`, which sift down at most `h - k` levels) to prove $\sum k / 2^k = O(N)$.
