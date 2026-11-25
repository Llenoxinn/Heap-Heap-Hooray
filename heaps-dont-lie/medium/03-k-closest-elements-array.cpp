#include <iostream>
#define MAX 100
using namespace std;

struct Candidate {
    int diff;
    int val;
};

int absDiff(int a, int b) {
    int d = a - b;
    return d < 0 ? -d : d;
}

class MaxHeap {
private:
    Candidate items[MAX];
    int size;

    bool isWorse(Candidate a, Candidate b) {
        if (a.diff != b.diff) {
            return a.diff > b.diff;
        }
        return a.val > b.val;
    }

    void heapifyUp(int curr) {
        Candidate x = items[curr];
        while (curr / 2 > 0 && isWorse(x, items[curr / 2])) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        Candidate x = items[curr];
        while (curr * 2 <= size) {
            int s = curr * 2;
            if (s + 1 <= size && isWorse(items[s + 1], items[s])) {
                s++;
            }
            if (isWorse(items[s], x)) {
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
        items[0].diff = 0;
        items[0].val = 0;
        size = 0;
    }

    bool insert(Candidate x) {
        if (size < MAX - 1) {
            items[++size] = x;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    Candidate getMax() {
        if (size == 0) {
            Candidate empty;
            empty.diff = 0;
            empty.val = 0;
            return empty;
        }
        return items[1];
    }

    Candidate extractMax() {
        if (size == 0) {
            Candidate empty;
            empty.diff = 0;
            empty.val = 0;
            return empty;
        }
        Candidate maxVal = items[1];
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
            cout << "(" << items[i].diff << "," << items[i].val << ") ";
        }
        cout << endl;
    }
};

void sortIntArray(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int findKClosestElements(int arr[], int n, int k, int x, int out[]) {
    if (k <= 0 || n <= 0) {
        return 0;
    }

    int limit = (k < n) ? k : n;
    MaxHeap maxHeap;

    for (int i = 0; i < n; i++) {
        Candidate cand;
        cand.diff = absDiff(arr[i], x);
        cand.val = arr[i];

        if (maxHeap.getSize() < limit) {
            maxHeap.insert(cand);
        } else {
            Candidate worst = maxHeap.getMax();
            bool candIsCloser = (cand.diff < worst.diff) ||
                                (cand.diff == worst.diff && cand.val < worst.val);
            if (candIsCloser) {
                maxHeap.extractMax();
                maxHeap.insert(cand);
            }
        }
    }

    int count = 0;
    while (!maxHeap.isEmpty()) {
        out[count++] = maxHeap.extractMax().val;
    }
    sortIntArray(out, count);
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

void runTest(int arr[], int n, int k, int x) {
    cout << "  Input  : ";
    printArray(arr, n);
    cout << "\n";
    int closest[MAX];
    int count = findKClosestElements(arr, n, k, x, closest);
    cout << "  Closest: ";
    printArray(closest, count);
    cout << "\n\n";
}

int main() {
    cout << "Test 1 (Primary Unsorted Example) (k = 3, x = 5)\n";
    int a1[] = {10, 2, 14, 4, 7, 6};
    runTest(a1, 6, 3, 5);

    cout << "Test 2 (Sorted Array With Ties) (k = 4, x = 3)\n";
    int a2[] = {1, 2, 3, 4, 5};
    runTest(a2, 5, 4, 3);

    cout << "Test 3 (Target Outside Range) (k = 3, x = 50)\n";
    int a3[] = {12, 16, 22, 30, 39, 42, 45};
    runTest(a3, 7, 3, 50);

    cout << "Test 4 (Negatives & Ties) (k = 3, x = 2)\n";
    int a4[] = {-10, -4, 0, 4, 8, 12};
    runTest(a4, 6, 3, 2);

    return 0;
}
