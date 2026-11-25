#include <iostream>
#include <vector>
using namespace std;

class MinHeap {
private:
    vector<int> items;
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
        items.push_back(0);
        size = 0;
    }

    void insert(int x) {
        size++;
        if (size < (int)items.size()) {
            items[size] = x;
        } else {
            items.push_back(x);
        }
        heapifyUp(size);
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

vector<int> kthLargestInStream(vector<int>& stream, int k) {
    KthLargestStream tracker(k);
    vector<int> result;
    for (int i = 0; i < (int)stream.size(); i++) {
        result.push_back(tracker.add(stream[i]));
    }
    return result;
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    vector<int> s1 = {1, 2, 3, 4, 5, 6};
    int k1 = 4;
    vector<int> res1 = kthLargestInStream(s1, k1);
    cout << "Test 1 (k = " << k1 << "): ";
    printVector(res1);
    cout << "\n";

    vector<int> s2 = {10, 20, 11, 70, 50, 40, 100, 5};
    int k2 = 3;
    vector<int> res2 = kthLargestInStream(s2, k2);
    cout << "Test 2 (k = " << k2 << "): ";
    printVector(res2);
    cout << "\n";

    return 0;
}
