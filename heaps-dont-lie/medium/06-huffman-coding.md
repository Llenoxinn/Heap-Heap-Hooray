# Huffman Coding

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Huffman Coding (Greedy Algo-3)](https://www.geeksforgeeks.org/dsa/huffman-coding-greedy-algo-3/)

---

## Problem Statement

Given `n` characters and their corresponding frequencies, construct a **Huffman Tree** and assign a **prefix-free binary code** to each character such that the total encoded bit length $\sum f_i \cdot \text{len}(\text{code}_i)$ is minimized.

**Example:**
- Input: `chars = ['a', 'b', 'c', 'd', 'e', 'f']`, `freqs = [5, 9, 12, 13, 16, 45]`
- Output:
  - `'f'` → `0`
  - `'c'` → `100`
  - `'d'` → `101`
  - `'a'` → `1100`
  - `'b'` → `1101`
  - `'e'` → `111`
  - Total encoded bits = `224`

---

## Pattern Recognition & Intuition

- **Why think "Heap"?**
  Huffman's greedy algorithm repeatedly merges the **two lowest-frequency** subtrees into a new internal node whose frequency is their sum, and pushes that merged node back into the active pool. You need a data structure that supports dynamic insertion *and* fast extraction of the minimum element after each merge — that's the exact job of a **Min-Heap**!
- **Min-Heap vs Max-Heap Decision Rule:**
  We want the two **smallest** frequencies first (so rarer characters end up deeper in the tree with longer codes, while frequent characters stay near the root with short codes). Therefore we use a **Min-Heap** ordered by `node->freq`.
- **Why it beats naive approaches:**
  - Re-sorting or linear scanning `n` nodes on each of the `n - 1` merges takes $O(n^2)$ time.
  - Using a Min-Heap drops each of the `n - 1` merges (2 `extractMin` + 1 `insert`) to $O(\log n)$, giving $O(n \log n)$ total.
- **Key Invariant:**
  At every iteration of the merge loop, the Min-Heap holds all root nodes of the partially built forest, ordered by total subtree frequency. The two extracted nodes are always the two lightest trees remaining, guaranteeing an optimal prefix-free tree.
- **Edge Cases:**
  - **Single character:** The loop `size > 1` runs 0 times; if we naively used `path`, the single leaf would get `""` (empty code!). We explicitly assign `"0"` when `path.empty()` at a leaf.
  - **Equal frequencies:** Any valid tie-breaking produces an optimal tree with the same minimum total bit cost.
  - **Empty input / mismatched lengths:** Guarded upfront, returning empty codes.
  - **Memory cleanup:** Every `HuffmanNode` allocated with `new` is freed recursively via `freeTree` after extracting codes.

---

## Step-by-Step Trace

Tracing **Test 1** from `main()`: `chars = ['a', 'b', 'c', 'd', 'e', 'f']`, `freqs = [5, 9, 12, 13, 16, 45]`.

1. **Initial Min-Heap (6 leaf nodes):**
   `[a:5, b:9, c:12, d:13, e:16, f:45]`
2. **Merge 1:** Pop `a:5` and `b:9` → create `$:14(a:5, b:9)`, insert into heap.
   - Heap frequencies: `[12, 13, 14, 16, 45]` (`c:12`, `d:13`, `$:14`, `e:16`, `f:45`)
3. **Merge 2:** Pop `c:12` and `d:13` → create `$:25(c:12, d:13)`, insert into heap.
   - Heap frequencies: `[14, 16, 25, 45]` (`$:14`, `e:16`, `$:25`, `f:45`)
4. **Merge 3:** Pop `$:14` and `e:16` → create `$:30($:14, e:16)`, insert into heap.
   - Heap frequencies: `[25, 30, 45]` (`$:25`, `$:30`, `f:45`)
5. **Merge 4:** Pop `$:25` and `$:30` → create `$:55($:25, $:30)`, insert into heap.
   - Heap frequencies: `[45, 55]` (`f:45`, `$:55`)
6. **Merge 5:** Pop `f:45` and `$:55` → create `$:100(f:45, $:55)`, insert into heap.
   - Heap size is `1` → stop! Root is `$:100`.
7. **Traverse Root-to-Leaf (`left = '0'`, `right = '1'`):**
   - `f` (left of `100`): `0`
   - `c` (`100` → `1` → `55` → `0` → `25` → `0`): `100`
   - `d` (`100` → `1` → `55` → `0` → `25` → `1`): `101`
   - `a` (`100` → `1` → `55` → `1` → `30` → `0` → `14` → `0`): `1100`
   - `b` (`100` → `1` → `55` → `1` → `30` → `0` → `14` → `1`): `1101`
   - `e` (`100` → `1` → `55` → `1` → `30` → `1`): `111`

Total encoded bits = `45*1 + 12*3 + 13*3 + 5*4 + 9*4 + 16*3 = 45 + 36 + 39 + 20 + 36 + 48 = 224`.

---

## Time & Space Complexity

- **Time Complexity:** $O(n \log n)$ where `n` is the number of distinct characters.
  - Inserting `n` leaves takes $O(n \log n)$ (or $O(n)$ with Floyd's build-heap).
  - Running `n - 1` merges, each doing 2 `extractMin` and 1 `insert`, takes $O(n \log n)$.
  - Tree traversal visits `2n - 1` nodes in $O(n)$ (building output strings of total length bounded by $O(n^2)$ in the worst-case Fibonacci skew, or $O(n \log n)$ typical).
- **Space Complexity:** $O(n)$
  - The Huffman tree has `2n - 1` nodes, and the Min-Heap holds at most `n` pointers.

---

## C++ Implementation

See [06-huffman-coding.cpp](06-huffman-coding.cpp).

```cpp
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
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts Huffman Coding to raw fixed-size arrays matching `Heap-Implementation-OPTIMIZED.cpp`, removing `std::vector` and `std::string`:
- `MinHeap` stores `HuffmanNode* items[MAX]` (`#define MAX 100`) with `int size`, implementing 1-indexed `heapifyUp`, `minHeapify`, `insert`, `getMin`, `extractMin`, `isEmpty`, `getSize`, and `display`.
- `CodeEntry` replaces `std::string code` with a null-terminated `char code[MAX_CODE_LEN]` buffer and `int codeLen`.
- `collectCodes` fills a local `char path[MAX_CODE_LEN]` buffer across DFS depth steps and writes `CodeEntry` records directly into `CodeEntry out[]`, while `buildHuffmanCodes(char chars[], int freqs[], int n, CodeEntry out[])` takes raw arrays and returns the number of generated codes.

See [06-huffman-coding-array.cpp](06-huffman-coding-array.cpp).

```cpp
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
```

---

## LeetCode Pattern Takeaways

Huffman Coding is the canonical **Greedy Merge with Min-Heap** pattern — whenever you must repeatedly combine the two smallest items and push the combined item back:
- **LeetCode 1167 — Minimum Cost to Connect Sticks:** Exact numeric twin of Huffman Coding (without reconstructing the tree).
- **LeetCode 1046 — Last Stone Weight:** Opposite polarity (Max-Heap merging the two heaviest stones).
- **LeetCode 1199 — Minimum Time to Build Blocks:** Optimal split/merge tree solved bottom-up with a Min-Heap.
