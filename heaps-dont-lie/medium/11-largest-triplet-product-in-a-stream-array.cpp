#include <iostream>
#define MAX 100
using namespace std;

class MinHeap {
private:
    int items[MAX];
    int size;

    void heapifyUp(int curr) {
        int x = items[curr];
        while (curr / 2 > 0 && x < items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        int x = items[curr];
        while (curr * 2 <= size) {
            int child = curr * 2;
            if (child + 1 <= size && items[child + 1] < items[child]) {
                child++;
            }
            if (items[child] < x) {
                items[curr] = items[child];
                curr = child;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MinHeap() {
        size = 0;
    }

    bool insert(int x) {
        if (size < MAX - 1) {
            items[++size] = x;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    int getMin() {
        if (size == 0) return -1;
        return items[1];
    }

    int extractMin() {
        if (size == 0) return -1;
        int minVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return minVal;
    }

    int keyAt(int idx) {
        return items[idx];
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
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

void largestTripletProductStream(int stream[], int n, long long result[]) {
    MinHeap topThree;

    for (int i = 0; i < n; i++) {
        int val = stream[i];
        if (topThree.getSize() < 3) {
            topThree.insert(val);
        } else if (val > topThree.getMin()) {
            topThree.extractMin();
            topThree.insert(val);
        }

        if (topThree.getSize() < 3) {
            result[i] = -1;
        } else {
            long long prod = 1LL * topThree.keyAt(1) * topThree.keyAt(2) * topThree.keyAt(3);
            result[i] = prod;
        }
    }
}

void printArray(long long arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    int s1[MAX] = {1, 2, 3, 4, 5};
    int n1 = 5;
    long long res1[MAX];
    largestTripletProductStream(s1, n1, res1);
    cout << "Test 1: ";
    printArray(res1, n1);
    cout << "\n";

    int s2[MAX] = {10, 3, 5, 6, 20};
    int n2 = 5;
    long long res2[MAX];
    largestTripletProductStream(s2, n2, res2);
    cout << "Test 2: ";
    printArray(res2, n2);
    cout << "\n";

    return 0;
}
