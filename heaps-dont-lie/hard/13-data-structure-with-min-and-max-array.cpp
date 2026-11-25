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
