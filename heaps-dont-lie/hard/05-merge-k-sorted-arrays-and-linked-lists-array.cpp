#include <iostream>
#define MAX 100
#define MAX_K 20
#define MAX_COLS 50
using namespace std;

struct ArrayEntry {
    int val;
    int arrIdx;
    int elemIdx;
};

struct Node {
    int val;
    Node* next;
    Node(int v) {
        val = v;
        next = nullptr;
    }
};

class ArrayMinHeap {
private:
    ArrayEntry items[MAX];
    int size;

    void heapifyUp(int curr) {
        ArrayEntry x = items[curr];
        while (curr / 2 > 0 && x.val < items[curr / 2].val) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        ArrayEntry x = items[curr];
        while (curr * 2 <= size) {
            int smallest = curr * 2;
            if (smallest + 1 <= size && items[smallest + 1].val < items[smallest].val) {
                smallest++;
            }
            if (items[smallest].val < x.val) {
                items[curr] = items[smallest];
                curr = smallest;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    ArrayMinHeap() {
        size = 0;
    }

    bool insert(ArrayEntry item) {
        if (size < MAX - 1) {
            items[++size] = item;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    ArrayEntry getMin() {
        if (size == 0) {
            return {0, -1, -1};
        }
        return items[1];
    }

    ArrayEntry extractMin() {
        if (size == 0) {
            return {0, -1, -1};
        }
        ArrayEntry top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return top;
    }

    bool remove() {
        if (size == 0) {
            return false;
        }
        extractMin();
        return true;
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
            cout << items[i].val << " ";
        }
        cout << endl;
    }
};

class NodeMinHeap {
private:
    Node* items[MAX];
    int size;

    void heapifyUp(int curr) {
        Node* x = items[curr];
        while (curr / 2 > 0 && x->val < items[curr / 2]->val) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        Node* x = items[curr];
        while (curr * 2 <= size) {
            int smallest = curr * 2;
            if (smallest + 1 <= size && items[smallest + 1]->val < items[smallest]->val) {
                smallest++;
            }
            if (items[smallest]->val < x->val) {
                items[curr] = items[smallest];
                curr = smallest;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    NodeMinHeap() {
        size = 0;
    }

    bool insert(Node* item) {
        if (size < MAX - 1) {
            items[++size] = item;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    Node* getMin() {
        if (size == 0) {
            return nullptr;
        }
        return items[1];
    }

    Node* extractMin() {
        if (size == 0) {
            return nullptr;
        }
        Node* top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return top;
    }

    bool remove() {
        if (size == 0) {
            return false;
        }
        extractMin();
        return true;
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
            cout << items[i]->val << " ";
        }
        cout << endl;
    }
};

// Part A: Merge k sorted arrays
void mergeKSortedArrays(int arrays[MAX_K][MAX_COLS], int sizes[], int k,
                        int merged[], int& mergedSize) {
    ArrayMinHeap heap;

    for (int i = 0; i < k; i++) {
        if (sizes[i] > 0) {
            heap.insert({arrays[i][0], i, 0});
        }
    }

    mergedSize = 0;
    while (!heap.isEmpty()) {
        ArrayEntry smallest = heap.extractMin();
        merged[mergedSize++] = smallest.val;

        int nextIdx = smallest.elemIdx + 1;
        if (nextIdx < sizes[smallest.arrIdx]) {
            heap.insert({arrays[smallest.arrIdx][nextIdx], smallest.arrIdx, nextIdx});
        }
    }
}

// Part B: Merge k sorted linked chains
Node* mergeKSortedLists(Node* lists[], int k) {
    NodeMinHeap heap;

    for (int i = 0; i < k; i++) {
        if (lists[i] != nullptr) {
            heap.insert(lists[i]);
        }
    }

    Node dummy(0);
    Node* tail = &dummy;

    while (!heap.isEmpty()) {
        Node* smallest = heap.extractMin();
        tail->next = smallest;
        tail = tail->next;

        if (smallest->next != nullptr) {
            heap.insert(smallest->next);
        }
    }
    tail->next = nullptr;

    return dummy.next;
}

Node* buildList(int vals[], int n) {
    Node dummy(0);
    Node* tail = &dummy;
    for (int i = 0; i < n; i++) {
        tail->next = new Node(vals[i]);
        tail = tail->next;
    }
    return dummy.next;
}

void freeList(Node* head) {
    while (head != nullptr) {
        Node* nxt = head->next;
        delete head;
        head = nxt;
    }
}

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i + 1 < n) {
            cout << ", ";
        }
    }
    cout << "]\n";
}

void printLinkedList(Node* head) {
    Node* curr = head;
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
    int arrays1[MAX_K][MAX_COLS] = {
        {1, 4, 5},
        {1, 3, 4},
        {2, 6}
    };
    int sizes1[3] = {3, 3, 2};
    int mergedArr1[MAX];
    int mergedSize1 = 0;
    mergeKSortedArrays(arrays1, sizes1, 3, mergedArr1, mergedSize1);
    cout << "Part A Test 1 (Merge K Sorted Arrays): ";
    printArray(mergedArr1, mergedSize1);

    int arrays2[MAX_K][MAX_COLS] = {
        {0},
        {2, 2, 10},
        {-3, 0, 5, 9}
    };
    int sizes2[3] = {0, 3, 4};
    int mergedArr2[MAX];
    int mergedSize2 = 0;
    mergeKSortedArrays(arrays2, sizes2, 3, mergedArr2, mergedSize2);
    cout << "Part A Test 2 (With empty array and negatives): ";
    printArray(mergedArr2, mergedSize2);

    // Part B tests
    int l1[3] = {1, 4, 5};
    int l2[3] = {1, 3, 4};
    int l3[2] = {2, 6};
    Node* lists1[3] = {buildList(l1, 3), buildList(l2, 3), buildList(l3, 2)};
    Node* mergedList1 = mergeKSortedLists(lists1, 3);
    cout << "Part B Test 1 (Merge K Sorted Linked Lists): ";
    printLinkedList(mergedList1);
    freeList(mergedList1);

    int l4[3] = {-10, -2, 7};
    int l5[3] = {-5, 0, 12};
    Node* lists2[3] = {nullptr, buildList(l4, 3), buildList(l5, 3)};
    Node* mergedList2 = mergeKSortedLists(lists2, 3);
    cout << "Part B Test 2 (With nullptr head and negatives): ";
    printLinkedList(mergedList2);
    freeList(mergedList2);

    return 0;
}
