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

    void collectLessThan(int curr, int x, int out[], int& outCount) {
        if (curr > size) {
            return;
        }
        if (items[curr] >= x) {
            return;
        }
        out[outCount++] = items[curr];
        collectLessThan(2 * curr, x, out, outCount);
        collectLessThan(2 * curr + 1, x, out, outCount);
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

    int getNodesLessThan(int x, int out[]) {
        int outCount = 0;
        collectLessThan(1, x, out, outCount);
        return outCount;
    }

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << items[i] << " ";
        }
        cout << endl;
    }

    void displayHeapArray() {
        cout << "[";
        for (int i = 1; i <= size; i++) {
            cout << items[i];
            if (i < size) {
                cout << ", ";
            }
        }
        cout << "]";
    }
};

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

void runQuery(MinHeap& heap, int x) {
    int ans[MAX];
    int count = heap.getNodesLessThan(x, ans);
    cout << " (x = " << x << ") -> ";
    printArray(ans, count);
    cout << "\n";
}

int main() {
    MinHeap heap;
    int values[] = {2, 3, 15, 5, 4, 45, 80, 6, 150, 77, 120};
    for (int i = 0; i < 11; i++) {
        heap.insert(values[i]);
    }

    cout << "Min-Heap array (1-indexed level order): ";
    heap.displayHeapArray();
    cout << "\n\n";

    cout << "Test 1 (Primary Example)";
    runQuery(heap, 10);

    cout << "Test 2 (Root Pruned Immediately)";
    runQuery(heap, 2);

    cout << "Test 3 (Threshold 50)";
    runQuery(heap, 50);

    cout << "Test 4 (Threshold 200 - All Nodes)";
    runQuery(heap, 200);

    return 0;
}
