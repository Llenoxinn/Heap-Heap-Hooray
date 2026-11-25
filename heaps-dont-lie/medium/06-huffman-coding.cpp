#include <iostream>
#include <vector>
#include <string>
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
    vector<HuffmanNode*> items;
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
        items.push_back(nullptr);
        size = 0;
    }

    void insert(HuffmanNode* node) {
        items.push_back(node);
        size++;
        heapifyUp(size);
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
        items.pop_back();
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
};

struct CodeEntry {
    char ch;
    int freq;
    string code;
};

void collectCodes(HuffmanNode* root, string path, vector<CodeEntry>& out) {
    if (root == nullptr) {
        return;
    }
    if (root->left == nullptr && root->right == nullptr) {
        CodeEntry entry;
        entry.ch = root->ch;
        entry.freq = root->freq;
        entry.code = path.empty() ? "0" : path;
        out.push_back(entry);
        return;
    }
    collectCodes(root->left, path + "0", out);
    collectCodes(root->right, path + "1", out);
}

void freeTree(HuffmanNode* root) {
    if (root == nullptr) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

vector<CodeEntry> buildHuffmanCodes(vector<char>& chars, vector<int>& freqs) {
    vector<CodeEntry> codes;
    if (chars.empty() || chars.size() != freqs.size()) {
        return codes;
    }

    MinHeap minHeap;
    for (int i = 0; i < (int)chars.size(); i++) {
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
    collectCodes(root, "", codes);
    freeTree(root);
    return codes;
}

void runTest(string label, vector<char> chars, vector<int> freqs) {
    cout << label << "\n";
    vector<CodeEntry> codes = buildHuffmanCodes(chars, freqs);
    int totalBits = 0;
    for (int i = 0; i < (int)codes.size(); i++) {
        cout << "  '" << codes[i].ch << "' (freq " << codes[i].freq << ") -> " << codes[i].code << "\n";
        totalBits += codes[i].freq * (int)codes[i].code.size();
    }
    cout << "  Total encoded bits: " << totalBits << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Example)",
            {'a', 'b', 'c', 'd', 'e', 'f'},
            {5, 9, 12, 13, 16, 45});

    runTest("Test 2 (Equal Frequencies)",
            {'w', 'x', 'y', 'z'},
            {10, 10, 10, 10});

    runTest("Test 3 (Single Character)",
            {'a'},
            {42});

    return 0;
}
