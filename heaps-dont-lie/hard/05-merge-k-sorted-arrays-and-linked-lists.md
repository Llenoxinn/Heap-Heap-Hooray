# Merge K Sorted Arrays and Linked Lists

- **Difficulty:** Hard
- **References:**
  - [GeeksforGeeks — Merge K Sorted Arrays](https://www.geeksforgeeks.org/dsa/merge-k-sorted-arrays/)
  - [GeeksforGeeks — Merge K Sorted Linked Lists](https://www.geeksforgeeks.org/dsa/merge-k-sorted-linked-lists/)

---

## 1. Problem Statement

Solve two classic K-way merge problems using a custom **Min-Heap**:
- **Part A (Arrays):** Given `k` sorted integer arrays, merge them into one sorted array.
- **Part B (Linked Lists):** Given `k` sorted singly linked lists, merge them into one sorted singly linked list.

---

## 2. Pattern Recognition & Intuition

### Why think "Heap" here?
At every step of merging `k` sorted sequences, the next global minimum **must** be one of the `k` current frontier heads (the smallest unconsumed element from each sequence).
- Scanning all `k` heads linearly takes $O(k)$ per output element $\rightarrow O(N \cdot k)$ total.
- Putting the `k` frontier heads into a **Min-Heap** shrinks finding the next minimum from $O(k)$ to $O(\log k)$!

Whenever you see **"K sorted lists/arrays/streams"**, immediately think **K-Way Merge with a Min-Heap of size $\le k$**.

### Min-Heap vs. Max-Heap Decision Rule
- We build the merged result from smallest to largest, so we need the **minimum** among the `k` active frontier elements $\rightarrow$ **Min-Heap**.
- Using a Max-Heap would only let us merge in reverse (from largest to smallest starting at the tails, which is awkward for singly linked lists).

### Why this beats naive approaches
- **Concatenate and sort:** $O(N \log N)$ time, ignoring the fact that each input list is already sorted! When $k \ll N$, $O(N \log k)$ is dramatically faster than $O(N \log N)$.
- **Merge lists one by one (`k - 1` merges):** Takes $O(N \cdot k)$ in the worst case.
- **Min-Heap K-way merge:** Always maintains at most `k` items in the heap, running in $O(N \log k)$ time and $O(k)$ auxiliary heap space.

### Key Invariants & Edge Cases
- **Frontier invariant:** At any moment, the `MinHeap` holds **at most one element per non-exhausted input sequence** — specifically, its next unconsumed minimum.
- **Arrays vs. Linked Lists difference:**
  - An **array entry** needs `{val, arrIdx, elemIdx}` so we know which array to advance (`elemIdx + 1`).
  - A **linked list node** (`ListNode*`) already carries `node->next` inside the pointer itself! So we can store `ListNode*` directly in the heap, relink `tail->next = smallest`, and push `smallest->next` if non-null — zero extra node allocations!
- **Edge cases:** Empty outer input (`k = 0`), individual empty arrays (`{}`), or `nullptr` linked lists are skipped during initial heap seeding.

---

## 3. Step-by-Step Trace

Using **Test 1** from `main()` (same values for both Part A and Part B):
`sequences = [[1, 4, 5], [1, 3, 4], [2, 6]]` (`k = 3`, total `N = 8`)

1. **Seed Min-Heap** with the head of each sequence:
   - `(1, seq 0)`, `(1, seq 1)`, `(2, seq 2)`

2. **Pop minimum and push successor from that same sequence:**

| Step | `extractMin()` | Output so far | Next pushed from same sequence | Heap Contents `(val, seq)` |
| :--- | :--- | :--- | :--- | :--- |
| Init | — | `[]` | Seeded heads | `(1, s0), (1, s1), (2, s2)` |
| 1 | `(1, s0)` | `[1]` | `(4, s0)` | `(1, s1), (2, s2), (4, s0)` |
| 2 | `(1, s1)` | `[1, 1]` | `(3, s1)` | `(2, s2), (3, s1), (4, s0)` |
| 3 | `(2, s2)` | `[1, 1, 2]` | `(6, s2)` | `(3, s1), (4, s0), (6, s2)` |
| 4 | `(3, s1)` | `[1, 1, 2, 3]` | `(4, s1)` | `(4, s0), (4, s1), (6, s2)` |
| 5 | `(4, s0)` | `[1, 1, 2, 3, 4]` | `(5, s0)` | `(4, s1), (5, s0), (6, s2)` |
| 6 | `(4, s1)` | `[1, 1, 2, 3, 4, 4]` | None (`s1` exhausted) | `(5, s0), (6, s2)` |
| 7 | `(5, s0)` | `[1, 1, 2, 3, 4, 4, 5]` | None (`s0` exhausted) | `(6, s2)` |
| 8 | `(6, s2)` | `[1, 1, 2, 3, 4, 4, 5, 6]` | None (`s2` exhausted) | `[]` |

Final Output:
- Part A: `[1, 1, 2, 3, 4, 4, 5, 6]`
- Part B: `1 -> 1 -> 2 -> 3 -> 4 -> 4 -> 5 -> 6`

---

## 4. Time & Space Complexity

Let $N$ be the total number of elements across all $k$ sequences.
- **Time Complexity:** $O(N \log k)$
  - Initial heap construction inserts at most $k$ heads: $O(k \log k)$.
  - Each of the $N$ elements is inserted once and extracted once from a heap of size $\le k$: $O(N \log k)$.
- **Space Complexity:**
  - **Part A (Arrays):** $O(k)$ auxiliary heap space + $O(N)$ output array.
  - **Part B (Linked Lists):** $O(k)$ auxiliary heap space and $O(1)$ extra node allocations (reuses existing `ListNode` objects in-place).

---

## 5. C++ Implementation

See [05-merge-k-sorted-arrays-and-linked-lists.cpp](05-merge-k-sorted-arrays-and-linked-lists.cpp).

```cpp
#include <iostream>
#include <vector>
using namespace std;

struct ArrayEntry {
    int val;
    int arrIdx;
    int elemIdx;
};

struct ArrayHigherPriority {
    bool operator()(ArrayEntry a, ArrayEntry b) {
        return a.val < b.val;
    }
};

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) {
        val = v;
        next = nullptr;
    }
};

struct ListNodeHigherPriority {
    bool operator()(ListNode* a, ListNode* b) {
        return a->val < b->val;
    }
};

template <typename T, typename Comp>
class MinHeap {
private:
    vector<T> items;
    int size;
    Comp isSmaller;

    void heapifyUp(int curr) {
        T x = items[curr];
        while (curr / 2 > 0 && isSmaller(x, items[curr / 2])) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        T x = items[curr];
        while (curr * 2 <= size) {
            int smallest = curr * 2;
            if (smallest + 1 <= size && isSmaller(items[smallest + 1], items[smallest])) {
                smallest++;
            }
            if (isSmaller(items[smallest], x)) {
                items[curr] = items[smallest];
                curr = smallest;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MinHeap() {
        items.push_back(T());
        size = 0;
    }

    bool insert(T item) {
        if (size + 1 < (int)items.size()) {
            items[++size] = item;
        } else {
            items.push_back(item);
            size++;
        }
        heapifyUp(size);
        return true;
    }

    T getMin() {
        if (size == 0) {
            return T();
        }
        return items[1];
    }

    T extractMin() {
        if (size == 0) {
            return T();
        }
        T top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return top;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

// Part A: Merge k sorted arrays
vector<int> mergeKSortedArrays(vector<vector<int>>& arrays) {
    MinHeap<ArrayEntry, ArrayHigherPriority> heap;

    for (int i = 0; i < (int)arrays.size(); i++) {
        if (!arrays[i].empty()) {
            heap.insert({arrays[i][0], i, 0});
        }
    }

    vector<int> merged;
    while (!heap.isEmpty()) {
        ArrayEntry smallest = heap.extractMin();
        merged.push_back(smallest.val);

        int nextIdx = smallest.elemIdx + 1;
        if (nextIdx < (int)arrays[smallest.arrIdx].size()) {
            heap.insert({arrays[smallest.arrIdx][nextIdx], smallest.arrIdx, nextIdx});
        }
    }

    return merged;
}

// Part B: Merge k sorted linked lists
ListNode* mergeKSortedLists(vector<ListNode*>& lists) {
    MinHeap<ListNode*, ListNodeHigherPriority> heap;

    for (int i = 0; i < (int)lists.size(); i++) {
        if (lists[i] != nullptr) {
            heap.insert(lists[i]);
        }
    }

    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (!heap.isEmpty()) {
        ListNode* smallest = heap.extractMin();
        tail->next = smallest;
        tail = tail->next;

        if (smallest->next != nullptr) {
            heap.insert(smallest->next);
        }
    }
    tail->next = nullptr;

    return dummy.next;
}

ListNode* buildList(vector<int>& vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int i = 0; i < (int)vals.size(); i++) {
        tail->next = new ListNode(vals[i]);
        tail = tail->next;
    }
    return dummy.next;
}

void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* nxt = head->next;
        delete head;
        head = nxt;
    }
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) {
            cout << ", ";
        }
    }
    cout << "]\n";
}

void printLinkedList(ListNode* head) {
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

int main() {
    // Part A tests
    vector<vector<int>> arrays1 = {
        {1, 4, 5},
        {1, 3, 4},
        {2, 6}
    };
    vector<int> mergedArr1 = mergeKSortedArrays(arrays1);
    cout << "Part A Test 1 (Merge K Sorted Arrays): ";
    printVector(mergedArr1);

    vector<vector<int>> arrays2 = {
        {},
        {2, 2, 10},
        {-3, 0, 5, 9}
    };
    vector<int> mergedArr2 = mergeKSortedArrays(arrays2);
    cout << "Part A Test 2 (With empty array and negatives): ";
    printVector(mergedArr2);

    // Part B tests
    vector<int> l1 = {1, 4, 5};
    vector<int> l2 = {1, 3, 4};
    vector<int> l3 = {2, 6};
    vector<ListNode*> lists1 = {buildList(l1), buildList(l2), buildList(l3)};
    ListNode* mergedList1 = mergeKSortedLists(lists1);
    cout << "Part B Test 1 (Merge K Sorted Linked Lists): ";
    printLinkedList(mergedList1);
    freeList(mergedList1);

    vector<int> l4 = {-10, -2, 7};
    vector<int> l5 = {-5, 0, 12};
    vector<ListNode*> lists2 = {nullptr, buildList(l4), buildList(l5)};
    ListNode* mergedList2 = mergeKSortedLists(lists2);
    cout << "Part B Test 2 (With nullptr list and negatives): ";
    printLinkedList(mergedList2);
    freeList(mergedList2);

    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

The **K-Way Merge via Min-Heap** pattern directly solves:
- **LC 23 — Merge k Sorted Lists** (exact Part B problem)
- **LC 378 — Kth Smallest Element in a Sorted Matrix** (each row is a sorted list; stop after $k$ extractions)
- **LC 373 — Find K Pairs with Smallest Sums** (implicit $k$-way merge over pairs `(nums1[i], nums2[j])`)
- **LC 632 — Smallest Range Covering Elements from K Lists** (K-way merge + tracking running max)
- **LC 786 — K-th Smallest Prime Fraction** (sorted rows of fractions `arr[i]/arr[j]`)
