#include <iostream>
#define MAX 10
using namespace std;

class Heap {
private:
    int items[MAX];
    int size;
public:
    Heap();
    bool insert(int);
    bool remove();
    void display() {
        cout<<"size :"<<size<<endl;
        for(int i = 1; i<=size;i++) {
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

int main() {

    Heap h;
    for(int i = 103; i>=0; i-=10) {
        h.insert(i);
        h.display();
    }
    h.remove();
    h.display();
    return 0;
}

Heap::Heap() {
    size = 0;
}

bool Heap::insert(int x) {
    if(size<MAX-1) {
        int curr=++size;
        while(curr/2>0 && x<items[curr/2]) {
            items[curr]=items[curr/2];
            curr=curr/2;
        }
        items[curr]=x;
        return true;
    }
    return false;
}

bool Heap::remove() {
    int last = items[size--];
    int curr = 1;
    while(curr*2<=size) {
        int s = curr*2;
        if(s+1<=size&&items[s] > items[s+1]) s++;
        if(items[s] < last) {
            items[curr]=items[s];
            curr=s;
        } else break;
    }
    items[curr]=last;
    return true;
}