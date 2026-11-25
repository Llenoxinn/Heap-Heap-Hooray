#include <iostream>
#define MAX 100
using namespace std;

struct SumNode {
    int sum;
    int i;
    int j;
};

struct IndexPair {
    int i;
    int j;
};

class MaxHeap {
private:
    SumNode items[MAX];
    int size;

    void heapifyUp(int curr) {
        SumNode x = items[curr];
        while (curr / 2 > 0 && x.sum > items[curr / 2].sum) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        SumNode x = items[curr];
        while (curr * 2 <= size) {
            int child = curr * 2;
            if (child + 1 <= size && items[child + 1].sum > items[child].sum) {
                child++;
            }
            if (items[child].sum > x.sum) {
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
        size = 0;
    }

    bool insert(SumNode node) {
        if (size < MAX - 1) {
            items[++size] = node;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    SumNode getMax() {
        if (size == 0) {
            SumNode empty = {0, -1, -1};
            return empty;
        }
        return items[1];
    }

    SumNode extractMax() {
        if (size == 0) {
            SumNode empty = {0, -1, -1};
            return empty;
        }
        SumNode topNode = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
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
            cout << items[i].sum << " ";
        }
        cout << endl;
    }
};

void sortDescending(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] < key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

bool wasVisited(IndexPair visited[], int visitedCount, int i, int j) {
    for (int idx = 0; idx < visitedCount; idx++) {
        if (visited[idx].i == i && visited[idx].j == j) {
            return true;
        }
    }
    return false;
}

int kMaxSumCombinations(int a[], int n, int b[], int m, int k, int result[]) {
    if (n <= 0 || m <= 0 || k <= 0) return 0;

    sortDescending(a, n);
    sortDescending(b, m);

    MaxHeap heap;
    IndexPair visited[MAX];
    int visitedCount = 0;

    SumNode first = {a[0] + b[0], 0, 0};
    heap.insert(first);
    visited[visitedCount++] = {0, 0};

    int resCount = 0;
    while (!heap.isEmpty() && resCount < k && resCount < MAX) {
        SumNode top = heap.extractMax();
        result[resCount++] = top.sum;

        int i = top.i;
        int j = top.j;

        if (i + 1 < n && !wasVisited(visited, visitedCount, i + 1, j)) {
            SumNode nextDown = {a[i + 1] + b[j], i + 1, j};
            heap.insert(nextDown);
            if (visitedCount < MAX) {
                visited[visitedCount++] = {i + 1, j};
            }
        }
        if (j + 1 < m && !wasVisited(visited, visitedCount, i, j + 1)) {
            SumNode nextRight = {a[i] + b[j + 1], i, j + 1};
            heap.insert(nextRight);
            if (visitedCount < MAX) {
                visited[visitedCount++] = {i, j + 1};
            }
        }
    }

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
    int a1[MAX] = {4, 2, 5, 1};
    int b1[MAX] = {8, 0, 3, 5};
    int k1 = 3;
    int res1[MAX];
    int resLen1 = kMaxSumCombinations(a1, 4, b1, 4, k1, res1);
    cout << "Test 1 (K = " << k1 << "): ";
    printArray(res1, resLen1);
    cout << "\n";

    int a2[MAX] = {1, 4, 2, 3};
    int b2[MAX] = {2, 5, 1, 6};
    int k2 = 4;
    int res2[MAX];
    int resLen2 = kMaxSumCombinations(a2, 4, b2, 4, k2, res2);
    cout << "Test 2 (K = " << k2 << "): ";
    printArray(res2, resLen2);
    cout << "\n";

    return 0;
}
