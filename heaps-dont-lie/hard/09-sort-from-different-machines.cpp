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
