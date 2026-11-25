# 14. Check for Min-Heap from Level Order

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Given level order traversal of a Binary Tree, check if the Tree is a Min-Heap](https://www.geeksforgeeks.org/dsa/given-level-order-traversal-binary-tree-check-tree-min-heap/)

---

## Problem Statement

Given the level-order traversal of a **complete binary tree** stored in an array `level` of size `N` (0-indexed), determine whether the binary tree satisfies the **Min-Heap property** (`parent <= leftChild` and `parent <= rightChild` for every node).

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
A binary heap is defined by **two properties**:
1. **Structural property:** It is a complete binary tree.
2. **Heap-order property:** Every parent is `<=` its children (for a Min-Heap).

Because the input is already the level-order traversal of a complete binary tree, the structural property is satisfied and the parent-child relationships map directly to array indices:
- For 0-indexed node `i`:
  - `left = 2 * i + 1`
  - `right = 2 * i + 2`
- All indices from `N / 2` to `N - 1` are **leaf nodes** (they have no children, so they trivially satisfy the heap property).
- Therefore, we only need to validate **internal nodes** from `i = N / 2 - 1` down to `0` (or `0` to `N / 2 - 1`).

### Min-Heap vs Max-Heap Decision Rule
- We are validating a **Min-Heap**, so at every internal index `i`:
  - If `left < N && level[i] > level[left]`, return `false`.
  - If `right < N && level[i] > level[right]`, return `false`.
- To check for a **Max-Heap**, simply flip the comparison to `level[i] < level[left]` and `level[i] < level[right]`.

### Key Invariants & Edge Cases
- **Bounds Check on Right Child:** The last internal node `i = N / 2 - 1` might have only a left child (when `N` is even). Always check `right < N` before indexing `level[right]`.
- **Leaf Shortcut:** Checking only `0 <= i <= N / 2 - 1` avoids redundant out-of-bounds checks for leaves.
- **Empty or Single-Node Tree:** When `N <= 1`, `N / 2 - 1 < 0`, so the loop doesn't execute and it returns `true`.
- **Equal Values:** Duplicates are valid in a Min-Heap (`parent == child` is allowed; only `parent > child` violates the rule).

---

## Step-by-Step Trace

### Test 1: `level = [10, 15, 14, 25, 30]` (`N = 5`, internal nodes: `i = 1, 0`)

- `i = 1` (`level[1] = 15`):
  - `left = 3` (`25`): `15 <= 25` (ok)
  - `right = 4` (`30`): `15 <= 30` (ok)
- `i = 0` (`level[0] = 10`):
  - `left = 1` (`15`): `10 <= 15` (ok)
  - `right = 2` (`14`): `10 <= 14` (ok)
- **Result:** `Yes (Min-Heap)`

### Test 2: `level = [30, 56, 22, 49, 30, 51, 2]` (`N = 7`, internal nodes: `i = 2, 1, 0`)

- `i = 2` (`level[2] = 22`):
  - `left = 5` (`51`): `22 <= 51` (ok)
  - `right = 6` (`2`): `22 > 2` -> **Violation!** Return `false` immediately.
- **Result:** `No (Not a Min-Heap)`

---

## Time & Space Complexity

- **Time Complexity:** $O(N)$ — checks at most $N / 2$ internal nodes, doing $O(1)$ comparisons per node.
- **Space Complexity:** $O(1)$ — no extra memory allocated beyond loop variables.

---

## C++ Implementation

Source file: [14-check-min-heap-from-level-order.cpp](14-check-min-heap-from-level-order.cpp)

```cpp
#include <iostream>
#include <vector>
using namespace std;

bool isMinHeapFromLevelOrder(vector<int>& level) {
    int n = (int)level.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && level[i] > level[left]) {
            return false;
        }
        if (right < n && level[i] > level[right]) {
            return false;
        }
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
    vector<int> t1 = {10, 15, 14, 25, 30};
    cout << "Test 1 ";
    printVector(t1);
    cout << " -> " << (isMinHeapFromLevelOrder(t1) ? "Yes (Min-Heap)" : "No (Not a Min-Heap)") << "\n";

    vector<int> t2 = {30, 56, 22, 49, 30, 51, 2};
    cout << "Test 2 ";
    printVector(t2);
    cout << " -> " << (isMinHeapFromLevelOrder(t2) ? "Yes (Min-Heap)" : "No (Not a Min-Heap)") << "\n";

    vector<int> t3 = {1, 3, 6, 5, 9, 8};
    cout << "Test 3 ";
    printVector(t3);
    cout << " -> " << (isMinHeapFromLevelOrder(t3) ? "Yes (Min-Heap)" : "No (Not a Min-Heap)") << "\n";

    return 0;
}
```

---

## LeetCode Pattern Takeaways

Understanding **0-indexed / 1-indexed array tree geometry** and **completeness validation** is essential for:
- **LeetCode 958** — *Check Completeness of a Binary Tree*
- **GeeksforGeeks** — *Is Binary Tree Heap* (combines completeness check + heap-order check on pointer-based `TreeNode*` trees)
- **LeetCode 297** — *Serialize and Deserialize Binary Tree* (level-order tree representations)
