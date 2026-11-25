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

    void collectLessThan(int curr, int x, vector<int>& out) {
        if (curr > size) {
            return;
        }
        if (items[curr] >= x) {
            return;
        }
        out.push_back(items[curr]);
        collectLessThan(2 * curr, x, out);
        collectLessThan(2 * curr + 1, x, out);
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

    vector<int> getNodesLessThan(int x) {
        vector<int> out;
        collectLessThan(1, x, out);
        return out;
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

void runQuery(MinHeap& heap, string label, int x) {
    vector<int> ans = heap.getNodesLessThan(x);
    cout << label << " (x = " << x << ") -> ";
    printArray(ans);
    cout << "\n";
}

int main() {
    MinHeap heap;
    vector<int> values = {2, 3, 15, 5, 4, 45, 80, 6, 150, 77, 120};
    for (int i = 0; i < (int)values.size(); i++) {
        heap.insert(values[i]);
    }

    cout << "Min-Heap array (1-indexed level order): ";
    heap.displayHeapArray();
    cout << "\n\n";

    runQuery(heap, "Test 1 (Primary Example)", 10);
    runQuery(heap, "Test 2 (Root Pruned Immediately)", 2);
    runQuery(heap, "Test 3 (Threshold 50)", 50);
    runQuery(heap, "Test 4 (Threshold 200 - All Nodes)", 200);

    return 0;
}
