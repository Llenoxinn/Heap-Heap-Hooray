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
