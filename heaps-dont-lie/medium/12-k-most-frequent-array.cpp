#include <iostream>
#define MAX 100
using namespace std;

struct FreqNode {
    int val;
    int freq;
};

bool hasLowerPriority(FreqNode a, FreqNode b) {
    if (a.freq != b.freq) {
        return a.freq < b.freq;
    }
    return a.val < b.val;
}

class MinHeap {
private:
    FreqNode items[MAX];
    int size;

    void heapifyUp(int curr) {
        FreqNode x = items[curr];
        while (curr / 2 > 0 && hasLowerPriority(x, items[curr / 2])) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        FreqNode x = items[curr];
        while (curr * 2 <= size) {
            int child = curr * 2;
            if (child + 1 <= size && hasLowerPriority(items[child + 1], items[child])) {
                child++;
            }
            if (hasLowerPriority(items[child], x)) {
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

    bool insert(FreqNode node) {
        if (size < MAX - 1) {
            items[++size] = node;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    FreqNode getMin() {
        if (size == 0) {
            FreqNode empty = {0, 0};
            return empty;
        }
        return items[1];
    }

    FreqNode extractMin() {
        if (size == 0) {
            FreqNode empty = {0, 0};
            return empty;
        }
        FreqNode topNode = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return topNode;
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
            cout << "(" << items[i].val << ":" << items[i].freq << ") ";
        }
        cout << endl;
    }
};

void reverseIntArray(int arr[], int n) {
    int left = 0;
    int right = n - 1;
    while (left < right) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

int kMostFrequent(int nums[], int n, int k, int result[]) {
    if (k <= 0 || n <= 0) return 0;

    FreqNode counts[MAX];
    int uniqueCount = 0;

    for (int i = 0; i < n; i++) {
        int foundIdx = -1;
        for (int j = 0; j < uniqueCount; j++) {
            if (counts[j].val == nums[i]) {
                foundIdx = j;
                break;
            }
        }
        if (foundIdx != -1) {
            counts[foundIdx].freq++;
        } else if (uniqueCount < MAX) {
            counts[uniqueCount].val = nums[i];
            counts[uniqueCount].freq = 1;
            uniqueCount++;
        }
    }

    MinHeap minHeap;
    for (int i = 0; i < uniqueCount; i++) {
        FreqNode node = counts[i];
        if (minHeap.getSize() < k) {
            minHeap.insert(node);
        } else if (hasLowerPriority(minHeap.getMin(), node)) {
            minHeap.extractMin();
            minHeap.insert(node);
        }
    }

    int resCount = 0;
    while (!minHeap.isEmpty() && resCount < MAX) {
        result[resCount++] = minHeap.extractMin().val;
    }
    reverseIntArray(result, resCount);
    return resCount;
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
    int arr1[MAX] = {3, 1, 4, 4, 5, 2, 6, 1};
    int n1 = 8;
    int k1 = 2;
    int res1[MAX];
    int resLen1 = kMostFrequent(arr1, n1, k1, res1);
    cout << "Test 1 (k = " << k1 << "): ";
    printArray(res1, resLen1);
    cout << "\n";

    int arr2[MAX] = {7, 10, 11, 5, 2, 5, 5, 7, 11, 8, 9};
    int n2 = 11;
    int k2 = 4;
    int res2[MAX];
    int resLen2 = kMostFrequent(arr2, n2, k2, res2);
    cout << "Test 2 (k = " << k2 << "): ";
    printArray(res2, resLen2);
    cout << "\n";

    return 0;
}
