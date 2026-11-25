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
            int smallest = curr * 2;
            if (smallest + 1 <= size && items[smallest + 1] < items[smallest]) {
                smallest++;
            }
            if (items[smallest] < x) {
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
        size = 0;
    }

    bool insert(int val) {
        if (size < MAX - 1) {
            items[++size] = val;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    int getMin() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    int extractMin() {
        if (size == 0) {
            return -1;
        }
        int minVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return minVal;
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
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

int kthLargestSumSubarray(int arr[], int n, int k) {
    int prefix[MAX];
    prefix[0] = 0;
    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + arr[i];
    }

    MinHeap topK;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j <= n; j++) {
            int subSum = prefix[j] - prefix[i];
            if (topK.getSize() < k) {
                topK.insert(subSum);
            } else if (subSum > topK.getMin()) {
                topK.extractMin();
                topK.insert(subSum);
            }
        }
    }

    return topK.getMin();
}

int main() {
    int arr1[3] = {20, -5, -1};
    int k1 = 3;
    cout << "Test 1 ([20, -5, -1], k=3): "
         << kthLargestSumSubarray(arr1, 3, k1) << "\n";

    int arr2[4] = {10, -10, 20, -40};
    int k2 = 6;
    cout << "Test 2 ([10, -10, 20, -40], k=6): "
         << kthLargestSumSubarray(arr2, 4, k2) << "\n";

    int arr3[3] = {-3, -1, -4};
    int k3 = 2;
    cout << "Test 3 ([-3, -1, -4], k=2): "
         << kthLargestSumSubarray(arr3, 3, k3) << "\n";

    return 0;
}
