# 15. Convert BST to Min Heap

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Convert BST to Min Heap](https://www.geeksforgeeks.org/dsa/convert-bst-min-heap/)

---

## Problem Statement

Given a **Binary Search Tree (BST)** that is already a **complete binary tree**, convert it into a **Min-Heap** in-place (preserving the tree's node structure) such that **all values in the left subtree of any node are smaller than all values in the right subtree** of that node.

---

## Pattern Recognition & Intuition

### Why think "Heap + Tree Traversal" here?
Let's examine the two target conditions on every node `u`:
1. **Min-Heap condition:** `u->val < u->left->val` and `u->val < u->right->val` (or `<=` if duplicates).
2. **Subtree ordering condition:** every value in `u->left` is smaller than every value in `u->right`.

Put those two rules together for any node `u`:
`u->val  <  (all values in u->left)  <  (all values in u->right)`

Look closely at that order: **`Node -> Left -> Right`**! That is literally **Preorder Traversal**!

And how do we get all the values in ascending sorted order from a BST? **Inorder Traversal** (`Left -> Node -> Right`) takes $O(N)$ without sorting!

### Two-Pass Algorithm
1. **Pass 1 (Inorder Traversal):** Collect all BST values into an array `vals` -> guaranteed sorted ascending!
2. **Pass 2 (Preorder Traversal):** Walk the tree in `Root -> Left -> Right` order and overwrite `node->val = vals[idx++]`.
   - Because `vals` is ascending and we assign `Root` before `Left` and `Left` completely before `Right`:
     - `Root < Left` and `Root < Right` (Min-Heap holds!).
     - Every value in `Left` is assigned before any value in `Right` (`Left < Right` holds!).
     - Tree shape stays untouched (completeness is preserved!).

### Key Invariants & Edge Cases
- **Completeness Precondition:** The input BST must already be a complete binary tree (all levels filled except possibly the last, left-aligned) for the resulting tree to be a valid complete Min-Heap.
- **What about BST to Max-Heap?** Use **Reverse Preorder** (`Root -> Right -> Left`) with ascending inorder, OR **Preorder** (`Root -> Left -> Right`) with **reverse inorder** (`Right -> Root -> Left`)!
- **Edge Cases:** Empty tree (`nullptr`), single-node tree, incomplete last level (Test 2 has 5 nodes, so last level only fills left subtree).

---

## Step-by-Step Trace

Using **Test 1** from `main()`:

Initial BST (level order `[4, 2, 6, 1, 3, 5, 7]`):
```text
        4
      /   \
     2     6
    / \   / \
   1   3 5   7
```

1. **Pass 1 — Inorder Traversal (`Left -> Root -> Right`):**
   - `vals = [1, 2, 3, 4, 5, 6, 7]`

2. **Pass 2 — Preorder Assignment (`Root -> Left -> Right`):**
   - Visit root `4` -> set to `vals[0] = 1`
   - Visit `2` -> set to `vals[1] = 2`
   - Visit `1` -> set to `vals[2] = 3`
   - Visit `3` -> set to `vals[3] = 4`
   - Visit `6` -> set to `vals[4] = 5`
   - Visit `5` -> set to `vals[5] = 6`
   - Visit `7` -> set to `vals[6] = 7`

Resulting Min-Heap:
```text
        1
      /   \
     2     5
    / \   / \
   3   4 6   7
```

- **Level order:** `[1, 2, 5, 3, 4, 6, 7]`
- **Preorder:** `1 2 3 4 5 6 7`
- Notice: every parent `<` children (`1 < 2, 5`; `2 < 3, 4`; `5 < 6, 7`) AND all left-subtree values (`{2, 3, 4}`) `<` all right-subtree values (`{5, 6, 7}`)!

---

## Time & Space Complexity

- **Time Complexity:** $O(N)$
  - One inorder traversal ($O(N)$) + one preorder traversal ($O(N)$).
- **Space Complexity:** $O(N)$ for the `vals` array plus $O(H)$ recursion stack ($H = \lfloor \log_2 N \rfloor$ since the tree is complete).

---

## C++ Implementation

Source file: [15-bst-to-min-heap.cpp](15-bst-to-min-heap.cpp)

```cpp
#include <iostream>
#include <queue>
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

void inorderCollect(TreeNode* root, vector<int>& vals) {
    if (root == nullptr) return;
    inorderCollect(root->left, vals);
    vals.push_back(root->val);
    inorderCollect(root->right, vals);
}

void preorderAssign(TreeNode* root, vector<int>& vals, int& idx) {
    if (root == nullptr) return;
    root->val = vals[idx];
    idx++;
    preorderAssign(root->left, vals, idx);
    preorderAssign(root->right, vals, idx);
}

void convertBSTToMinHeap(TreeNode* root) {
    vector<int> vals;
    inorderCollect(root, vals);
    int idx = 0;
    preorderAssign(root, vals, idx);
}

vector<int> levelOrder(TreeNode* root) {
    vector<int> res;
    if (root == nullptr) return res;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* curr = q.front();
        q.pop();
        res.push_back(curr->val);
        if (curr->left != nullptr) q.push(curr->left);
        if (curr->right != nullptr) q.push(curr->right);
    }
    return res;
}

void preorderPrint(TreeNode* root) {
    if (root == nullptr) return;
    cout << root->val << " ";
    preorderPrint(root->left);
    preorderPrint(root->right);
}

void freeTree(TreeNode* root) {
    if (root == nullptr) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
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
    TreeNode* root1 = new TreeNode(4);
    root1->left = new TreeNode(2);
    root1->right = new TreeNode(6);
    root1->left->left = new TreeNode(1);
    root1->left->right = new TreeNode(3);
    root1->right->left = new TreeNode(5);
    root1->right->right = new TreeNode(7);

    vector<int> before1 = levelOrder(root1);
    cout << "Test 1 BST level order:      ";
    printVector(before1);
    cout << "\n";

    convertBSTToMinHeap(root1);
    vector<int> after1 = levelOrder(root1);
    cout << "Test 1 Min-Heap level order: ";
    printVector(after1);
    cout << "\nTest 1 Min-Heap preorder:    ";
    preorderPrint(root1);
    cout << "\n\n";
    freeTree(root1);

    TreeNode* root2 = new TreeNode(8);
    root2->left = new TreeNode(4);
    root2->right = new TreeNode(10);
    root2->left->left = new TreeNode(2);
    root2->left->right = new TreeNode(6);

    vector<int> before2 = levelOrder(root2);
    cout << "Test 2 BST level order:      ";
    printVector(before2);
    cout << "\n";

    convertBSTToMinHeap(root2);
    vector<int> after2 = levelOrder(root2);
    cout << "Test 2 Min-Heap level order: ";
    printVector(after2);
    cout << "\nTest 2 Min-Heap preorder:    ";
    preorderPrint(root2);
    cout << "\n";
    freeTree(root2);

    return 0;
}
```

---

## LeetCode Pattern Takeaways

Using **BST Inorder -> Sorted Sequence -> Structural Reassignment** is a recurring tree-transformation pattern:
- **LeetCode 538 / 1038** — *Convert BST to Greater Tree* (Reverse Inorder accumulation)
- **LeetCode 897** — *Increasing Order Search Tree* (Inorder relinking)
- **LeetCode 1382** — *Balance a Binary Search Tree* (Inorder collect + midpoint rebuild)
- **LeetCode 99** — *Recover Binary Search Tree* (Inorder order-violation detection)
