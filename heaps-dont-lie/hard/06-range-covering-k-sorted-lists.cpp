#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct ListEntry {
    int val;
    int listIdx;
    int elemIdx;
};

class MinHeap {
private:
    vector<ListEntry> items;
    int size;

    void heapifyUp(int curr) {
        ListEntry x = items[curr];
        while (curr / 2 > 0 && x.val < items[curr / 2].val) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        ListEntry x = items[curr];
        while (curr * 2 <= size) {
            int smallest = curr * 2;
            if (smallest + 1 <= size && items[smallest + 1].val < items[smallest].val) {
                smallest++;
            }
            if (items[smallest].val < x.val) {
                items[curr] = items[smallest];
                curr = smallest;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MinHeap() {
        items.push_back({0, 0, 0});
        size = 0;
    }

    bool insert(ListEntry entry) {
        if (size + 1 < (int)items.size()) {
            items[++size] = entry;
        } else {
            items.push_back(entry);
            size++;
        }
        heapifyUp(size);
        return true;
    }

    ListEntry getMin() {
        if (size == 0) {
            return {0, -1, -1};
        }
        return items[1];
    }

    ListEntry extractMin() {
        if (size == 0) {
            return {0, -1, -1};
        }
        ListEntry top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
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

vector<int> smallestRangeCoveringKLists(vector<vector<int>>& lists) {
    if (lists.empty()) {
        return {};
    }

    MinHeap heap;
    int currMax = lists[0][0];

    for (int i = 0; i < (int)lists.size(); i++) {
        if (lists[i].empty()) {
            return {};
        }
        int firstVal = lists[i][0];
        heap.insert({firstVal, i, 0});
        currMax = max(currMax, firstVal);
    }

    int bestStart = heap.getMin().val;
    int bestEnd = currMax;

    while (true) {
        ListEntry minEntry = heap.extractMin();
        int currStart = minEntry.val;

        if ((long long)currMax - currStart < (long long)bestEnd - bestStart ||
            ((long long)currMax - currStart == (long long)bestEnd - bestStart && currStart < bestStart)) {
            bestStart = currStart;
            bestEnd = currMax;
        }

        int nextIdx = minEntry.elemIdx + 1;
        if (nextIdx >= (int)lists[minEntry.listIdx].size()) {
            break; // One list exhausted; no further window covers all k lists
        }

        int nextVal = lists[minEntry.listIdx][nextIdx];
        heap.insert({nextVal, minEntry.listIdx, nextIdx});
        currMax = max(currMax, nextVal);
    }

    return {bestStart, bestEnd};
}

int main() {
    vector<vector<int>> lists1 = {
        {4, 10, 15, 24, 26},
        {0, 9, 12, 20},
        {5, 18, 22, 30}
    };
    vector<int> ans1 = smallestRangeCoveringKLists(lists1);
    cout << "Test 1 (Classic 3 lists): [" << ans1[0] << ", " << ans1[1] << "]\n";

    vector<vector<int>> lists2 = {
        {1, 2, 3},
        {1, 2, 3},
        {1, 2, 3}
    };
    vector<int> ans2 = smallestRangeCoveringKLists(lists2);
    cout << "Test 2 (Identical lists): [" << ans2[0] << ", " << ans2[1] << "]\n";

    vector<vector<int>> lists3 = {
        {-5, -1, 4},
        {-3, 2, 10},
        {0, 5, 8}
    };
    vector<int> ans3 = smallestRangeCoveringKLists(lists3);
    cout << "Test 3 (Lists with negative numbers): [" << ans3[0] << ", " << ans3[1] << "]\n";

    return 0;
}
