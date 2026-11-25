#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

struct Candidate {
    int diff;
    int val;
};

class MaxHeap {
private:
    vector<Candidate> items;
    int size;

    bool isWorse(Candidate& a, Candidate& b) {
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
        Candidate dummy;
        dummy.diff = 0;
        dummy.val = 0;
        items.push_back(dummy);
        size = 0;
    }

    void insert(Candidate x) {
        items.push_back(x);
        size++;
        heapifyUp(size);
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

vector<int> findKClosestElements(vector<int>& arr, int k, int x) {
    vector<int> result;
    if (k <= 0 || arr.empty()) {
        return result;
    }

    int limit = min(k, (int)arr.size());
    MaxHeap maxHeap;

    for (int i = 0; i < (int)arr.size(); i++) {
        Candidate cand;
        cand.diff = abs(arr[i] - x);
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

    while (!maxHeap.isEmpty()) {
        result.push_back(maxHeap.extractMax().val);
    }
    sort(result.begin(), result.end());
    return result;
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

void runTest(string label, vector<int> arr, int k, int x) {
    cout << label << " (k = " << k << ", x = " << x << ")\n";
    cout << "  Input  : ";
    printArray(arr);
    cout << "\n";
    vector<int> closest = findKClosestElements(arr, k, x);
    cout << "  Closest: ";
    printArray(closest);
    cout << "\n\n";
}

int main() {
    runTest("Test 1 (Primary Unsorted Example)", {10, 2, 14, 4, 7, 6}, 3, 5);
    runTest("Test 2 (Sorted Array With Ties)", {1, 2, 3, 4, 5}, 4, 3);
    runTest("Test 3 (Target Outside Range)", {12, 16, 22, 30, 39, 42, 45}, 3, 50);
    runTest("Test 4 (Negatives & Ties)", {-10, -4, 0, 4, 8, 12}, 3, 2);
    return 0;
}
