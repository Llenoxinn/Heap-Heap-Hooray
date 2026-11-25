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

    int keyAt(int idx) {
        return items[idx];
    }

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << items[i] << " ";
        }
        cout << endl;
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

    TreeNode* t2 = new TreeNode(50);
    t2->left = new TreeNode(30);
    t2->right = new TreeNode(20);
    t2->left->right = new TreeNode(10);
    cout << "Test 2 (Incomplete Tree): " << (isBinaryTreeHeap(t2) ? "true" : "false") << "\n";
    freeTree(t2);

    TreeNode* t3 = new TreeNode(50);
    t3->left = new TreeNode(30);
    t3->right = new TreeNode(20);
    t3->left->left = new TreeNode(40);
    cout << "Test 3 (Heap Order Violated): " << (isBinaryTreeHeap(t3) ? "true" : "false") << "\n";
    freeTree(t3);

    TreeNode* t4 = nullptr;
    TreeNode* t5 = new TreeNode(42);
    cout << "Test 4 (Empty Tree): " << (isBinaryTreeHeap(t4) ? "true" : "false") << "\n";
    cout << "Test 5 (Single Node): " << (isBinaryTreeHeap(t5) ? "true" : "false") << "\n";
    freeTree(t5);

    MaxHeap mh;
    int vals[] = {10, 20, 5, 40, 30, 15};
    for (int i = 0; i < 6; i++) {
        mh.insert(vals[i]);
    }
    TreeNode* t6 = buildTreeFromMaxHeap(mh, 1);
    cout << "Test 6 (Tree Built From MaxHeap): " << (isBinaryTreeHeap(t6) ? "true" : "false") << "\n";
    freeTree(t6);

    return 0;
}
