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

class KthLargestStream {
private:
    MinHeap minHeap;
    int k;

public:
    KthLargestStream(int kVal) {
        k = kVal;
    }

    int add(int val) {
        if (k <= 0) return -1;
        if (minHeap.getSize() < k) {
            minHeap.insert(val);
        } else if (val > minHeap.getMin()) {
            minHeap.extractMin();
            minHeap.insert(val);
        }

        if (minHeap.getSize() < k) {
            return -1;
        }
        return minHeap.getMin();
    }
};

void kthLargestInStream(int stream[], int n, int k, int result[]) {
    KthLargestStream tracker(k);
    for (int i = 0; i < n; i++) {
        result[i] = tracker.add(stream[i]);
    }
}

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    int s1[MAX] = {1, 2, 3, 4, 5, 6};
    int n1 = 6;
    int k1 = 4;
    int res1[MAX];
    kthLargestInStream(s1, n1, k1, res1);
    cout << "Test 1 (k = " << k1 << "): ";
    printArray(res1, n1);
    cout << "\n";

    int s2[MAX] = {10, 20, 11, 70, 50, 40, 100, 5};
    int n2 = 8;
    int k2 = 3;
    int res2[MAX];
    kthLargestInStream(s2, n2, k2, res2);
    cout << "Test 2 (k = " << k2 << "): ";
    printArray(res2, n2);
    cout << "\n";

    return 0;
}
