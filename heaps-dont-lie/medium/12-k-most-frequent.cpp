#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
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
    vector<FreqNode> items;
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
        items.push_back({0, 0});
        size = 0;
    }

    void insert(FreqNode node) {
        size++;
        if (size < (int)items.size()) {
            items[size] = node;
        } else {
            items.push_back(node);
        }
        heapifyUp(size);
    }

    FreqNode getMin() {
        if (size == 0) return {0, 0};
        return items[1];
    }

    FreqNode extractMin() {
        if (size == 0) return {0, 0};
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
};

vector<int> kMostFrequent(vector<int>& nums, int k) {
    vector<int> result;
    if (k <= 0 || nums.empty()) return result;

    unordered_map<int, int> counts;
    for (int i = 0; i < (int)nums.size(); i++) {
        counts[nums[i]]++;
    }

    MinHeap minHeap;
    for (auto it = counts.begin(); it != counts.end(); ++it) {
        FreqNode node = {it->first, it->second};
        if (minHeap.getSize() < k) {
            minHeap.insert(node);
        } else if (hasLowerPriority(minHeap.getMin(), node)) {
            minHeap.extractMin();
            minHeap.insert(node);
        }
    }

    while (!minHeap.isEmpty()) {
        result.push_back(minHeap.extractMin().val);
    }
    reverse(result.begin(), result.end());
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
    vector<int> arr1 = {3, 1, 4, 4, 5, 2, 6, 1};
    int k1 = 2;
    vector<int> res1 = kMostFrequent(arr1, k1);
    cout << "Test 1 (k = " << k1 << "): ";
    printVector(res1);
    cout << "\n";

    vector<int> arr2 = {7, 10, 11, 5, 2, 5, 5, 7, 11, 8, 9};
    int k2 = 4;
    vector<int> res2 = kMostFrequent(arr2, k2);
    cout << "Test 2 (k = " << k2 << "): ";
    printVector(res2);
    cout << "\n";

    return 0;
}
