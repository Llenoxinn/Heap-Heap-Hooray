#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>
using namespace std;

struct SumNode {
    int sum;
    int i;
    int j;
};

class MaxHeap {
private:
    vector<SumNode> items;
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
        items.push_back({0, -1, -1});
        size = 0;
    }

    void insert(SumNode node) {
        size++;
        if (size < (int)items.size()) {
            items[size] = node;
        } else {
            items.push_back(node);
        }
        heapifyUp(size);
    }

    SumNode getMax() {
        if (size == 0) return {0, -1, -1};
        return items[1];
    }

    SumNode extractMax() {
        if (size == 0) return {0, -1, -1};
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
};

bool descendingOrder(int a, int b) {
    return a > b;
}

vector<int> kMaxSumCombinations(vector<int>& a, vector<int>& b, int k) {
    vector<int> result;
    int n = (int)a.size();
    int m = (int)b.size();
    if (n == 0 || m == 0 || k <= 0) return result;

    sort(a.begin(), a.end(), descendingOrder);
    sort(b.begin(), b.end(), descendingOrder);

    MaxHeap heap;
    set<pair<int, int>> visited;

    heap.insert({a[0] + b[0], 0, 0});
    visited.insert({0, 0});

    while (!heap.isEmpty() && (int)result.size() < k) {
        SumNode top = heap.extractMax();
        result.push_back(top.sum);

        int i = top.i;
        int j = top.j;

        if (i + 1 < n && visited.find({i + 1, j}) == visited.end()) {
            heap.insert({a[i + 1] + b[j], i + 1, j});
            visited.insert({i + 1, j});
        }
        if (j + 1 < m && visited.find({i, j + 1}) == visited.end()) {
            heap.insert({a[i] + b[j + 1], i, j + 1});
            visited.insert({i, j + 1});
        }
    }

    return result;
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    vector<int> a1 = {4, 2, 5, 1};
    vector<int> b1 = {8, 0, 3, 5};
    int k1 = 3;
    vector<int> res1 = kMaxSumCombinations(a1, b1, k1);
    cout << "Test 1 (K = " << k1 << "): ";
    printVector(res1);
    cout << "\n";

    vector<int> a2 = {1, 4, 2, 3};
    vector<int> b2 = {2, 5, 1, 6};
    int k2 = 4;
    vector<int> res2 = kMaxSumCombinations(a2, b2, k2);
    cout << "Test 2 (K = " << k2 << "): ";
    printVector(res2);
    cout << "\n";

    return 0;
}
