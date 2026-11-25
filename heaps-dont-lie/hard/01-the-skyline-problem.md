# The Skyline Problem

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — The Skyline Problem](https://www.geeksforgeeks.org/dsa/the-skyline-problem-using-divide-and-conquer-algorithm/)

---

## 1. Problem Statement

Given a list of `buildings`, where `buildings[i] = [left, right, height]` describes a rectangular building spanning `[left, right)` on the x-axis with height `height`, compute the **skyline** formed by all buildings together.

The skyline is represented as a list of key points `[x, y]` sorted by `x`, where each key point marks the left endpoint of a horizontal segment of height `y`. The final key point has `y = 0` (returning to ground level), and no two consecutive key points may have the same height `y`.

---

## 2. Pattern Recognition & Intuition

### Why think "Heap" here?
When you sweep a vertical scan-line from left to right across the x-axis:
- At any x-coordinate, the skyline height is simply the **maximum height among all currently active buildings** (buildings whose `[left, right)` interval covers `x`).
- As the sweep-line crosses a building's `left` edge, that height **enters** the active set.
- As the sweep-line crosses a building's `right` edge, that height **leaves** the active set.

Whenever a problem asks you to sweep across intervals and repeatedly query **"what is the current maximum/minimum among active items?"**, think **Sweep-Line + Heap**.

### Min-Heap vs. Max-Heap Decision Rule
- We need the **tallest** active building at every event coordinate, so we use a **Max-Heap**.
- Why not a Min-Heap? A Min-Heap gives the shortest active building, which is hidden behind taller buildings and never determines the silhouette.

### Why this beats naive approaches
- **Brute-force grid / coordinate scan:** Checking every building across all x-coordinates takes $O(N \cdot X)$ or $O(N^2)$ time when coordinates are large (up to $2^{31}-1$).
- **Sweep-line + Max-Heap:** Only $2N$ event coordinates matter. Each insertion or lazy deletion takes $O(\log N)$ amortized time, giving $O(N \log N)$ total.

### Key Invariants & Lazy Deletion
A standard binary heap only supports removing from the **root** in $O(\log N)$, not removing an arbitrary interior value when a building ends. To bridge that gap, we pair the `MaxHeap` with a `delayedRemoval` hash map:
1. **Ground-level sentinel:** Insert `0` into `activeHeights` at the start. Because `0` is never removed, the heap is never empty and handles gaps between disconnected building clusters automatically.
2. **Lazy deletion:** When a building ends, increment `delayedRemoval[height]` instead of searching inside the heap. Before reading `getMax()`, pop any root elements whose `delayedRemoval[top] > 0`.
3. **Tie-breaking at equal `x`:**
   - Process **start** events before **end** events (so touching buildings like `[0, 2, 3]` and `[2, 5, 3]` keep height `3` continuously without dipping to `0`).
   - Among starts at the same `x`, process **taller** buildings first (so a shorter building starting at the same `x` does not emit a false intermediate step).
   - Among ends at the same `x`, process **shorter** buildings first (so a shorter building ending underneath a taller one does not trigger a false drop).

### Edge Cases
- **Disconnected buildings:** Ground sentinel `0` naturally emits `[right, 0]` when the last active building in a cluster ends.
- **Duplicate heights:** `delayedRemoval` stores counts, not booleans, so two overlapping buildings of height `10` decrement cleanly one by one.
- **Touching buildings of equal height:** Start-before-end tie-breaking keeps `currMax == prevMax`, avoiding redundant horizontal points.

---

## 3. Step-by-Step Trace

Using **Test 1** from `main()`:
`buildings = [[2, 9, 10], [3, 7, 15], [5, 12, 12], [15, 20, 10], [19, 24, 8]]`

Sorted events `(x, height, type)` (`S` = start, `E` = end):
1. `(2, 10, S)`
2. `(3, 15, S)`
3. `(5, 12, S)`
4. `(7, 15, E)`
5. `(9, 10, E)`
6. `(12, 12, E)`
7. `(15, 10, S)`
8. `(19, 8, S)`
9. `(20, 10, E)`
10. `(24, 8, E)`

Initial state: `activeHeights = [0]`, `delayedRemoval = {}`, `prevMax = 0`.

| Step | Event | Action | Lazy Prune Top | `activeHeights` (logical) | `currMax` | `prevMax` | Emit? |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `(2, 10, S)` | Insert `10` | None | `{10, 0}` | `10` | `0 -> 10` | **`[2, 10]`** |
| 2 | `(3, 15, S)` | Insert `15` | None | `{15, 10, 0}` | `15` | `10 -> 15` | **`[3, 15]`** |
| 3 | `(5, 12, S)` | Insert `12` | None | `{15, 12, 10, 0}` | `15` | `15` | No |
| 4 | `(7, 15, E)` | `delayed[15] = 1` | Pop `15` (`delayed[15]=0`) | `{12, 10, 0}` | `12` | `15 -> 12` | **`[7, 12]`** |
| 5 | `(9, 10, E)` | `delayed[10] = 1` | Top is `12` (`delayed[12]=0`), stop | `{12, 0}` (+ stale `10`) | `12` | `12` | No |
| 6 | `(12, 12, E)` | `delayed[12] = 1` | Pop `12`, then pop stale `10`! | `{0}` | `0` | `12 -> 0` | **`[12, 0]`** |
| 7 | `(15, 10, S)` | Insert `10` | None | `{10, 0}` | `10` | `0 -> 10` | **`[15, 10]`** |
| 8 | `(19, 8, S)` | Insert `8` | None | `{10, 8, 0}` | `10` | `10` | No |
| 9 | `(20, 10, E)` | `delayed[10] = 1` | Pop `10` | `{8, 0}` | `8` | `10 -> 8` | **`[20, 8]`** |
| 10 | `(24, 8, E)` | `delayed[8] = 1` | Pop `8` | `{0}` | `0` | `8 -> 0` | **`[24, 0]`** |

Final Output: `[[2, 10], [3, 15], [7, 12], [12, 0], [15, 10], [20, 8], [24, 0]]`.

---

## 4. Time & Space Complexity

- **Time Complexity:** $O(N \log N)$
  - Sorting $2N$ events takes $O(N \log N)$.
  - Each of the $N$ buildings is inserted into `MaxHeap` once and extracted at most once during lazy deletion: $O(N \log N)$ total heap work.
- **Space Complexity:** $O(N)$
  - `events` holds $2N$ entries, `MaxHeap` holds at most $N + 1$ elements, and `delayedRemoval` holds at most $N$ distinct heights.

---

## 5. C++ Implementation

See [01-the-skyline-problem.cpp](01-the-skyline-problem.cpp).

```cpp
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
```

---

## 6. LeetCode Pattern Takeaways

This **Sweep-Line + Heap with Lazy Deletion** pattern transfers directly to:
- **LC 218 — The Skyline Problem** (exact problem)
- **LC 480 — Sliding Window Median** (lazy deletion across a sliding window)
- **LC 239 — Sliding Window Maximum** (solvable with Max-Heap + lazy deletion, though monotonic deque reaches $O(N)$)
- **LC 1851 — Minimum Interval to Include Each Query** (sort queries & intervals, sweep with Min-Heap of active interval lengths)
- **LC 253 — Meeting Rooms II** (sweep-line + Min-Heap of end times)

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the sweep-line `MaxHeap` solution to match the fixed-size raw-array style in `Heap-Implementation-OPTIMIZED.cpp` (`#define MAX 200`, 1-indexed `items[MAX]`, `int size`, `heapifyUp`, `maxHeapify`, `insert`, `getMax`, `extractMax`, `remove`, `isEmpty`, `getSize`, `display`), with no STL containers, no `<algorithm>`, no global variables, and no `const`/`static`/`constexpr`:
- `events[MAX]` and `skyline[MAX]` replace `std::vector`, sorted in-place with a manual `quickSortEvents` helper.
- Instead of `unordered_map<int, int>`, each building gets an index `id` (`0..n-1`) stored inside `HeapItem { height, id }` alongside a raw `bool active[MAX]` array for $O(1)$ lazy deletion (`!active[activeHeights.getMax().id]`), with `{0, -1}` as the permanent ground sentinel.

See [01-the-skyline-problem-array.cpp](01-the-skyline-problem-array.cpp).

```cpp
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
```
