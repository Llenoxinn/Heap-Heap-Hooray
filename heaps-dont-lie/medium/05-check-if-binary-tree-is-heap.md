# Check if Binary Tree is a Heap

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Check if a Given Binary Tree is Heap](https://www.geeksforgeeks.org/dsa/check-if-a-given-binary-tree-is-heap/)

---

## Problem Statement

Given the root of a binary tree, determine whether it represents a valid **Max-Heap**.

A binary tree is a valid Max-Heap if and only if:
1. **Structural Property:** It is a **Complete Binary Tree (CBT)** — every level is completely filled except possibly the last, and all nodes on the last level are as far left as possible.
2. **Heap-Order Property:** Every node's value is greater than or equal to the values of its children (`node->val >= child->val`).

**Example:**
```text
          97
        /    \
      46      37
     /  \    /  \
    12   3  7   31
   /  \
  6    9
```
- Output: `true` (complete tree of 9 nodes, and every parent `>=` its children)

---

## Pattern Recognition & Intuition

- **Why think "Heap"?**
  This tests the **exact two-part definition** of a binary heap (structural completeness + local parent-child ordering). Many students mistakenly check *only* `parent >= children` and forget completeness — for example, a skewed chain `50 -> right: 30 -> right: 20` satisfies `parent >= child` everywhere, yet it is **not** a heap because it cannot map compactly to indices `0..n-1`!
- **Min-Heap vs Max-Heap Decision Rule:**
  - The standard GFG problem asks whether a binary tree is a **Max-Heap** (`parent >= children`).
  - Checking for a Min-Heap simply flips `>=` to `<=` inside `hasMaxHeapOrder`.
- **Why the index trick beats BFS for completeness:**
  - In a 0-indexed array representation of a Complete Binary Tree with `N` nodes, node indices form a gapless contiguous block `0, 1, ..., N - 1` (left child = `2*i + 1`, right child = `2*i + 2`).
  - If *any* node receives an index `>= N`, there must be a missing slot ("hole") earlier in level order, proving the tree is **not** complete!
  - And once completeness is confirmed, `hasMaxHeapOrder` can cleanly assume that if `root->right == nullptr`, `root->left` is either a leaf or `nullptr` (because a complete tree cannot have `root->left->left` while `root->right == nullptr`).
- **Key Invariant:**
  `countNodes` gives `totalNodes = N`. `isCompleteTree(root, 0, N)` guarantees a bijection onto array indices `[0, N - 1]`, and `hasMaxHeapOrder(root)` enforces `node->val >= left->val` and `node->val >= right->val` at every node.
- **Edge Cases:**
  - `root == nullptr` (empty tree): vacuously a valid heap (`true`).
  - Single node: complete (`index 0 < 1`) and no children (`true`).
  - Left missing, right present: `right` gets index `2*i + 2 >= N` → caught immediately as `false`.
  - Equal parent and child values: allowed (`>=`), since heaps permit duplicate keys.

---

## Step-by-Step Trace

Tracing **Test 1** from `main()` (`totalNodes = 9`):

1. **Completeness Check (`isCompleteTree(root, index, 9)`):**
   - `97` → `index = 0 < 9`
   - `46` → `index = 1 < 9`, `37` → `index = 2 < 9`
   - `12` → `index = 3 < 9`, `3` → `index = 4 < 9`, `7` → `index = 5 < 9`, `31` → `index = 6 < 9`
   - `6` → `index = 7 < 9`, `9` → `index = 8 < 9`
   - All 9 nodes occupy indices `0..8` with no index `>= 9` → **Complete!**

2. **Max-Heap Order Check (`hasMaxHeapOrder(root)`):**
   - Node `97`: `97 >= 46` and `97 >= 37` ✓
   - Node `46`: `46 >= 12` and `46 >= 3` ✓
   - Node `12`: `12 >= 6` and `12 >= 9` ✓
   - Leaves `6, 9, 3`: trivially `true` ✓
   - Node `37`: `37 >= 7` and `37 >= 31` ✓
   - Leaves `7, 31`: trivially `true` ✓

Both checks return `true` → **`isBinaryTreeHeap(t1) = true`**.

*(Compare with **Test 2**: `t2` has 4 nodes `[50, 30, 20, right-of-30: 10]`. Node `10` gets index `2*(1) + 2 = 4 >= 4`, so `isCompleteTree` immediately returns `false`!)*

---

## Time & Space Complexity

- **Time Complexity:** $O(n)$
  - `countNodes`: visits each of the `n` nodes once → $O(n)$.
  - `isCompleteTree`: visits each node once → $O(n)$.
  - `hasMaxHeapOrder`: visits each node once → $O(n)$.
- **Space Complexity:** $O(h)$
  - Recursion stack depth equals tree height `h` ($O(\log n)$ for a valid complete tree, up to $O(n)$ for a skewed non-heap input).

---

## C++ Implementation

See [05-check-if-binary-tree-is-heap.cpp](05-check-if-binary-tree-is-heap.cpp).

```cpp
#include <iostream>
#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int v) {
        val = v;
        left = nullptr;
        right = nullptr;
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

    int keyAt(int idx) {
        return items[idx];
    }
};

int countNodes(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

bool isCompleteTree(TreeNode* root, int index, int totalNodes) {
    if (root == nullptr) {
        return true;
    }
    if (index >= totalNodes) {
        return false;
    }
    return isCompleteTree(root->left, 2 * index + 1, totalNodes) &&
           isCompleteTree(root->right, 2 * index + 2, totalNodes);
}

bool hasMaxHeapOrder(TreeNode* root) {
    if (root == nullptr) {
        return true;
    }
    if (root->left == nullptr && root->right == nullptr) {
        return true;
    }
    if (root->right == nullptr) {
        return root->val >= root->left->val;
    }
    if (root->val >= root->left->val && root->val >= root->right->val) {
        return hasMaxHeapOrder(root->left) && hasMaxHeapOrder(root->right);
    }
    return false;
}

bool isBinaryTreeHeap(TreeNode* root) {
    int totalNodes = countNodes(root);
    return isCompleteTree(root, 0, totalNodes) && hasMaxHeapOrder(root);
}

TreeNode* buildTreeFromMaxHeap(MaxHeap& heap, int idx) {
    if (idx > heap.getSize()) {
        return nullptr;
    }
    TreeNode* node = new TreeNode(heap.keyAt(idx));
    node->left = buildTreeFromMaxHeap(heap, 2 * idx);
    node->right = buildTreeFromMaxHeap(heap, 2 * idx + 1);
    return node;
}

void freeTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    /* Test 1 (Primary Example): Valid Max-Heap
              97
            /    \
          46      37
         /  \    /  \
        12   3  7   31
       /  \
       6   9
    */
    TreeNode* t1 = new TreeNode(97);
    t1->left = new TreeNode(46);
    t1->right = new TreeNode(37);
    t1->left->left = new TreeNode(12);
    t1->left->right = new TreeNode(3);
    t1->right->left = new TreeNode(7);
    t1->right->right = new TreeNode(31);
    t1->left->left->left = new TreeNode(6);
    t1->left->left->right = new TreeNode(9);
    cout << "Test 1 (Valid Max-Heap): " << (isBinaryTreeHeap(t1) ? "true" : "false") << "\n";
    freeTree(t1);

    // Test 2: Fails completeness (right child exists without left child)
    TreeNode* t2 = new TreeNode(50);
    t2->left = new TreeNode(30);
    t2->right = new TreeNode(20);
    t2->left->right = new TreeNode(10);
    cout << "Test 2 (Incomplete Tree): " << (isBinaryTreeHeap(t2) ? "true" : "false") << "\n";
    freeTree(t2);

    // Test 3: Fails heap order (child 40 > parent 30)
    TreeNode* t3 = new TreeNode(50);
    t3->left = new TreeNode(30);
    t3->right = new TreeNode(20);
    t3->left->left = new TreeNode(40);
    cout << "Test 3 (Heap Order Violated): " << (isBinaryTreeHeap(t3) ? "true" : "false") << "\n";
    freeTree(t3);

    // Test 4: Empty tree and single-node tree
    TreeNode* t4 = nullptr;
    TreeNode* t5 = new TreeNode(42);
    cout << "Test 4 (Empty Tree): " << (isBinaryTreeHeap(t4) ? "true" : "false") << "\n";
    cout << "Test 5 (Single Node): " << (isBinaryTreeHeap(t5) ? "true" : "false") << "\n";
    freeTree(t5);

    // Test 6: Tree materialized directly from our MaxHeap class
    MaxHeap mh;
    vector<int> vals = {10, 20, 5, 40, 30, 15};
    for (int i = 0; i < (int)vals.size(); i++) {
        mh.insert(vals[i]);
    }
    TreeNode* t6 = buildTreeFromMaxHeap(mh, 1);
    cout << "Test 6 (Tree Built From MaxHeap): " << (isBinaryTreeHeap(t6) ? "true" : "false") << "\n";
    freeTree(t6);

    return 0;
}
```

---

## LeetCode Pattern Takeaways

The `index >= totalNodes` completeness check is one of the cleanest tree tricks to keep in your toolkit:
- **LeetCode 958 — Check Completeness of a Binary Tree:** Uses the exact `index >= totalNodes` DFS check (or level-order null-gap check).
- **LeetCode 222 — Count Complete Tree Nodes:** Exploits completeness to count nodes faster than $O(n)$ via subtree heights.
- **LeetCode 98 — Validate Binary Search Tree:** Contrast how BST validation passes `(min, max)` bounds downward, whereas Heap validation checks local `parent >= child` plus structural completeness.
