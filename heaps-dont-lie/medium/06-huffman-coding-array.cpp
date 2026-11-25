#include <iostream>
#define MAX 100
#define MAX_CODE_LEN 100
using namespace std;

struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, int f, HuffmanNode* l, HuffmanNode* r) {
        ch = c;
        freq = f;
        left = l;
        right = r;
    }
};

class MinHeap {
private:
    HuffmanNode* items[MAX];
    int size;

    void heapifyUp(int curr) {
        HuffmanNode* x = items[curr];
        while (curr / 2 > 0 && x->freq < items[curr / 2]->freq) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        HuffmanNode* x = items[curr];
        while (curr * 2 <= size) {
            int s = curr * 2;
            if (s + 1 <= size && items[s]->freq > items[s + 1]->freq) {
                s++;
            }
            if (items[s]->freq < x->freq) {
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
        items[0] = nullptr;
        size = 0;
    }

    bool insert(HuffmanNode* node) {
        if (size < MAX - 1) {
            items[++size] = node;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    HuffmanNode* getMin() {
        if (size == 0) {
            return nullptr;
        }
        return items[1];
    }

    HuffmanNode* extractMin() {
        if (size == 0) {
            return nullptr;
        }
        HuffmanNode* minNode = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return minNode;
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
            cout << items[i]->ch << ":" << items[i]->freq << " ";
        }
        cout << endl;
    }
};

struct CodeEntry {
    char ch;
    int freq;
    char code[MAX_CODE_LEN];
    int codeLen;
};

void collectCodes(HuffmanNode* root, char path[], int depth, CodeEntry out[], int& outCount) {
    if (root == nullptr) {
        return;
    }
    if (root->left == nullptr && root->right == nullptr) {
        CodeEntry entry;
        entry.ch = root->ch;
        entry.freq = root->freq;
        if (depth == 0) {
            entry.code[0] = '0';
            entry.code[1] = '\0';
            entry.codeLen = 1;
        } else {
            for (int i = 0; i < depth; i++) {
                entry.code[i] = path[i];
            }
            entry.code[depth] = '\0';
            entry.codeLen = depth;
        }
        out[outCount++] = entry;
        return;
    }
    path[depth] = '0';
    collectCodes(root->left, path, depth + 1, out, outCount);
    path[depth] = '1';
    collectCodes(root->right, path, depth + 1, out, outCount);
}

void freeTree(HuffmanNode* root) {
    if (root == nullptr) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int buildHuffmanCodes(char chars[], int freqs[], int n, CodeEntry out[]) {
    if (n <= 0) {
        return 0;
    }

    MinHeap minHeap;
    for (int i = 0; i < n; i++) {
        HuffmanNode* leaf = new HuffmanNode(chars[i], freqs[i], nullptr, nullptr);
        minHeap.insert(leaf);
    }

    while (minHeap.getSize() > 1) {
        HuffmanNode* left = minHeap.extractMin();
        HuffmanNode* right = minHeap.extractMin();
        HuffmanNode* merged = new HuffmanNode('$', left->freq + right->freq, left, right);
        minHeap.insert(merged);
    }

    HuffmanNode* root = minHeap.extractMin();
    char path[MAX_CODE_LEN];
    int outCount = 0;
    collectCodes(root, path, 0, out, outCount);
    freeTree(root);
    return outCount;
}

void runTest(char chars[], int freqs[], int n) {
    CodeEntry codes[MAX];
    int count = buildHuffmanCodes(chars, freqs, n, codes);
    int totalBits = 0;
    for (int i = 0; i < count; i++) {
        cout << "  '" << codes[i].ch << "' (freq " << codes[i].freq << ") -> " << codes[i].code << "\n";
        totalBits += codes[i].freq * codes[i].codeLen;
    }
    cout << "  Total encoded bits: " << totalBits << "\n\n";
}

int main() {
    cout << "Test 1 (Primary Example)\n";
    char c1[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int f1[] = {5, 9, 12, 13, 16, 45};
    runTest(c1, f1, 6);

    cout << "Test 2 (Equal Frequencies)\n";
    char c2[] = {'w', 'x', 'y', 'z'};
    int f2[] = {10, 10, 10, 10};
    runTest(c2, f2, 4);

    cout << "Test 3 (Single Character)\n";
    char c3[] = {'a'};
    int f3[] = {42};
    runTest(c3, f3, 1);

    return 0;
}
