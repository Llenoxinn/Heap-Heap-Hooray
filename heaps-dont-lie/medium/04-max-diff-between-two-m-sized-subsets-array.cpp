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
        items[0] = 0;
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

class MaxHeap {
private:
    int items[MAX];
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
        items[0] = 0;
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

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

long long maxDiffBetweenTwoMSizedSubsets(int arr[], int n, int m) {
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

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i + 1 < n) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(int arr[], int n, int m) {
    cout << "  Input   : ";
    printArray(arr, n);
    cout << "\n  Max Diff: " << maxDiffBetweenTwoMSizedSubsets(arr, n, m) << "\n\n";
}

int main() {
    cout << "Test 1 (Primary Example) (m = 2)\n";
    int a1[] = {1, 2, 3, 4, 5};
    runTest(a1, 5, 2);

    cout << "Test 2 (Unsorted Array) (m = 2)\n";
    int a2[] = {5, 8, 11, 40, 15};
    runTest(a2, 5, 2);

    cout << "Test 3 (Negatives & m = 1) (m = 1)\n";
    int a3[] = {10, -4, 7, 2};
    runTest(a3, 4, 1);

    cout << "Test 4 (Edge Case m = n) (m = 3)\n";
    int a4[] = {3, 1, 4};
    runTest(a4, 3, 3);

    cout << "Test 5 (All Equal) (m = 2)\n";
    int a5[] = {5, 5, 5, 5};
    runTest(a5, 4, 2);

    return 0;
}
