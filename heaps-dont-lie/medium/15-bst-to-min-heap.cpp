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
