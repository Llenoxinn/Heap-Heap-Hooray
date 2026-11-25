#include <climits>
#include <iostream>
#include <vector>
using namespace std;

class MaxHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr);
    void maxHeapify(int curr);

public:
    MaxHeap();
    void insert(int x);
    int getMax();
    int extractMax();
    bool isEmpty();
    int getSize();
};

MaxHeap::MaxHeap() {
    items.push_back(0);
    size = 0;
}

void MaxHeap::heapifyUp(int curr) {
    int x = items[curr];
    while (curr / 2 > 0 && x > items[curr / 2]) {
        items[curr] = items[curr / 2];
        curr = curr / 2;
    }
    items[curr] = x;
}

void MaxHeap::maxHeapify(int curr) {
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

void MaxHeap::insert(int x) {
    items.push_back(x);
    size++;
    heapifyUp(size);
}

int MaxHeap::getMax() {
    if (size == 0) {
        return INT_MIN;
    }
    return items[1];
}

int MaxHeap::extractMax() {
    if (size == 0) {
        return INT_MIN;
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

bool MaxHeap::isEmpty() {
    return size == 0;
}

int MaxHeap::getSize() {
    return size;
}

vector<int> largestDerangement(vector<int>& seq) {
    int n = (int)seq.size();
    if (n <= 1) {
        return {};
    }

    MaxHeap pq;
    for (int i = 0; i < n; i++) {
        pq.insert(seq[i]);
    }

    vector<int> res(n, 0);
    for (int i = 0; i < n; i++) {
        int d = pq.extractMax();
        if (d != seq[i] || i == n - 1) {
            res[i] = d;
        } else {
            int nextMax = pq.extractMax();
            res[i] = nextMax;
            pq.insert(d);
        }
    }

    // If the final position collided with seq[n - 1], swap the last two elements
    if (res[n - 1] == seq[n - 1]) {
        int temp = res[n - 1];
        res[n - 1] = res[n - 2];
        res[n - 2] = temp;
    }

    return res;
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(string label, vector<int> seq) {
    cout << "=== " << label << " ===\n";
    cout << "Input:  ";
    printVector(seq);
    cout << "\n";

    vector<int> ans = largestDerangement(seq);
    if (ans.empty()) {
        cout << "Output: No derangement possible\n\n";
        return;
    }

    cout << "Output: ";
    printVector(ans);
    bool valid = true;
    for (int i = 0; i < (int)seq.size(); i++) {
        if (ans[i] == seq[i]) {
            valid = false;
        }
    }
    cout << " (valid derangement: " << (valid ? "YES" : "NO") << ")\n\n";
}

int main() {
    runTest("Test Case 1: Reverse-sorted sequence (triggers last-two swap)", {5, 4, 3, 2, 1});
    runTest("Test Case 2: Unsorted distinct sequence", {56, 21, 42, 67, 23, 74});
    runTest("Test Case 3: Smallest valid sequence", {1, 2});
    runTest("Test Case 4: Single element (impossible)", {9});
    return 0;
}
