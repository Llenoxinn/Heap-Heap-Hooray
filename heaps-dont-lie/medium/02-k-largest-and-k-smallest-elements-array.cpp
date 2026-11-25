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

void reverseArray(int arr[], int n) {
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

int findKLargest(int arr[], int n, int k, int out[]) {
    if (k <= 0 || n <= 0) {
        return 0;
    }
    int limit = (k < n) ? k : n;
    MinHeap minHeap;

    for (int i = 0; i < n; i++) {
        if (minHeap.getSize() < limit) {
            minHeap.insert(arr[i]);
        } else if (arr[i] > minHeap.getMin()) {
            minHeap.extractMin();
            minHeap.insert(arr[i]);
        }
    }

    int count = 0;
    while (!minHeap.isEmpty()) {
        out[count++] = minHeap.extractMin();
    }
    reverseArray(out, count);
    return count;
}

int findKSmallest(int arr[], int n, int k, int out[]) {
    if (k <= 0 || n <= 0) {
        return 0;
    }
    int limit = (k < n) ? k : n;
    MaxHeap maxHeap;

    for (int i = 0; i < n; i++) {
        if (maxHeap.getSize() < limit) {
            maxHeap.insert(arr[i]);
        } else if (arr[i] < maxHeap.getMax()) {
            maxHeap.extractMax();
            maxHeap.insert(arr[i]);
        }
    }

    int count = 0;
    while (!maxHeap.isEmpty()) {
        out[count++] = maxHeap.extractMax();
    }
    reverseArray(out, count);
    return count;
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

void runTest(int arr[], int n, int k) {
    cout << "  Input     : ";
    printArray(arr, n);
    cout << "\n";

    int largest[MAX];
    int smallest[MAX];
    int largestCount = findKLargest(arr, n, k, largest);
    int smallestCount = findKSmallest(arr, n, k, smallest);

    cout << "  K Largest : ";
    printArray(largest, largestCount);
    cout << "\n  K Smallest: ";
    printArray(smallest, smallestCount);
    cout << "\n\n";
}

int main() {
    cout << "Test 1 (Primary Example) (k = 3)\n";
    int a1[] = {1, 23, 12, 9, 30, 2, 50};
    runTest(a1, 7, 3);

    cout << "Test 2 (k = 4) (k = 4)\n";
    int a2[] = {7, 10, 4, 3, 20, 15};
    runTest(a2, 6, 4);

    cout << "Test 3 (Duplicates & Negatives) (k = 3)\n";
    int a3[] = {-5, 12, -5, 0, 8, 12};
    runTest(a3, 6, 3);

    cout << "Test 4 (Edge Case k = 1) (k = 1)\n";
    int a4[] = {42, 7, 19, -3, 88};
    runTest(a4, 5, 1);

    cout << "Test 5 (Edge Case k >= n) (k = 5)\n";
    int a5[] = {3, 1, 2};
    runTest(a5, 3, 5);

    return 0;
}
