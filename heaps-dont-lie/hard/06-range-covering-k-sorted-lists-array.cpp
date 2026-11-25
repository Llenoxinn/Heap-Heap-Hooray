#include <iostream>
#define MAX 100
#define MAX_K 20
#define MAX_COLS 50
using namespace std;

struct ListEntry {
    int val;
    int listIdx;
    int elemIdx;
};

struct Range {
    int start;
    int end;
    bool valid;
};

class MinHeap {
private:
    ListEntry items[MAX];
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
        size = 0;
    }

    bool insert(ListEntry entry) {
        if (size < MAX - 1) {
            items[++size] = entry;
            heapifyUp(size);
            return true;
        }
        return false;
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

    bool remove() {
        if (size == 0) {
            return false;
        }
        extractMin();
        return true;
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
            cout << items[i].val << " ";
        }
        cout << endl;
    }
};

Range smallestRangeCoveringKLists(int lists[MAX_K][MAX_COLS], int sizes[], int k) {
    if (k <= 0) {
        return {0, 0, false};
    }

    MinHeap heap;
    int currMax = lists[0][0];

    for (int i = 0; i < k; i++) {
        if (sizes[i] <= 0) {
            return {0, 0, false};
        }
        int firstVal = lists[i][0];
        heap.insert({firstVal, i, 0});
        if (firstVal > currMax) {
            currMax = firstVal;
        }
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
        if (nextIdx >= sizes[minEntry.listIdx]) {
            break; // One sequence exhausted; no further window covers all k sequences
        }

        int nextVal = lists[minEntry.listIdx][nextIdx];
        heap.insert({nextVal, minEntry.listIdx, nextIdx});
        if (nextVal > currMax) {
            currMax = nextVal;
        }
    }

    return {bestStart, bestEnd, true};
}

int main() {
    int lists1[MAX_K][MAX_COLS] = {
        {4, 10, 15, 24, 26},
        {0, 9, 12, 20},
        {5, 18, 22, 30}
    };
    int sizes1[3] = {5, 4, 4};
    Range ans1 = smallestRangeCoveringKLists(lists1, sizes1, 3);
    cout << "Test 1 (Classic 3 lists): [" << ans1.start << ", " << ans1.end << "]\n";

    int lists2[MAX_K][MAX_COLS] = {
        {1, 2, 3},
        {1, 2, 3},
        {1, 2, 3}
    };
    int sizes2[3] = {3, 3, 3};
    Range ans2 = smallestRangeCoveringKLists(lists2, sizes2, 3);
    cout << "Test 2 (Identical lists): [" << ans2.start << ", " << ans2.end << "]\n";

    int lists3[MAX_K][MAX_COLS] = {
        {-5, -1, 4},
        {-3, 2, 10},
        {0, 5, 8}
    };
    int sizes3[3] = {3, 3, 3};
    Range ans3 = smallestRangeCoveringKLists(lists3, sizes3, 3);
    cout << "Test 3 (Lists with negative numbers): [" << ans3.start << ", " << ans3.end << "]\n";

    return 0;
}
