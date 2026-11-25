# 13. Data Structure Supporting O(1) Min/Max and O(log n) Insert/DeleteMin/DeleteMax

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — A Data Structure Question (Min-Max Heap + DLL)](https://www.geeksforgeeks.org/dsa/a-data-structure-question/)

---

## 1. Problem Statement

Design a data structure that efficiently supports all five operations on a dynamic multiset of integers:
- `findMin()` in `O(1)`
- `findMax()` in `O(1)`
- `insert(x)` in `O(log n)`
- `deleteMin()` in `O(log n)`
- `deleteMax()` in `O(log n)`

---

## 2. Pattern Recognition & Intuition

### Why Think "Dual Heaps + Doubly Linked List with Cross-Referenced Indices"?
A single Min-Heap gives `O(1)` `findMin()`, and a single Max-Heap gives `O(1)` `findMax()`. Why can't you just use two independent heaps storing raw `int` values?

- **The trap with two unlinked heaps:** When you run `deleteMin()`, you pop the minimum from `MinHeap` in `O(log n)`, **but where is that same element sitting inside `MaxHeap`?** Searching `MaxHeap` linearly would take `O(n)`!
- **Why lazy-deletion hash maps aren't as clean as cross-referenced indices:** Lazy deletion leaves dead entries accumulating in memory if you alternate `deleteMin` / `deleteMax`, whereas **cross-referenced indices** keep both heaps strictly synchronized at size `n` with zero stale nodes.

### Architecture & Key Invariants
1. **Doubly Linked List (`DLLNode`):** Each inserted element lives in one `DLLNode` containing:
   - `data` (the integer value)
   - `minHeapIdx` (its current 1-based index inside `MinHeap`)
   - `maxHeapIdx` (its current 1-based index inside `MaxHeap`)
   - `prev`, `next` pointers
2. **MinHeap & MaxHeap store `DLLNode*`:** Whenever `swapNodes(i, j)` or `removeAt(idx)` moves a `DLLNode*` to index `k` in `MinHeap`, it immediately updates `node->minHeapIdx = k` (and symmetrically `node->maxHeapIdx = k` in `MaxHeap`).
3. **`O(log n)` arbitrary removal (`removeAt(idx)`):**
   - Replace `items[idx]` with the last element `items[size]`, update its heap index to `idx`, and shrink the heap by 1.
   - Because the tree was already a valid heap before replacement, the moved element can violate the heap property in **at most one direction**:
     - If `items[idx]` is better than its parent (`idx > 1`), call `heapifyUp(idx)`.
     - Otherwise, call `minHeapify(idx)` (or `maxHeapify(idx)`).
4. **Synchronized deletion:**
   - `deleteMin()`: `node = minHeap.extractMin()` -> `maxHeap.removeAt(node->maxHeapIdx)` -> `dll.removeNode(node)` in `O(1)`!
   - `deleteMax()`: `node = maxHeap.extractMax()` -> `minHeap.removeAt(node->minHeapIdx)` -> `dll.removeNode(node)` in `O(1)`!

### Edge Cases
- **Duplicate values:** Because each insertion creates a distinct `DLLNode*` with its own `minHeapIdx` and `maxHeapIdx`, duplicate values (like inserting `5` twice) never collide or delete the wrong copy!
- **Single-element deletion:** Removing index `1` when `size == 1` simply pops the vector without accessing out-of-bounds indices.
- **Empty structure:** Guard `findMin()`, `findMax()`, `deleteMin()`, and `deleteMax()` when `isEmpty()` is true.

---

## 3. Step-by-Step Trace (Test Case 1 from `main()`)

| Step | Operation | DLL Contents | `MinHeap` (`data @ minIdx`) | `MaxHeap` (`data @ maxIdx`) | `findMin()` | `findMax()` |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1..5 | `insert(10, 20, 30, 40, 50)` | `[10, 20, 30, 40, 50]` | `[10@1, 20@2, 30@3, 40@4, 50@5]` | `[50@1, 40@2, 20@3, 10@4, 30@5]` | `10` | `50` |
| 6 | `deleteMin()` -> `10` | `[20, 30, 40, 50]` | Extract `10` -> `[20@1, 40@2, 30@3, 50@4]` | `removeAt(10->maxHeapIdx=4)` -> `[50@1, 40@2, 20@3, 30@4]` | `20` | `50` |
| 7 | `deleteMax()` -> `50` | `[20, 30, 40]` | `removeAt(50->minHeapIdx=4)` -> `[20@1, 40@2, 30@3]` | Extract `50` -> `[40@1, 30@2, 20@3]` | `20` | `40` |
| 8 | `insert(5)` | `[20, 30, 40, 5]` | `[5@1, 20@2, 30@3, 40@4]` | `[40@1, 30@2, 20@3, 5@4]` | `5` | `40` |
| 9 | `insert(35)` | `[20, 30, 40, 5, 35]` | `[5@1, 20@2, 30@3, 40@4, 35@5]` | `[40@1, 35@2, 20@3, 5@4, 30@5]` | `5` | `40` |
| 10 | `insert(5)` (dup) | `[20, 30, 40, 5, 35, 5]` | `[5@1, 20@2, 5@3, 40@4, 35@5, 30@6]` | `[40@1, 35@2, 20@3, 5@4, 30@5, 5@6]` | `5` | `40` |
| 11 | `deleteMax()` -> `40` | `[20, 30, 5, 35, 5]` | `removeAt(4)` -> `[5@1, 20@2, 5@3, 30@4, 35@5]` | Extract `40` -> `[35@1, 30@2, 20@3, 5@4, 5@5]` | `5` | `35` |
| 12 | `deleteMin()` -> `5` | `[20, 30, 35, 5]` | Extract `5` -> `[5@1, 20@2, 35@3, 30@4]` | `removeAt(4)` -> `[35@1, 30@2, 20@3, 5@4]` | `5` | `35` |
| 13..16 | Alternate `deleteMax`/`deleteMin` | Drains `35, 5, 30, 20` -> `[]` | Empty | Empty | — | — |

---

## 4. Time & Space Complexity

- **`findMin()` / `findMax()`:** `O(1)` — reads `items[1]->data` directly.
- **`insert(x)`:** `O(log n)` — `O(1)` DLL append + `O(log n)` insertion into `MinHeap` and `MaxHeap`.
- **`deleteMin()` / `deleteMax()`:** `O(log n)` — `O(log n)` extract from one heap + `O(log n)` `removeAt` in the partner heap + `O(1)` DLL node unlink.
- **Space Complexity:** `O(n)` — `n` DLL nodes plus two `n`-element pointer arrays.

---

## 5. C++ Implementation

Source file: [13-data-structure-with-min-and-max.cpp](13-data-structure-with-min-and-max.cpp)

```cpp
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

struct DLLNode {
    int data;
    int minHeapIdx;
    int maxHeapIdx;
    DLLNode* prev;
    DLLNode* next;

    DLLNode(int val) {
        data = val;
        minHeapIdx = -1;
        maxHeapIdx = -1;
        prev = nullptr;
        next = nullptr;
    }
};

class DoublyLinkedList {
private:
    DLLNode* head;
    DLLNode* tail;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    DLLNode* insertBack(int val);
    void removeNode(DLLNode* node);
    void printList();
};

DoublyLinkedList::DoublyLinkedList() {
    head = nullptr;
    tail = nullptr;
}

DoublyLinkedList::~DoublyLinkedList() {
    DLLNode* curr = head;
    while (curr != nullptr) {
        DLLNode* nxt = curr->next;
        delete curr;
        curr = nxt;
    }
}

DLLNode* DoublyLinkedList::insertBack(int val) {
    DLLNode* node = new DLLNode(val);
    if (tail == nullptr) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    return node;
}

void DoublyLinkedList::removeNode(DLLNode* node) {
    if (node == nullptr) {
        return;
    }
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }
    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }
    delete node;
}

void DoublyLinkedList::printList() {
    DLLNode* curr = head;
    cout << "[";
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next != nullptr) {
            cout << ", ";
        }
        curr = curr->next;
    }
    cout << "]";
}

class MinHeap {
private:
    vector<DLLNode*> items;
    int size;

    void swapNodes(int i, int j);
    void heapifyUp(int curr);
    void minHeapify(int curr);

public:
    MinHeap();
    void insert(DLLNode* node);
    DLLNode* getMin();
    DLLNode* extractMin();
    void removeAt(int idx);
    bool isEmpty();
    int getSize();
};

MinHeap::MinHeap() {
    items.push_back(nullptr);
    size = 0;
}

void MinHeap::swapNodes(int i, int j) {
    DLLNode* temp = items[i];
    items[i] = items[j];
    items[j] = temp;
    items[i]->minHeapIdx = i;
    items[j]->minHeapIdx = j;
}

void MinHeap::heapifyUp(int curr) {
    while (curr / 2 > 0 && items[curr]->data < items[curr / 2]->data) {
        swapNodes(curr, curr / 2);
        curr = curr / 2;
    }
}

void MinHeap::minHeapify(int curr) {
    while (curr * 2 <= size) {
        int s = curr * 2;
        if (s + 1 <= size && items[s]->data > items[s + 1]->data) {
            s++;
        }
        if (items[s]->data < items[curr]->data) {
            swapNodes(curr, s);
            curr = s;
        } else {
            break;
        }
    }
}

void MinHeap::insert(DLLNode* node) {
    items.push_back(node);
    size++;
    node->minHeapIdx = size;
    heapifyUp(size);
}

DLLNode* MinHeap::getMin() {
    if (size == 0) {
        return nullptr;
    }
    return items[1];
}

DLLNode* MinHeap::extractMin() {
    if (size == 0) {
        return nullptr;
    }
    DLLNode* minNode = items[1];
    minNode->minHeapIdx = -1;
    if (size == 1) {
        items.pop_back();
        size = 0;
        return minNode;
    }
    items[1] = items[size];
    items[1]->minHeapIdx = 1;
    items.pop_back();
    size--;
    minHeapify(1);
    return minNode;
}

void MinHeap::removeAt(int idx) {
    if (idx < 1 || idx > size) {
        return;
    }
    items[idx]->minHeapIdx = -1;
    if (idx == size) {
        items.pop_back();
        size--;
        return;
    }
    items[idx] = items[size];
    items[idx]->minHeapIdx = idx;
    items.pop_back();
    size--;

    if (idx > 1 && items[idx]->data < items[idx / 2]->data) {
        heapifyUp(idx);
    } else {
        minHeapify(idx);
    }
}

bool MinHeap::isEmpty() {
    return size == 0;
}

int MinHeap::getSize() {
    return size;
}

class MaxHeap {
private:
    vector<DLLNode*> items;
    int size;

    void swapNodes(int i, int j);
    void heapifyUp(int curr);
    void maxHeapify(int curr);

public:
    MaxHeap();
    void insert(DLLNode* node);
    DLLNode* getMax();
    DLLNode* extractMax();
    void removeAt(int idx);
    bool isEmpty();
    int getSize();
};

MaxHeap::MaxHeap() {
    items.push_back(nullptr);
    size = 0;
}

void MaxHeap::swapNodes(int i, int j) {
    DLLNode* temp = items[i];
    items[i] = items[j];
    items[j] = temp;
    items[i]->maxHeapIdx = i;
    items[j]->maxHeapIdx = j;
}

void MaxHeap::heapifyUp(int curr) {
    while (curr / 2 > 0 && items[curr]->data > items[curr / 2]->data) {
        swapNodes(curr, curr / 2);
        curr = curr / 2;
    }
}

void MaxHeap::maxHeapify(int curr) {
    while (curr * 2 <= size) {
        int s = curr * 2;
        if (s + 1 <= size && items[s]->data < items[s + 1]->data) {
            s++;
        }
        if (items[s]->data > items[curr]->data) {
            swapNodes(curr, s);
            curr = s;
        } else {
            break;
        }
    }
}

void MaxHeap::insert(DLLNode* node) {
    items.push_back(node);
    size++;
    node->maxHeapIdx = size;
    heapifyUp(size);
}

DLLNode* MaxHeap::getMax() {
    if (size == 0) {
        return nullptr;
    }
    return items[1];
}

DLLNode* MaxHeap::extractMax() {
    if (size == 0) {
        return nullptr;
    }
    DLLNode* maxNode = items[1];
    maxNode->maxHeapIdx = -1;
    if (size == 1) {
        items.pop_back();
        size = 0;
        return maxNode;
    }
    items[1] = items[size];
    items[1]->maxHeapIdx = 1;
    items.pop_back();
    size--;
    maxHeapify(1);
    return maxNode;
}

void MaxHeap::removeAt(int idx) {
    if (idx < 1 || idx > size) {
        return;
    }
    items[idx]->maxHeapIdx = -1;
    if (idx == size) {
        items.pop_back();
        size--;
        return;
    }
    items[idx] = items[size];
    items[idx]->maxHeapIdx = idx;
    items.pop_back();
    size--;

    if (idx > 1 && items[idx]->data > items[idx / 2]->data) {
        heapifyUp(idx);
    } else {
        maxHeapify(idx);
    }
}

bool MaxHeap::isEmpty() {
    return size == 0;
}

int MaxHeap::getSize() {
    return size;
}

class MinMaxStructure {
private:
    DoublyLinkedList dll;
    MinHeap minHeap;
    MaxHeap maxHeap;

public:
    void insert(int x);
    int findMin();
    int findMax();
    int deleteMin();
    int deleteMax();
    bool isEmpty();
    int getSize();
    void displayState(string action);
};

void MinMaxStructure::insert(int x) {
    DLLNode* node = dll.insertBack(x);
    minHeap.insert(node);
    maxHeap.insert(node);
}

int MinMaxStructure::findMin() {
    DLLNode* minNode = minHeap.getMin();
    if (minNode == nullptr) {
        return INT_MAX;
    }
    return minNode->data;
}

int MinMaxStructure::findMax() {
    DLLNode* maxNode = maxHeap.getMax();
    if (maxNode == nullptr) {
        return INT_MIN;
    }
    return maxNode->data;
}

int MinMaxStructure::deleteMin() {
    if (isEmpty()) {
        return INT_MAX;
    }
    DLLNode* minNode = minHeap.extractMin();
    maxHeap.removeAt(minNode->maxHeapIdx);
    int val = minNode->data;
    dll.removeNode(minNode);
    return val;
}

int MinMaxStructure::deleteMax() {
    if (isEmpty()) {
        return INT_MIN;
    }
    DLLNode* maxNode = maxHeap.extractMax();
    minHeap.removeAt(maxNode->minHeapIdx);
    int val = maxNode->data;
    dll.removeNode(maxNode);
    return val;
}

bool MinMaxStructure::isEmpty() {
    return minHeap.isEmpty();
}

int MinMaxStructure::getSize() {
    return minHeap.getSize();
}

void MinMaxStructure::displayState(string action) {
    cout << action << " -> DLL: ";
    dll.printList();
    if (isEmpty()) {
        cout << " | size=0 (empty)\n";
    } else {
        cout << " | min=" << findMin() << ", max=" << findMax()
             << ", size=" << getSize() << "\n";
    }
}

int main() {
    MinMaxStructure ds;
    cout << "=== Test Case 1: Interleaved Inserts, Min/Max Queries & Deletions ===\n";

    vector<int> initial = {10, 20, 30, 40, 50};
    for (int i = 0; i < (int)initial.size(); i++) {
        ds.insert(initial[i]);
        ds.displayState("insert(" + to_string(initial[i]) + ")");
    }

    int removedMin = ds.deleteMin();
    ds.displayState("deleteMin() => " + to_string(removedMin));

    int removedMax = ds.deleteMax();
    ds.displayState("deleteMax() => " + to_string(removedMax));

    ds.insert(5);
    ds.displayState("insert(5)");

    ds.insert(35);
    ds.displayState("insert(35)");

    ds.insert(5);
    ds.displayState("insert(5) (duplicate)");

    cout << "\nDraining alternating deleteMax and deleteMin:\n";
    bool takeMax = true;
    while (!ds.isEmpty()) {
        if (takeMax) {
            int val = ds.deleteMax();
            ds.displayState("deleteMax() => " + to_string(val));
        } else {
            int val = ds.deleteMin();
            ds.displayState("deleteMin() => " + to_string(val));
        }
        takeMax = !takeMax;
    }

    cout << "\n=== Test Case 2: Deleting from Empty Structure ===\n";
    cout << "isEmpty(): " << (ds.isEmpty() ? "true" : "false") << "\n";
    cout << "deleteMin() on empty: " << (ds.deleteMin() == INT_MAX ? "EMPTY" : "OK") << "\n";
    cout << "deleteMax() on empty: " << (ds.deleteMax() == INT_MIN ? "EMPTY" : "OK") << "\n";

    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

This is the **Indexed / Cross-Referenced Heap + DLL** pattern — crucial when `std::priority_queue` isn't enough because you need **`O(log n)` arbitrary deletion or key update**:

- **LC 480 — Sliding Window Median:** Two heaps where elements leaving the window must be removed from either heap (`removeAt` / lazy deletion).
- **LC 895 / LC 1172 / Design Problems (e.g., LC 2349 — Design a Number Container System, LC 2353 — Design a Food Rating System):** Synchronizing a heap with external IDs or nodes when items are deleted or updated out of order.
- **LC 146 / LC 460 — LRU / LFU Cache:** Same philosophy of storing a pointer/handle to a doubly linked list node for `O(1)` unlink.
- **LC 1825 — Finding MK Average:** Maintaining order statistics across multiple balanced structures under dynamic insertions and deletions.

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the solution to fixed-size C++ arrays matching `Heap-Implementation-OPTIMIZED.cpp` (`#define MAX 100`, `DLLNode* items[MAX]`, `int size`, 1-indexed `MinHeap` and `MaxHeap` with `heapifyUp`, `minHeapify`/`maxHeapify`, `insert`, `getMin`/`getMax`, `extractMin`/`extractMax`, `removeAt`, `isEmpty`, `getSize`, and `display`):
- Both `MinHeap` and `MaxHeap` replace `vector<DLLNode*>` with fixed `DLLNode* items[MAX]` arrays while keeping `minHeapIdx` / `maxHeapIdx` synchronized on every swap, extraction, and `removeAt`.
- `DoublyLinkedChain` manages the `DLLNode` lifetime and `O(1)` unlinking without any STL container.
- Avoids `std::vector`, `std::string`, `to_string`, globals, `const`, `static`, and `constexpr`.

Source file: [13-data-structure-with-min-and-max-array.cpp](13-data-structure-with-min-and-max-array.cpp)

```cpp
#include <climits>
#include <iostream>
#define MAX 100
using namespace std;

struct DLLNode {
    int data;
    int minHeapIdx;
    int maxHeapIdx;
    DLLNode* prev;
    DLLNode* next;

    DLLNode(int val) {
        data = val;
        minHeapIdx = -1;
        maxHeapIdx = -1;
        prev = nullptr;
        next = nullptr;
    }
};

class DoublyLinkedChain {
private:
    DLLNode* head;
    DLLNode* tail;

public:
    DoublyLinkedChain();
    ~DoublyLinkedChain();
    DLLNode* insertBack(int val);
    void removeNode(DLLNode* node);
    void printChain();
};

DoublyLinkedChain::DoublyLinkedChain() {
    head = nullptr;
    tail = nullptr;
}

DoublyLinkedChain::~DoublyLinkedChain() {
    DLLNode* curr = head;
    while (curr != nullptr) {
        DLLNode* nxt = curr->next;
        delete curr;
        curr = nxt;
    }
}

DLLNode* DoublyLinkedChain::insertBack(int val) {
    DLLNode* node = new DLLNode(val);
    if (tail == nullptr) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    return node;
}

void DoublyLinkedChain::removeNode(DLLNode* node) {
    if (node == nullptr) {
        return;
    }
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }
    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }
    delete node;
}

void DoublyLinkedChain::printChain() {
    DLLNode* curr = head;
    cout << "[";
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next != nullptr) {
            cout << ", ";
        }
        curr = curr->next;
    }
    cout << "]";
}

class MinHeap {
private:
    DLLNode* items[MAX];
    int size;

    void swapNodes(int i, int j);
    void heapifyUp(int curr);
    void minHeapify(int curr);

public:
    MinHeap();
    bool insert(DLLNode* node);
    DLLNode* getMin();
    DLLNode* extractMin();
    void removeAt(int idx);
    bool isEmpty();
    int getSize();
    void display();
};

MinHeap::MinHeap() {
    size = 0;
}

void MinHeap::swapNodes(int i, int j) {
    DLLNode* temp = items[i];
    items[i] = items[j];
    items[j] = temp;
    items[i]->minHeapIdx = i;
    items[j]->minHeapIdx = j;
}

void MinHeap::heapifyUp(int curr) {
    while (curr / 2 > 0 && items[curr]->data < items[curr / 2]->data) {
        swapNodes(curr, curr / 2);
        curr = curr / 2;
    }
}

void MinHeap::minHeapify(int curr) {
    while (curr * 2 <= size) {
        int s = curr * 2;
        if (s + 1 <= size && items[s]->data > items[s + 1]->data) {
            s++;
        }
        if (items[s]->data < items[curr]->data) {
            swapNodes(curr, s);
            curr = s;
        } else {
            break;
        }
    }
}

bool MinHeap::insert(DLLNode* node) {
    if (size < MAX - 1) {
        items[++size] = node;
        node->minHeapIdx = size;
        heapifyUp(size);
        return true;
    }
    return false;
}

DLLNode* MinHeap::getMin() {
    if (size == 0) {
        return nullptr;
    }
    return items[1];
}

DLLNode* MinHeap::extractMin() {
    if (size == 0) {
        return nullptr;
    }
    DLLNode* minNode = items[1];
    minNode->minHeapIdx = -1;
    if (size == 1) {
        size = 0;
        return minNode;
    }
    items[1] = items[size];
    items[1]->minHeapIdx = 1;
    size--;
    minHeapify(1);
    return minNode;
}

void MinHeap::removeAt(int idx) {
    if (idx < 1 || idx > size) {
        return;
    }
    items[idx]->minHeapIdx = -1;
    if (idx == size) {
        size--;
        return;
    }
    items[idx] = items[size];
    items[idx]->minHeapIdx = idx;
    size--;

    if (idx > 1 && items[idx]->data < items[idx / 2]->data) {
        heapifyUp(idx);
    } else {
        minHeapify(idx);
    }
}

bool MinHeap::isEmpty() {
    return size == 0;
}

int MinHeap::getSize() {
    return size;
}

void MinHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << items[i]->data << " ";
    }
    cout << endl;
}

class MaxHeap {
private:
    DLLNode* items[MAX];
    int size;

    void swapNodes(int i, int j);
    void heapifyUp(int curr);
    void maxHeapify(int curr);

public:
    MaxHeap();
    bool insert(DLLNode* node);
    DLLNode* getMax();
    DLLNode* extractMax();
    void removeAt(int idx);
    bool isEmpty();
    int getSize();
    void display();
};

MaxHeap::MaxHeap() {
    size = 0;
}

void MaxHeap::swapNodes(int i, int j) {
    DLLNode* temp = items[i];
    items[i] = items[j];
    items[j] = temp;
    items[i]->maxHeapIdx = i;
    items[j]->maxHeapIdx = j;
}

void MaxHeap::heapifyUp(int curr) {
    while (curr / 2 > 0 && items[curr]->data > items[curr / 2]->data) {
        swapNodes(curr, curr / 2);
        curr = curr / 2;
    }
}

void MaxHeap::maxHeapify(int curr) {
    while (curr * 2 <= size) {
        int s = curr * 2;
        if (s + 1 <= size && items[s]->data < items[s + 1]->data) {
            s++;
        }
        if (items[s]->data > items[curr]->data) {
            swapNodes(curr, s);
            curr = s;
        } else {
            break;
        }
    }
}

bool MaxHeap::insert(DLLNode* node) {
    if (size < MAX - 1) {
        items[++size] = node;
        node->maxHeapIdx = size;
        heapifyUp(size);
        return true;
    }
    return false;
}

DLLNode* MaxHeap::getMax() {
    if (size == 0) {
        return nullptr;
    }
    return items[1];
}

DLLNode* MaxHeap::extractMax() {
    if (size == 0) {
        return nullptr;
    }
    DLLNode* maxNode = items[1];
    maxNode->maxHeapIdx = -1;
    if (size == 1) {
        size = 0;
        return maxNode;
    }
    items[1] = items[size];
    items[1]->maxHeapIdx = 1;
    size--;
    maxHeapify(1);
    return maxNode;
}

void MaxHeap::removeAt(int idx) {
    if (idx < 1 || idx > size) {
        return;
    }
    items[idx]->maxHeapIdx = -1;
    if (idx == size) {
        size--;
        return;
    }
    items[idx] = items[size];
    items[idx]->maxHeapIdx = idx;
    size--;

    if (idx > 1 && items[idx]->data > items[idx / 2]->data) {
        heapifyUp(idx);
    } else {
        maxHeapify(idx);
    }
}

bool MaxHeap::isEmpty() {
    return size == 0;
}

int MaxHeap::getSize() {
    return size;
}

void MaxHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << items[i]->data << " ";
    }
    cout << endl;
}

class MinMaxStructure {
private:
    DoublyLinkedChain dll;
    MinHeap minHeap;
    MaxHeap maxHeap;

public:
    bool insert(int x);
    int findMin();
    int findMax();
    int deleteMin();
    int deleteMax();
    bool isEmpty();
    int getSize();
    void displayState();
};

bool MinMaxStructure::insert(int x) {
    if (minHeap.getSize() >= MAX - 1) {
        return false;
    }
    DLLNode* node = dll.insertBack(x);
    minHeap.insert(node);
    maxHeap.insert(node);
    return true;
}

int MinMaxStructure::findMin() {
    DLLNode* minNode = minHeap.getMin();
    if (minNode == nullptr) {
        return INT_MAX;
    }
    return minNode->data;
}

int MinMaxStructure::findMax() {
    DLLNode* maxNode = maxHeap.getMax();
    if (maxNode == nullptr) {
        return INT_MIN;
    }
    return maxNode->data;
}

int MinMaxStructure::deleteMin() {
    if (isEmpty()) {
        return INT_MAX;
    }
    DLLNode* minNode = minHeap.extractMin();
    maxHeap.removeAt(minNode->maxHeapIdx);
    int val = minNode->data;
    dll.removeNode(minNode);
    return val;
}

int MinMaxStructure::deleteMax() {
    if (isEmpty()) {
        return INT_MIN;
    }
    DLLNode* maxNode = maxHeap.extractMax();
    minHeap.removeAt(maxNode->minHeapIdx);
    int val = maxNode->data;
    dll.removeNode(maxNode);
    return val;
}

bool MinMaxStructure::isEmpty() {
    return minHeap.isEmpty();
}

int MinMaxStructure::getSize() {
    return minHeap.getSize();
}

void MinMaxStructure::displayState() {
    cout << "DLL: ";
    dll.printChain();
    if (isEmpty()) {
        cout << " | size=0 (empty)\n";
    } else {
        cout << " | min=" << findMin() << ", max=" << findMax()
             << ", size=" << getSize() << "\n";
    }
}

int main() {
    MinMaxStructure ds;
    cout << "=== Test Case 1: Interleaved Inserts, Min/Max Queries & Deletions ===\n";

    int initial[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        ds.insert(initial[i]);
        cout << "insert(" << initial[i] << ") -> ";
        ds.displayState();
    }

    int removedMin = ds.deleteMin();
    cout << "deleteMin() => " << removedMin << " -> ";
    ds.displayState();

    int removedMax = ds.deleteMax();
    cout << "deleteMax() => " << removedMax << " -> ";
    ds.displayState();

    ds.insert(5);
    cout << "insert(5) -> ";
    ds.displayState();

    ds.insert(35);
    cout << "insert(35) -> ";
    ds.displayState();

    ds.insert(5);
    cout << "insert(5) (duplicate) -> ";
    ds.displayState();

    cout << "\nDraining alternating deleteMax and deleteMin:\n";
    bool takeMax = true;
    while (!ds.isEmpty()) {
        if (takeMax) {
            int val = ds.deleteMax();
            cout << "deleteMax() => " << val << " -> ";
            ds.displayState();
        } else {
            int val = ds.deleteMin();
            cout << "deleteMin() => " << val << " -> ";
            ds.displayState();
        }
        takeMax = !takeMax;
    }

    cout << "\n=== Test Case 2: Deleting from Empty Structure ===\n";
    cout << "isEmpty(): " << (ds.isEmpty() ? "true" : "false") << "\n";
    cout << "deleteMin() on empty: " << (ds.deleteMin() == INT_MAX ? "EMPTY" : "OK") << "\n";
    cout << "deleteMax() on empty: " << (ds.deleteMax() == INT_MIN ? "EMPTY" : "OK") << "\n";

    return 0;
}
```
