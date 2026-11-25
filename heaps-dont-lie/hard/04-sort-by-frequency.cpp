#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

struct FreqItem {
    int val;
    int freq;
};

bool hasHigherPriority(FreqItem a, FreqItem b) {
    if (a.freq != b.freq) {
        return a.freq > b.freq; // Higher frequency first
    }
    return a.val < b.val; // Tie-breaker: smaller value first
}

class MaxHeap {
private:
    vector<FreqItem> items;
    int size;

    void heapifyUp(int curr) {
        FreqItem x = items[curr];
        while (curr / 2 > 0 && hasHigherPriority(x, items[curr / 2])) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        FreqItem x = items[curr];
        while (curr * 2 <= size) {
            int best = curr * 2;
            if (best + 1 <= size && hasHigherPriority(items[best + 1], items[best])) {
                best++;
            }
            if (hasHigherPriority(items[best], x)) {
                items[curr] = items[best];
                curr = best;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        items.push_back({0, 0});
        size = 0;
    }

    bool insert(FreqItem item) {
        if (size + 1 < (int)items.size()) {
            items[++size] = item;
        } else {
            items.push_back(item);
            size++;
        }
        heapifyUp(size);
        return true;
    }

    FreqItem getMax() {
        if (size == 0) {
            return {0, 0};
        }
        return items[1];
    }

    FreqItem extractMax() {
        if (size == 0) {
            return {0, 0};
        }
        FreqItem top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return top;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<int> sortByFrequency(vector<int>& arr) {
    unordered_map<int, int> freqMap;
    for (int i = 0; i < (int)arr.size(); i++) {
        freqMap[arr[i]]++;
    }

    MaxHeap heap;
    for (auto it = freqMap.begin(); it != freqMap.end(); it++) {
        heap.insert({it->first, it->second});
    }

    vector<int> result;
    while (!heap.isEmpty()) {
        FreqItem top = heap.extractMax();
        for (int c = 0; c < top.freq; c++) {
            result.push_back(top.val);
        }
    }

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
    cout << "]\n";
}

int main() {
    vector<int> arr1 = {5, 5, 4, 6, 4};
    vector<int> res1 = sortByFrequency(arr1);
    cout << "Test 1 ([5, 5, 4, 6, 4]): ";
    printArray(res1);

    vector<int> arr2 = {9, 9, 9, 2, 5};
    vector<int> res2 = sortByFrequency(arr2);
    cout << "Test 2 ([9, 9, 9, 2, 5]): ";
    printArray(res2);

    vector<int> arr3 = {2, 5, 2, 8, 5, 6, 8, 8};
    vector<int> res3 = sortByFrequency(arr3);
    cout << "Test 3 ([2, 5, 2, 8, 5, 6, 8, 8]): ";
    printArray(res3);

    return 0;
}
