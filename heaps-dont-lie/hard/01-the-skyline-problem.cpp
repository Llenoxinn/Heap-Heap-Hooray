#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

struct Event {
    int x;
    int height;
    int type; // 0 = start (left edge), 1 = end (right edge)
};

bool compareEvents(Event a, Event b) {
    if (a.x != b.x) {
        return a.x < b.x;
    }
    if (a.type != b.type) {
        return a.type < b.type; // Process building starts before ends at same x
    }
    if (a.type == 0) {
        return a.height > b.height; // Taller start first
    }
    return a.height < b.height; // Shorter end first
}

class MaxHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr) {
        int x = items[curr];
        while (curr / 2 > 0 && x > items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        int x = items[curr];
        while (curr * 2 <= size) {
            int largest = curr * 2;
            if (largest + 1 <= size && items[largest + 1] > items[largest]) {
                largest++;
            }
            if (items[largest] > x) {
                items[curr] = items[largest];
                curr = largest;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        items.push_back(0); // 1-based indexing sentinel at index 0
        size = 0;
    }

    bool insert(int val) {
        if (size + 1 < (int)items.size()) {
            items[++size] = val;
        } else {
            items.push_back(val);
            size++;
        }
        heapifyUp(size);
        return true;
    }

    int getMax() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    int extractMax() {
        if (size == 0) {
            return -1;
        }
        int maxVal = items[1];
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
};

vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
    vector<Event> events;
    for (int i = 0; i < (int)buildings.size(); i++) {
        events.push_back({buildings[i][0], buildings[i][2], 0});
        events.push_back({buildings[i][1], buildings[i][2], 1});
    }

    sort(events.begin(), events.end(), compareEvents);

    MaxHeap activeHeights;
    activeHeights.insert(0); // Ground level always active
    unordered_map<int, int> delayedRemoval;

    vector<vector<int>> skyline;
    int prevMax = 0;

    for (int i = 0; i < (int)events.size(); i++) {
        Event e = events[i];
        if (e.type == 0) {
            activeHeights.insert(e.height);
        } else {
            delayedRemoval[e.height]++;
        }

        // Lazy deletion: discard removed heights sitting at the heap root
        while (!activeHeights.isEmpty() && delayedRemoval[activeHeights.getMax()] > 0) {
            int topVal = activeHeights.extractMax();
            delayedRemoval[topVal]--;
        }

        int currMax = activeHeights.getMax();
        if (currMax != prevMax) {
            skyline.push_back({e.x, currMax});
            prevMax = currMax;
        }
    }

    return skyline;
}

void printSkyline(vector<vector<int>>& pts) {
    cout << "[";
    for (int i = 0; i < (int)pts.size(); i++) {
        cout << "[" << pts[i][0] << ", " << pts[i][1] << "]";
        if (i + 1 < (int)pts.size()) {
            cout << ", ";
        }
    }
    cout << "]\n";
}

int main() {
    vector<vector<int>> buildings1 = {
        {2, 9, 10},
        {3, 7, 15},
        {5, 12, 12},
        {15, 20, 10},
        {19, 24, 8}
    };
    vector<vector<int>> ans1 = getSkyline(buildings1);
    cout << "Test 1 (Classic 5-building skyline): ";
    printSkyline(ans1);

    vector<vector<int>> buildings2 = {
        {0, 2, 3},
        {2, 5, 3}
    };
    vector<vector<int>> ans2 = getSkyline(buildings2);
    cout << "Test 2 (Touching equal-height buildings): ";
    printSkyline(ans2);

    vector<vector<int>> buildings3 = {
        {1, 5, 10},
        {1, 3, 15},
        {3, 5, 10}
    };
    vector<vector<int>> ans3 = getSkyline(buildings3);
    cout << "Test 3 (Shared left & right edges): ";
    printSkyline(ans3);

    return 0;
}
