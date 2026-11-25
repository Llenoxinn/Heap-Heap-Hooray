# 09. Sort Numbers Stored on Different Machines (K-Way Merge)

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — Sort Numbers Stored on Different Machines](https://www.geeksforgeeks.org/dsa/sort-numbers-stored-on-different-machines/)

---

## 1. Problem Statement

Given `K` machines, where each machine stores a sorted singly linked list of integers, combine all numbers into a single globally sorted sequence (without loading everything into an array and re-sorting from scratch).

---

## 2. Pattern Recognition & Intuition

### Why Think "Heap" Here?
You have `K` already-sorted streams (linked lists) and want one globally sorted output. Because each list is sorted, the next smallest unconsumed value overall **must** be one of the `K` current list heads.

- **Naive approach 1 (Concatenate + Sort):** Collect all `N` elements into an array and sort in `O(N log N)`. This ignores the fact that each machine is *already* sorted!
- **Naive approach 2 (Linear scan across `K` heads):** Scan all `K` heads for every element -> `O(N * K)`, which is slow when `K` is large.
- **Heap approach (K-Way Merge):** Keep the current head `ListNode*` of each non-empty machine in a **Min-Heap** of size at most `K`. Extracting the global minimum and advancing that machine's pointer takes `O(log K)` per element, giving `O(N log K)` total time!

### Min-Heap vs. Max-Heap Decision Rule
- We want the globally **smallest** head at every step to emit ascending order, so we use a **Min-Heap** ordered by `node->val`.
- Storing `ListNode*` directly in the heap is especially clean: `node->next` gives immediate `O(1)` access to the next element on that same machine without tracking machine IDs separately.

### Key Invariants & Edge Cases
1. **Heap size invariant:** At any moment, the Min-Heap holds at most one node per machine (size `<= K`).
2. **Sorted-prefix invariant:** Every node popped from the heap is `>=` the previously popped node and `<=` all remaining nodes in all `K` lists.
3. **Edge cases:**
   - Empty machine lists (`nullptr` heads must not be inserted into the heap).
   - Duplicate values across machines or within the same machine.
   - `K = 0` or all machines empty -> returns `nullptr`.

---

## 3. Step-by-Step Trace (Test Case 1 from `main()`)

**Input:**
- Machine 0: `30 -> 40 -> 50`
- Machine 1: `35 -> 45`
- Machine 2: `10 -> 60 -> 70 -> 80 -> 100`

| Step | Min-Heap Nodes (by `val`) | Extracted Min | Next Node Inserted | Merged List So Far |
| :--- | :--- | :--- | :--- | :--- |
| Init | `[10, 30, 35]` | — | — | `[]` |
| 1 | `[10, 30, 35]` | `10` (M2) | `60` (M2) | `10` |
| 2 | `[30, 35, 60]` | `30` (M0) | `40` (M0) | `10 -> 30` |
| 3 | `[35, 40, 60]` | `35` (M1) | `45` (M1) | `10 -> 30 -> 35` |
| 4 | `[40, 45, 60]` | `40` (M0) | `50` (M0) | `10 -> 30 -> 35 -> 40` |
| 5 | `[45, 50, 60]` | `45` (M1) | `nullptr` | `10 -> 30 -> 35 -> 40 -> 45` |
| 6 | `[50, 60]` | `50` (M0) | `nullptr` | `10 -> 30 -> 35 -> 40 -> 45 -> 50` |
| 7 | `[60]` | `60` (M2) | `70` (M2) | `... -> 60` |
| 8 | `[70]` | `70` (M2) | `80` (M2) | `... -> 70` |
| 9 | `[80]` | `80` (M2) | `100` (M2) | `... -> 80` |
| 10 | `[100]` | `100` (M2) | `nullptr` | `10 -> 30 -> 35 -> 40 -> 45 -> 50 -> 60 -> 70 -> 80 -> 100` |

---

## 4. Time & Space Complexity

Let `N` be the total number of elements across all `K` machines.
- **Time Complexity:** `O(N log K)`. Building the initial heap of up to `K` heads takes `O(K log K)` (and `K <= N`), followed by `N` extractions and up to `N` insertions into a heap of size `<= K`.
- **Space Complexity:** `O(K)` auxiliary space for the Min-Heap (we relink the existing `ListNode` pointers in-place rather than allocating new nodes).

---

## 5. C++ Implementation

Source file: [09-sort-from-different-machines.cpp](09-sort-from-different-machines.cpp)

```cpp
#include <iostream>
#include <vector>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

class MinHeap {
private:
    vector<ListNode*> items;
    int size;

    void heapifyUp(int curr);
    void minHeapify(int curr);

public:
    MinHeap();
    void insert(ListNode* node);
    ListNode* getMin();
    ListNode* extractMin();
    bool isEmpty();
    int getSize();
};

MinHeap::MinHeap() {
    items.push_back(nullptr);
    size = 0;
}

void MinHeap::heapifyUp(int curr) {
    ListNode* x = items[curr];
    while (curr / 2 > 0 && x->val < items[curr / 2]->val) {
        items[curr] = items[curr / 2];
        curr = curr / 2;
    }
    items[curr] = x;
}

void MinHeap::minHeapify(int curr) {
    ListNode* x = items[curr];
    while (curr * 2 <= size) {
        int s = curr * 2;
        if (s + 1 <= size && items[s]->val > items[s + 1]->val) {
            s++;
        }
        if (items[s]->val < x->val) {
            items[curr] = items[s];
            curr = s;
        } else {
            break;
        }
    }
    items[curr] = x;
}

void MinHeap::insert(ListNode* node) {
    if (node == nullptr) {
        return;
    }
    items.push_back(node);
    size++;
    heapifyUp(size);
}

ListNode* MinHeap::getMin() {
    if (size == 0) {
        return nullptr;
    }
    return items[1];
}

ListNode* MinHeap::extractMin() {
    if (size == 0) {
        return nullptr;
    }
    ListNode* minNode = items[1];
    items[1] = items[size];
    items.pop_back();
    size--;
    if (size > 0) {
        minHeapify(1);
    }
    return minNode;
}

bool MinHeap::isEmpty() {
    return size == 0;
}

int MinHeap::getSize() {
    return size;
}

ListNode* sortFromDifferentMachines(vector<ListNode*>& machines) {
    MinHeap pq;
    for (int i = 0; i < (int)machines.size(); i++) {
        if (machines[i] != nullptr) {
            pq.insert(machines[i]);
        }
    }

    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (!pq.isEmpty()) {
        ListNode* smallest = pq.extractMin();
        tail->next = smallest;
        tail = tail->next;

        if (smallest->next != nullptr) {
            pq.insert(smallest->next);
        }
    }
    tail->next = nullptr;
    return dummy.next;
}

ListNode* buildList(vector<int> values) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int i = 0; i < (int)values.size(); i++) {
        tail->next = new ListNode(values[i]);
        tail = tail->next;
    }
    return dummy.next;
}

void printList(ListNode* head) {
    if (head == nullptr) {
        cout << "(empty)\n";
        return;
    }
    ListNode* curr = head;
    while (curr != nullptr) {
        cout << curr->val;
        if (curr->next != nullptr) {
            cout << " -> ";
        }
        curr = curr->next;
    }
    cout << "\n";
}

void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* nxt = head->next;
        delete head;
        head = nxt;
    }
}

int main() {
    // Test Case 1: 3 machines with sorted linked lists of varying lengths
    vector<ListNode*> machines1;
    machines1.push_back(buildList({30, 40, 50}));
    machines1.push_back(buildList({35, 45}));
    machines1.push_back(buildList({10, 60, 70, 80, 100}));

    cout << "=== Test Case 1: 3 Machines ===\n";
    for (int i = 0; i < (int)machines1.size(); i++) {
        cout << "Machine " << i << ": ";
        printList(machines1[i]);
    }
    ListNode* merged1 = sortFromDifferentMachines(machines1);
    cout << "Merged sorted output: ";
    printList(merged1);
    freeList(merged1);

    // Test Case 2: Duplicate values and an empty machine
    vector<ListNode*> machines2;
    machines2.push_back(buildList({1, 4, 5}));
    machines2.push_back(buildList({}));
    machines2.push_back(buildList({1, 3, 4}));
    machines2.push_back(buildList({2, 6}));

    cout << "\n=== Test Case 2: Duplicates & Empty Machine ===\n";
    for (int i = 0; i < (int)machines2.size(); i++) {
        cout << "Machine " << i << ": ";
        printList(machines2[i]);
    }
    ListNode* merged2 = sortFromDifferentMachines(machines2);
    cout << "Merged sorted output: ";
    printList(merged2);
    freeList(merged2);

    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

This is the canonical **K-Way Merge** pattern (`O(N log K)`). Look for **"K sorted lists/arrays/rows/streams"**:

- **LC 23 — Merge k Sorted Lists:** Literally this exact problem on `ListNode*`.
- **LC 378 — Kth Smallest Element in a Sorted Matrix:** Treat each row as a sorted stream and extract `k` times from a Min-Heap.
- **LC 373 — Find K Pairs with Smallest Sums:** Treat `(nums1[i], nums2[0..m-1])` as `n` sorted streams.
- **LC 632 — Smallest Range Covering Elements from K Lists:** Min-Heap tracks the current minimum across `K` lists while a variable tracks the current maximum.

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the solution to fixed-size C++ arrays matching `Heap-Implementation-OPTIMIZED.cpp` (`#define MAX 100`, `ListNode* items[MAX]`, `int size`, 1-indexed `MinHeap` with `heapifyUp`, `minHeapify`, `insert`, `getMin`, `extractMin`, `isEmpty`, `getSize`, and `display`):
- `MinHeap` replaces `vector<ListNode*>` with `ListNode* items[MAX]` and enforces `size < MAX - 1`.
- `sortFromDifferentMachines` takes a raw `ListNode* machines[]` array and machine count `int k`, and `buildChain` builds each linked list from a raw `int values[]` array and length `int n`.
- Avoids all STL containers, globals, `const`, `static`, and `constexpr`.

Source file: [09-sort-from-different-machines-array.cpp](09-sort-from-different-machines-array.cpp)

```cpp
#include <iostream>
#define MAX 100
#define MAX_MACHINES 20
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

class MinHeap {
private:
    ListNode* items[MAX];
    int size;

    void heapifyUp(int curr);
    void minHeapify(int curr);

public:
    MinHeap();
    bool insert(ListNode* node);
    ListNode* getMin();
    ListNode* extractMin();
    bool isEmpty();
    int getSize();
    void display();
};

MinHeap::MinHeap() {
    size = 0;
}

void MinHeap::heapifyUp(int curr) {
    ListNode* x = items[curr];
    while (curr / 2 > 0 && x->val < items[curr / 2]->val) {
        items[curr] = items[curr / 2];
        curr = curr / 2;
    }
    items[curr] = x;
}

void MinHeap::minHeapify(int curr) {
    ListNode* x = items[curr];
    while (curr * 2 <= size) {
        int s = curr * 2;
        if (s + 1 <= size && items[s]->val > items[s + 1]->val) {
            s++;
        }
        if (items[s]->val < x->val) {
            items[curr] = items[s];
            curr = s;
        } else {
            break;
        }
    }
    items[curr] = x;
}

bool MinHeap::insert(ListNode* node) {
    if (node == nullptr) {
        return false;
    }
    if (size < MAX - 1) {
        items[++size] = node;
        heapifyUp(size);
        return true;
    }
    return false;
}

ListNode* MinHeap::getMin() {
    if (size == 0) {
        return nullptr;
    }
    return items[1];
}

ListNode* MinHeap::extractMin() {
    if (size == 0) {
        return nullptr;
    }
    ListNode* minNode = items[1];
    items[1] = items[size];
    size--;
    if (size > 0) {
        minHeapify(1);
    }
    return minNode;
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
        cout << items[i]->val << " ";
    }
    cout << endl;
}

ListNode* sortFromDifferentMachines(ListNode* machines[], int k) {
    MinHeap pq;
    for (int i = 0; i < k; i++) {
        if (machines[i] != nullptr) {
            pq.insert(machines[i]);
        }
    }

    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (!pq.isEmpty()) {
        ListNode* smallest = pq.extractMin();
        tail->next = smallest;
        tail = tail->next;

        if (smallest->next != nullptr) {
            pq.insert(smallest->next);
        }
    }
    tail->next = nullptr;
    return dummy.next;
}

ListNode* buildChain(int values[], int n) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int i = 0; i < n; i++) {
        tail->next = new ListNode(values[i]);
        tail = tail->next;
    }
    return dummy.next;
}

void printChain(ListNode* head) {
    if (head == nullptr) {
        cout << "(empty)\n";
        return;
    }
    ListNode* curr = head;
    while (curr != nullptr) {
        cout << curr->val;
        if (curr->next != nullptr) {
            cout << " -> ";
        }
        curr = curr->next;
    }
    cout << "\n";
}

void freeChain(ListNode* head) {
    while (head != nullptr) {
        ListNode* nxt = head->next;
        delete head;
        head = nxt;
    }
}

int main() {
    // Test Case 1: 3 machines with sorted linked chains of varying lengths
    int m1_0[] = {30, 40, 50};
    int m1_1[] = {35, 45};
    int m1_2[] = {10, 60, 70, 80, 100};

    ListNode* machines1[MAX_MACHINES];
    machines1[0] = buildChain(m1_0, 3);
    machines1[1] = buildChain(m1_1, 2);
    machines1[2] = buildChain(m1_2, 5);
    int k1 = 3;

    cout << "=== Test Case 1: 3 Machines ===\n";
    for (int i = 0; i < k1; i++) {
        cout << "Machine " << i << ": ";
        printChain(machines1[i]);
    }
    ListNode* merged1 = sortFromDifferentMachines(machines1, k1);
    cout << "Merged sorted output: ";
    printChain(merged1);
    freeChain(merged1);

    // Test Case 2: Duplicate values and an empty machine
    int m2_0[] = {1, 4, 5};
    int m2_2[] = {1, 3, 4};
    int m2_3[] = {2, 6};

    ListNode* machines2[MAX_MACHINES];
    machines2[0] = buildChain(m2_0, 3);
    machines2[1] = nullptr;
    machines2[2] = buildChain(m2_2, 3);
    machines2[3] = buildChain(m2_3, 2);
    int k2 = 4;

    cout << "\n=== Test Case 2: Duplicates & Empty Machine ===\n";
    for (int i = 0; i < k2; i++) {
        cout << "Machine " << i << ": ";
        printChain(machines2[i]);
    }
    ListNode* merged2 = sortFromDifferentMachines(machines2, k2);
    cout << "Merged sorted output: ";
    printChain(merged2);
    freeChain(merged2);

    return 0;
}
```
