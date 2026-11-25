#include <iostream>
#define MAX 10
using namespace std;

class Heap {
private:
    int items[MAX];
    int size;

    void heapifyUp(int curr);
    void minHeapify(int curr);
public:
    Heap();
    bool insert(int);
    int getMin();
    int extractMin();
    bool remove();
    bool decreaseKey(int, int);
    bool isEmpty();
    int  getSize();
    int keyAt(int);
    void display(){
        cout << "size: " << size << endl;
        for(int i=1; i<=size; i++) {
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

Heap::Heap() {
    size=0;
}

bool Heap::insert(int x) {
    if(size<MAX-1)
        items[++size]==x;
        heapifyUp(size);
        return true;
    
    return false;
}

void Heap::heapifyUp(int curr) {
    int x = items[curr];
    while(curr/2>0 && x<items[curr/2]) {
        items[curr]=items[curr/2];
        curr=curr/2;
    }
    items[curr]=x;
}

void Heap::minHeapify(int curr){
    int x = items[curr];
    while(curr*2<=size) {
        int s = curr*2;
        if(s+1<=size && items[s] > items[s+1]) {
            s++;
        }
        if(items[s]<x) {
            items[curr]=items[s];
            curr=s;
        } else break;
    }
    items[curr]=x;
}

// int Heap::getMin() {
//     if (size == 0) return EMPTY;
//     return items[1];
// }

// int Heap::extractMin() {
//     if (size == 0) return EMPTY;
//     int minVal = items[1];
//     items[1] = items[size];       // last element goes to the root
//     size--;
//     if (size > 0) minHeapify(1);
//     return minVal;
// }

bool Heap::remove() {
    if (size == 0) return false;
    extractMin();
    return true;
}

bool Heap::decreaseKey(int i, int newVal) {
    if (i < 1 || i > size || newVal > items[i]) return false;
    items[i] = newVal;
    heapifyUp(i);
    return true;
}

bool Heap::isEmpty() { return size == 0; }
int  Heap::getSize() { return size; }
int  Heap::keyAt(int i) { return items[i]; }

int main() {
    Heap h;
    for (int i = 103; i >= 0; i -= 10) {
        if (!h.insert(i)) cout << "insert(" << i << ") failed: heap is full\n";
        h.display();
    }

    int m = h.getMin();
    cout << "\ngetMin: " << m << "\n";

    h.decreaseKey(9, 1);
    cout << "after decreaseKey(9, 1):\n";
    h.display();

    m = h.extractMin();
    cout << "extractMin gave " << m << ":\n";
    h.display();

    h.remove();
    cout << "after remove():\n";
    h.display();

    cout << "draining:";
    while (!h.isEmpty()) cout << " " << h.extractMin();

    // cout << "\nextractMin() on an empty heap returns EMPTY? " << (h.extractMin() == EMPTY ? "yes" : "no");
    // cout << "\nremove() on an empty heap returns " << (h.remove() ? "true" : "false") << "\n";
    return 0;
}