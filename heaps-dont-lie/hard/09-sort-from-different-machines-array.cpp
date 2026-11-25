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
