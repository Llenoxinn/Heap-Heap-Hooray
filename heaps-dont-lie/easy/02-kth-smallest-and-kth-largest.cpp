#include <climits>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 1-indexed MaxHeap class built from scratch.
class MaxHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr) {
        int x = items[curr];
        while (curr / 2 > 0 && x > items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        int x = items[curr];
        while (curr * 2 <= size) {
            int child = curr * 2;
            if (child + 1 <= size && items[child + 1] > items[child]) {
                child++;
            }
            if (items[child] > x) {
                items[curr] = items[child];
                curr = child;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        items.push_back(0); // sentinel at index 0 for 1-based indexing
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

    int getMax() {
        if (size == 0) return INT_MIN;
        return items[1];
    }

    int extractMax() {
        if (size == 0) return INT_MIN;
        int maxVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) maxHeapify(1);
        return maxVal;
    }

    bool isEmpty() { return size == 0; }
    int getSize() { return size; }
};

// 1-indexed MinHeap class built from scratch.
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
        items.push_back(0); // sentinel at index 0 for 1-based indexing
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
        if (size == 0) return INT_MAX;
        return items[1];
    }

    int extractMin() {
        if (size == 0) return INT_MAX;
        int minVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) minHeapify(1);
        return minVal;
    }

    bool isEmpty() { return size == 0; }
    int getSize() { return size; }
};

// K'th Smallest using a MaxHeap of size k:
// keeps the k smallest elements seen so far; root is the k'th smallest.
int kthSmallest(vector<int>& arr, int k) {
    int n = (int)arr.size();
    if (k < 1 || k > n) return INT_MIN;

    MaxHeap maxH;
    for (int i = 0; i < n; i++) {
        if (maxH.getSize() < k) {
            maxH.insert(arr[i]);
        } else if (arr[i] < maxH.getMax()) {
            maxH.extractMax();
            maxH.insert(arr[i]);
        }
    }
    return maxH.getMax();
}

// K'th Largest using a MinHeap of size k:
// keeps the k largest elements seen so far; root is the k'th largest.
int kthLargest(vector<int>& arr, int k) {
    int n = (int)arr.size();
    if (k < 1 || k > n) return INT_MIN;

    MinHeap minH;
    for (int i = 0; i < n; i++) {
        if (minH.getSize() < k) {
            minH.insert(arr[i]);
        } else if (arr[i] > minH.getMin()) {
            minH.extractMin();
            minH.insert(arr[i]);
        }
    }
    return minH.getMin();
}

void printArray(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) cout << ", ";
    }
    cout << "]";
}

void runTest(string label, vector<int> arr, int k) {
    cout << label << " (k = " << k << ")\n";
    cout << "  Array        : ";
    printArray(arr);
    cout << "\n";

    int small = kthSmallest(arr, k);
    int large = kthLargest(arr, k);

    if (small == INT_MIN) {
        cout << "  Result       : Invalid k (out of bounds)\n\n";
    } else {
        cout << "  K'th Smallest: " << small << "\n";
        cout << "  K'th Largest : " << large << "\n\n";
    }
}

int main() {
    runTest("Test 1 (Classic Unsorted Array)", {7, 10, 4, 3, 20, 15}, 3);
    runTest("Test 2 (LeetCode 215 Example)", {3, 2, 1, 5, 6, 4}, 2);
    runTest("Test 3 (With Duplicates)", {3, 2, 3, 1, 2, 4, 5, 5, 6}, 4);
    runTest("Test 4 (k = 1 Extremes)", {12, 5, 787, 1, 23}, 1);
    runTest("Test 5 (k = n Extremes)", {12, 5, 787, 1, 23}, 5);
    runTest("Test 6 (Invalid k)", {4, 1, 9}, 0);

    return 0;
}
