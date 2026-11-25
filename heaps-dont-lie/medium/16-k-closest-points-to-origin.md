# 16. K Closest Points to the Origin

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Find K Closest Points to the Origin](https://www.geeksforgeeks.org/dsa/find-k-closest-points-to-the-origin/)

---

## Problem Statement

Given an array of 2D integer coordinates `points`, where `points[i] = (x, y)`, and an integer `k`, return the **`k` closest points** to the origin `(0, 0)` measured by Euclidean distance $\sqrt{x^2 + y^2}$.

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
Sorting all `N` points by distance takes $O(N \log N)$ time. Since we only care about the **`k` smallest** distances, we can maintain a bounded heap of size `k` and drop the time complexity to $O(N \log k)$ (and $O(k)$ auxiliary space).

### Min-Heap vs Max-Heap Decision Rule
Again, use the **opposite** heap of what the problem asks for!
- The problem asks for the **`k` CLOSEST (smallest distance)** points.
- Therefore, maintain a **Max-Heap of size `k`** keyed by distance!
- Why? The root of the Max-Heap holds the **farthest** point among our current `k` best candidates.
  - While `heap.getSize() < k`, insert every point.
  - Once `heap.getSize() == k`, if a new point `p` has `p.distSq < maxHeap.getMax().distSq`, it belongs inside the top `k` closest — so evict the farthest candidate (`extractMax()`) and insert `p`!

### Key Invariants & Edge Cases
- **Avoid Floating Point & `sqrt`:** Because $f(d) = \sqrt{d}$ is strictly increasing for $d \ge 0$, comparing $x^2 + y^2$ gives the **exact same ordering** as $\sqrt{x^2 + y^2}$ with zero floating-point precision error.
- **Overflow Guard:** If coordinates can reach $10^4$ or $10^5$, `x * x + y * y` can exceed 32-bit `int` max ($2.14 \times 10^9$). Store `distSq` as `long long` (`1LL * x * x + 1LL * y * y`).
- **Edge Cases:** `k <= 0`, empty `points`, `k >= N` (all points remain in the heap), ties in distance.

---

## Step-by-Step Trace

Using **Test 2** from `main()`:
- `points = [(3, 3), (5, -1), (-2, 4)]`, `k = 2`

| Step | Point `(x, y)` | `distSq = x^2 + y^2` | Action | Max-Heap State `(x, y : distSq)` |
| :---: | :---: | :---: | :--- | :--- |
| 1 | `(3, 3)` | `9 + 9 = 18` | `size < 2` -> insert | `[(3, 3 : 18)]` |
| 2 | `(5, -1)` | `25 + 1 = 26` | `size < 2` -> insert | `[(5, -1 : 26), (3, 3 : 18)]` |
| 3 | `(-2, 4)` | `4 + 16 = 20` | `20 < max(26)` -> pop `(5, -1)`, push `(-2, 4)` | `[(-2, 4 : 20), (3, 3 : 18)]` |

Drain & reverse so closest comes first:
- Extract max: `(-2, 4)` (distSq 20), then `(3, 3)` (distSq 18).
- Reversed: `[(3, 3), (-2, 4)]`.

**Final Output:** `[(3, 3), (-2, 4)]`

---

## Time & Space Complexity

- **Time Complexity:** $O(N \log K)$
  - Each of the `N` points causes at most one `extractMax()` and one `insert()` on a Max-Heap of size at most `K`.
  - Draining `K` points at the end takes $O(K \log K)$.
- **Space Complexity:** $O(K)$ auxiliary space for the Max-Heap (vs $O(N)$ if you built a Min-Heap of all `N` points).

---

## C++ Implementation

Source file: [16-k-closest-points-to-origin.cpp](16-k-closest-points-to-origin.cpp)

```cpp
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    int x;
    int y;
    long long distSq;
};

class MaxHeap {
private:
    vector<Point> items;
    int size;

    void heapifyUp(int curr) {
        Point p = items[curr];
        while (curr / 2 > 0 && p.distSq > items[curr / 2].distSq) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = p;
    }

    void maxHeapify(int curr) {
        Point p = items[curr];
        while (curr * 2 <= size) {
            int child = curr * 2;
            if (child + 1 <= size && items[child + 1].distSq > items[child].distSq) {
                child++;
            }
            if (items[child].distSq > p.distSq) {
                items[curr] = items[child];
                curr = child;
            } else {
                break;
            }
        }
        items[curr] = p;
    }

public:
    MaxHeap() {
        items.push_back({0, 0, 0});
        size = 0;
    }

    void insert(Point p) {
        size++;
        if (size < (int)items.size()) {
            items[size] = p;
        } else {
            items.push_back(p);
        }
        heapifyUp(size);
    }

    Point getMax() {
        if (size == 0) return {0, 0, 0};
        return items[1];
    }

    Point extractMax() {
        if (size == 0) return {0, 0, 0};
        Point topPoint = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return topPoint;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<Point> kClosestPoints(vector<Point>& points, int k) {
    vector<Point> result;
    if (k <= 0 || points.empty()) return result;

    MaxHeap maxHeap;
    for (int i = 0; i < (int)points.size(); i++) {
        Point p = points[i];
        p.distSq = 1LL * p.x * p.x + 1LL * p.y * p.y;

        if (maxHeap.getSize() < k) {
            maxHeap.insert(p);
        } else if (p.distSq < maxHeap.getMax().distSq) {
            maxHeap.extractMax();
            maxHeap.insert(p);
        }
    }

    while (!maxHeap.isEmpty()) {
        result.push_back(maxHeap.extractMax());
    }
    reverse(result.begin(), result.end());
    return result;
}

void printPoints(vector<Point>& pts) {
    cout << "[";
    for (int i = 0; i < (int)pts.size(); i++) {
        if (i > 0) cout << ", ";
        cout << "(" << pts[i].x << ", " << pts[i].y << ")";
    }
    cout << "]";
}

int main() {
    vector<Point> p1 = {{1, 3, 0}, {-2, 2, 0}};
    int k1 = 1;
    vector<Point> res1 = kClosestPoints(p1, k1);
    cout << "Test 1 (k = " << k1 << "): ";
    printPoints(res1);
    cout << "\n";

    vector<Point> p2 = {{3, 3, 0}, {5, -1, 0}, {-2, 4, 0}};
    int k2 = 2;
    vector<Point> res2 = kClosestPoints(p2, k2);
    cout << "Test 2 (k = " << k2 << "): ";
    printPoints(res2);
    cout << "\n";

    vector<Point> p3 = {{1, 0, 0}, {2, 1, 0}, {0, -1, 0}, {-3, 2, 0}, {1, 1, 0}};
    int k3 = 3;
    vector<Point> res3 = kClosestPoints(p3, k3);
    cout << "Test 3 (k = " << k3 << "): ";
    printPoints(res3);
    cout << "\n";

    return 0;
}
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the solution to pure fixed-size raw C++ arrays (`#define MAX 100`, `items[MAX]`, `int size`, and `Point points[], int n` parameters), matching `Heap-Implementation-OPTIMIZED.cpp` without `<vector>` or `<algorithm>`:
- `MaxHeap` stores `Point` values in a 1-indexed `Point items[MAX]` array (`1..size`) with `insert`, `getMax`, `extractMax`, `isEmpty`, `getSize`, and `display`.
- `kClosestPoints(Point points[], int n, int k, Point result[])` writes the selected points into `result[]`, manually reverses them via `reversePoints(Point pts[], int n)`, and returns `resCount`.

Source file: [16-k-closest-points-to-origin-array.cpp](16-k-closest-points-to-origin-array.cpp)

```cpp
#include <iostream>
#define MAX 100
using namespace std;

struct Point {
    int x;
    int y;
    long long distSq;
};

class MaxHeap {
private:
    Point items[MAX];
    int size;

    void heapifyUp(int curr) {
        Point p = items[curr];
        while (curr / 2 > 0 && p.distSq > items[curr / 2].distSq) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = p;
    }

    void maxHeapify(int curr) {
        Point p = items[curr];
        while (curr * 2 <= size) {
            int child = curr * 2;
            if (child + 1 <= size && items[child + 1].distSq > items[child].distSq) {
                child++;
            }
            if (items[child].distSq > p.distSq) {
                items[curr] = items[child];
                curr = child;
            } else {
                break;
            }
        }
        items[curr] = p;
    }

public:
    MaxHeap() {
        size = 0;
    }

    bool insert(Point p) {
        if (size < MAX - 1) {
            items[++size] = p;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    Point getMax() {
        if (size == 0) {
            Point empty = {0, 0, 0};
            return empty;
        }
        return items[1];
    }

    Point extractMax() {
        if (size == 0) {
            Point empty = {0, 0, 0};
            return empty;
        }
        Point topPoint = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return topPoint;
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
            cout << "(" << items[i].x << ", " << items[i].y << ") ";
        }
        cout << endl;
    }
};

void reversePoints(Point pts[], int n) {
    int left = 0;
    int right = n - 1;
    while (left < right) {
        Point temp = pts[left];
        pts[left] = pts[right];
        pts[right] = temp;
        left++;
        right--;
    }
}

int kClosestPoints(Point points[], int n, int k, Point result[]) {
    if (k <= 0 || n <= 0) return 0;

    MaxHeap maxHeap;
    for (int i = 0; i < n; i++) {
        Point p = points[i];
        p.distSq = 1LL * p.x * p.x + 1LL * p.y * p.y;

        if (maxHeap.getSize() < k) {
            maxHeap.insert(p);
        } else if (p.distSq < maxHeap.getMax().distSq) {
            maxHeap.extractMax();
            maxHeap.insert(p);
        }
    }

    int resCount = 0;
    while (!maxHeap.isEmpty() && resCount < MAX) {
        result[resCount++] = maxHeap.extractMax();
    }
    reversePoints(result, resCount);
    return resCount;
}

void printPoints(Point pts[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ", ";
        cout << "(" << pts[i].x << ", " << pts[i].y << ")";
    }
    cout << "]";
}

int main() {
    Point p1[MAX] = {{1, 3, 0}, {-2, 2, 0}};
    int n1 = 2;
    int k1 = 1;
    Point res1[MAX];
    int resLen1 = kClosestPoints(p1, n1, k1, res1);
    cout << "Test 1 (k = " << k1 << "): ";
    printPoints(res1, resLen1);
    cout << "\n";

    Point p2[MAX] = {{3, 3, 0}, {5, -1, 0}, {-2, 4, 0}};
    int n2 = 3;
    int k2 = 2;
    Point res2[MAX];
    int resLen2 = kClosestPoints(p2, n2, k2, res2);
    cout << "Test 2 (k = " << k2 << "): ";
    printPoints(res2, resLen2);
    cout << "\n";

    Point p3[MAX] = {{1, 0, 0}, {2, 1, 0}, {0, -1, 0}, {-3, 2, 0}, {1, 1, 0}};
    int n3 = 5;
    int k3 = 3;
    Point res3[MAX];
    int resLen3 = kClosestPoints(p3, n3, k3, res3);
    cout << "Test 3 (k = " << k3 << "): ";
    printPoints(res3, resLen3);
    cout << "\n";

    return 0;
}
```

---

## LeetCode Pattern Takeaways

This **"Bounded Max-Heap of Size K for K Smallest"** pattern appears across geometry and selection problems:
- **LeetCode 973** — *K Closest Points to Origin* (exact match!)
- **LeetCode 658** — *Find K Closest Elements* (when unsorted, Max-Heap of size `K`; when sorted, two pointers / binary search)
- **LeetCode 1985** / **LeetCode 215** — *Kth Largest* (mirror: Min-Heap of size `K`)
- **LeetCode 703** — *Kth Largest Element in a Stream*
