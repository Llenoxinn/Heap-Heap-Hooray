#include <iostream>
#define MAX 100
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

void inorderCollect(TreeNode* root, int vals[], int& count) {
    if (root == nullptr) return;
    inorderCollect(root->left, vals, count);
    if (count < MAX) {
        vals[count++] = root->val;
    }
    inorderCollect(root->right, vals, count);
}

void preorderAssign(TreeNode* root, int vals[], int count, int& idx) {
    if (root == nullptr || idx >= count) return;
    root->val = vals[idx];
    idx++;
    preorderAssign(root->left, vals, count, idx);
    preorderAssign(root->right, vals, count, idx);
}

void convertBSTToMinHeap(TreeNode* root) {
    int vals[MAX];
    int count = 0;
    inorderCollect(root, vals, count);
    int idx = 0;
    preorderAssign(root, vals, count, idx);
}

int levelOrder(TreeNode* root, int res[]) {
    if (root == nullptr) return 0;

    TreeNode* fifo[MAX];
    int front = 0;
    int rear = 0;
    fifo[rear++] = root;

    int count = 0;
    while (front < rear && count < MAX) {
        TreeNode* curr = fifo[front++];
        res[count++] = curr->val;
        if (curr->left != nullptr && rear < MAX) {
            fifo[rear++] = curr->left;
        }
        if (curr->right != nullptr && rear < MAX) {
            fifo[rear++] = curr->right;
        }
    }
    return count;
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

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
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

    int before1[MAX];
    int beforeLen1 = levelOrder(root1, before1);
    cout << "Test 1 BST level order:      ";
    printArray(before1, beforeLen1);
    cout << "\n";

    convertBSTToMinHeap(root1);
    int after1[MAX];
    int afterLen1 = levelOrder(root1, after1);
    cout << "Test 1 Min-Heap level order: ";
    printArray(after1, afterLen1);
    cout << "\nTest 1 Min-Heap preorder:    ";
    preorderPrint(root1);
    cout << "\n\n";
    freeTree(root1);

    TreeNode* root2 = new TreeNode(8);
    root2->left = new TreeNode(4);
    root2->right = new TreeNode(10);
    root2->left->left = new TreeNode(2);
    root2->left->right = new TreeNode(6);

    int before2[MAX];
    int beforeLen2 = levelOrder(root2, before2);
    cout << "Test 2 BST level order:      ";
    printArray(before2, beforeLen2);
    cout << "\n";

    convertBSTToMinHeap(root2);
    int after2[MAX];
    int afterLen2 = levelOrder(root2, after2);
    cout << "Test 2 Min-Heap level order: ";
    printArray(after2, afterLen2);
    cout << "\nTest 2 Min-Heap preorder:    ";
    preorderPrint(root2);
    cout << "\n";
    freeTree(root2);

    return 0;
}
