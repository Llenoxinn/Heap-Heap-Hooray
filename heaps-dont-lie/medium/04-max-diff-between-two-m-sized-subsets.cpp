#include <iostream>
#include <vector>
#include <algorithm>
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
            int s = curr * 2;
            if (s + 1 <= size && items[s] > items[s + 1]) {
                s++;
            }
            if (items[s] < x) {
                items[curr] = items[s];
                curr = s;
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
        items.push_back(x);
        size++;
        heapifyUp(size);
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
        items.pop_back();
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
            int s = curr * 2;
            if (s + 1 <= size && items[s] < items[s + 1]) {
                s++;
            }
            if (items[s] > x) {
                items[curr] = items[s];
                curr = s;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        items.push_back(0);
        size = 0;
    }

    void insert(int x) {
        items.push_back(x);
        size++;
        heapifyUp(size);
    }

    int getMax() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    int extractMax() {
        if (size == 0) {
            return -1;
        }
        int maxVal = items[1];
        items[1] = items[size];
        items.pop_back();
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return maxVal;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

long long maxDiffBetweenTwoMSizedSubsets(vector<int>& arr, int m) {
    int n = (int)arr.size();
    if (m <= 0 || m > n) {
        return 0;
    }

    MinHeap largestM;
    MaxHeap smallestM;

    for (int i = 0; i < n; i++) {
        if (largestM.getSize() < m) {
            largestM.insert(arr[i]);
        } else if (arr[i] > largestM.getMin()) {
            largestM.extractMin();
            largestM.insert(arr[i]);
        }

        if (smallestM.getSize() < m) {
            smallestM.insert(arr[i]);
        } else if (arr[i] < smallestM.getMax()) {
            smallestM.extractMax();
            smallestM.insert(arr[i]);
        }
    }

    long long maxSum = 0;
    while (!largestM.isEmpty()) {
        maxSum += largestM.extractMin();
    }

    long long minSum = 0;
    while (!smallestM.isEmpty()) {
        minSum += smallestM.extractMax();
    }

    return maxSum - minSum;
}

void printArray(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(string label, vector<int> arr, int m) {
    cout << label << " (m = " << m << ")\n";
    cout << "  Input   : ";
    printArray(arr);
    cout << "\n  Max Diff: " << maxDiffBetweenTwoMSizedSubsets(arr, m) << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Example)", {1, 2, 3, 4, 5}, 2);
    runTest("Test 2 (Unsorted Array)", {5, 8, 11, 40, 15}, 2);
    runTest("Test 3 (Negatives & m = 1)", {10, -4, 7, 2}, 1);
    runTest("Test 4 (Edge Case m = n)", {3, 1, 4}, 3);
    runTest("Test 5 (All Equal)", {5, 5, 5, 5}, 2);
    return 0;
}
