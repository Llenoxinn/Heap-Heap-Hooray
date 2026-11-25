#include <iostream>
#define MAX 200
using namespace std;

struct Building {
    int left;
    int right;
    int height;
};

struct Event {
    int x;
    int height;
    int type; // 0 = start (left edge), 1 = end (right edge)
    int id;
};

struct HeapItem {
    int height;
    int id;
};

struct Point {
    int x;
    int y;
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

void swapEvents(Event& a, Event& b) {
    Event temp = a;
    a = b;
    b = temp;
}

int partitionEvents(Event arr[], int low, int high) {
    Event pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (compareEvents(arr[j], pivot)) {
            i++;
            swapEvents(arr[i], arr[j]);
        }
    }
    swapEvents(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortEvents(Event arr[], int low, int high) {
    if (low < high) {
        int pi = partitionEvents(arr, low, high);
        quickSortEvents(arr, low, pi - 1);
        quickSortEvents(arr, pi + 1, high);
    }
}

class MaxHeap {
private:
    HeapItem items[MAX];
    int size;

    void heapifyUp(int curr) {
        HeapItem x = items[curr];
        while (curr / 2 > 0 && x.height > items[curr / 2].height) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        HeapItem x = items[curr];
        while (curr * 2 <= size) {
            int largest = curr * 2;
            if (largest + 1 <= size && items[largest + 1].height > items[largest].height) {
                largest++;
            }
            if (items[largest].height > x.height) {
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
        size = 0;
    }

    bool insert(HeapItem item) {
        if (size < MAX - 1) {
            items[++size] = item;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    HeapItem getMax() {
        if (size == 0) {
            return {-1, -1};
        }
        return items[1];
    }

    HeapItem extractMax() {
        if (size == 0) {
            return {-1, -1};
        }
        HeapItem maxVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return maxVal;
    }

    bool remove() {
        if (size == 0) {
            return false;
        }
        extractMax();
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
            cout << "(" << items[i].height << ", id=" << items[i].id << ") ";
        }
        cout << endl;
    }
};

void getSkyline(Building buildings[], int n, Point skyline[], int& skylineCount) {
    Event events[MAX];
    int eventCount = 0;
    bool active[MAX];

    for (int i = 0; i < n; i++) {
        active[i] = false;
        events[eventCount++] = {buildings[i].left, buildings[i].height, 0, i};
        events[eventCount++] = {buildings[i].right, buildings[i].height, 1, i};
    }

    quickSortEvents(events, 0, eventCount - 1);

    MaxHeap activeHeights;
    activeHeights.insert({0, -1}); // Ground level always active (id = -1)

    skylineCount = 0;
    int prevMax = 0;

    for (int i = 0; i < eventCount; i++) {
        Event e = events[i];
        if (e.type == 0) {
            active[e.id] = true;
            activeHeights.insert({e.height, e.id});
        } else {
            active[e.id] = false;
        }

        // Lazy deletion: discard ended buildings sitting at the heap root
        while (!activeHeights.isEmpty() && activeHeights.getMax().id != -1 &&
               !active[activeHeights.getMax().id]) {
            activeHeights.extractMax();
        }

        int currMax = activeHeights.getMax().height;
        if (currMax != prevMax) {
            skyline[skylineCount++] = {e.x, currMax};
            prevMax = currMax;
        }
    }
}

void printSkyline(Point pts[], int count) {
    cout << "[";
    for (int i = 0; i < count; i++) {
        cout << "[" << pts[i].x << ", " << pts[i].y << "]";
        if (i + 1 < count) {
            cout << ", ";
        }
    }
    cout << "]\n";
}

int main() {
    Building buildings1[5] = {
        {2, 9, 10},
        {3, 7, 15},
        {5, 12, 12},
        {15, 20, 10},
        {19, 24, 8}
    };
    Point ans1[MAX];
    int ansCount1 = 0;
    getSkyline(buildings1, 5, ans1, ansCount1);
    cout << "Test 1 (Classic 5-building skyline): ";
    printSkyline(ans1, ansCount1);

    Building buildings2[2] = {
        {0, 2, 3},
        {2, 5, 3}
    };
    Point ans2[MAX];
    int ansCount2 = 0;
    getSkyline(buildings2, 2, ans2, ansCount2);
    cout << "Test 2 (Touching equal-height buildings): ";
    printSkyline(ans2, ansCount2);

    Building buildings3[3] = {
        {1, 5, 10},
        {1, 3, 15},
        {3, 5, 10}
    };
    Point ans3[MAX];
    int ansCount3 = 0;
    getSkyline(buildings3, 3, ans3, ansCount3);
    cout << "Test 3 (Shared left & right edges): ";
    printSkyline(ans3, ansCount3);

    return 0;
}
