# 09. K Maximum Sum Combinations from Two Arrays

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — K maximum sum combinations from two arrays](https://www.geeksforgeeks.org/dsa/k-maximum-sum-combinations-two-arrays/)

---

## Problem Statement

Given two integer arrays `A` (of size `N`) and `B` (of size `M`) and an integer `K`, find the **K largest sum combinations** formed by picking one element from `A` and one element from `B`, i.e., `A[i] + B[j]`. Return the top `K` sums in descending order.

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
If you generate all `N * M` pairs `(i, j)`, compute `A[i] + B[j]`, and sort them, it takes $O(NM \log(NM))$ time and $O(NM)$ space. When `N = 10^5` and `K = 100`, computing $10^{10}$ pairs is impossible.

Instead, notice the **monotonicity after sorting**:
1. Sort both `A` and `B` in **descending** order.
2. The single largest sum MUST be `A[0] + B[0]`.
3. Once we pick `(i, j)`, what are the *only* immediate next-best candidates spawned by `(i, j)`? Only `(i + 1, j)` and `(i, j + 1)`, because `A[i + 1] <= A[i]` and `B[j + 1] <= B[j]`.

This turns the problem into a **Best-First Search (Dijkstra-like frontier)** over an implicit 2D sorted matrix of sums, where we only ever explore the frontier of size $O(K)$!

### Min-Heap vs Max-Heap Decision Rule
- We want the **K largest** sums *one by one* starting from the global maximum (`A[0] + B[0]`), expanding neighbors only when a pair is popped.
- Therefore, we use a **Max-Heap** keyed by `sum = A[i] + B[j]` (storing `{sum, i, j}`).
- Contrast with "K-th largest in a stream": there we don't have sorted arrays to expand lazily, so we cap a Min-Heap at size `K`. Here, sorting lets us expand lazily from the top down using a **Max-Heap** in $O(K \log K)$ after sorting!

### Key Invariants & Edge Cases
- **Duplicate Pair Invariant:** A cell `(i + 1, j + 1)` can be reached from both `(i, j + 1)` and `(i + 1, j)`. Track visited `(i, j)` index pairs in a `set<pair<int, int>>` so the same index pair is never pushed twice.
- **Note on Duplicate Values:** Duplicate *sums* from *different* `(i, j)` pairs ARE valid and should be kept (e.g., `4+5=9` and `3+6=9`). Deduplicate by `(i, j)` indices, never by sum value.
- **Edge Cases:** `k == 0`, empty arrays, `k > N * M` (the `!heap.isEmpty()` loop guard handles `k > N * M` cleanly).

---

## Step-by-Step Trace

Using **Test 1** from `main()`:
- `A = [4, 2, 5, 1]`, `B = [8, 0, 3, 5]`, `K = 3`

1. **Sort descending:**
   - `A = [5, 4, 2, 1]`
   - `B = [8, 5, 3, 0]`

2. **Initialize Max-Heap & Visited:**
   - Push `(5 + 8 = 13, 0, 0)`, `visited = {(0,0)}`
   - Heap: `[(13, 0, 0)]`

3. **Pop #1:**
   - Extract max `(13, 0, 0)` -> `result = [13]`
   - Push `(0+1, 0) = (1, 0)` -> `A[1] + B[0] = 4 + 8 = 12`
   - Push `(0, 0+1) = (0, 1)` -> `A[0] + B[1] = 5 + 5 = 10`
   - Heap: `[(12, 1, 0), (10, 0, 1)]`, `visited = {(0,0), (1,0), (0,1)}`

4. **Pop #2:**
   - Extract max `(12, 1, 0)` -> `result = [13, 12]`
   - Push `(2, 0)` -> `A[2] + B[0] = 2 + 8 = 10`
   - Push `(1, 1)` -> `A[1] + B[1] = 4 + 5 = 9`
   - Heap: `[(10, 0, 1), (10, 2, 0), (9, 1, 1)]`

5. **Pop #3:**
   - Extract max `(10, 0, 1)` -> `result = [13, 12, 10]`
   - `result.size() == 3 == K` -> Stop!

**Final Output:** `[13, 12, 10]`

---

## Time & Space Complexity

- **Time Complexity:** $O(N \log N + M \log M + K \log K)$
  - Sorting `A` and `B` takes $O(N \log N + M \log M)$.
  - Each of the `K` iterations pops once and pushes at most 2 elements into a heap and `set` of size at most $2K$, taking $O(\log K)$ per step.
- **Space Complexity:** $O(K)$ extra space for the Max-Heap, `visited` set, and output array (ignoring input sorting space).

---

## C++ Implementation

Source file: [09-k-maximum-sum-combinations.cpp](09-k-maximum-sum-combinations.cpp)

```cpp
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
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the solution to pure fixed-size raw C++ arrays (`#define MAX 100`, `items[MAX]`, `int size`, and `int arr[], int n` parameters), matching the 1-indexed array-backed heap style in `Heap-Implementation-OPTIMIZED.cpp` without any STL headers (`<vector>`, `<set>`, `<algorithm>`, or `<utility>`):
- `MaxHeap` uses a fixed `SumNode items[MAX]` array (1-indexed from `1..size`) with `insert`, `getMax`, `extractMax`, `isEmpty`, `getSize`, and `display`.
- `std::sort` is replaced by an in-place `sortDescending(int arr[], int n)` insertion sort.
- `set<pair<int, int>>` is replaced by a fixed `IndexPair visited[MAX]` array paired with a linear `wasVisited` lookup, and results are written into `int result[]` returning `resCount`.

Source file: [09-k-maximum-sum-combinations-array.cpp](09-k-maximum-sum-combinations-array.cpp)

```cpp
#include <iostream>
#define MAX 100
using namespace std;

struct SumNode {
    int sum;
    int i;
    int j;
};

struct IndexPair {
    int i;
    int j;
};

class MaxHeap {
private:
    SumNode items[MAX];
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
        size = 0;
    }

    bool insert(SumNode node) {
        if (size < MAX - 1) {
            items[++size] = node;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    SumNode getMax() {
        if (size == 0) {
            SumNode empty = {0, -1, -1};
            return empty;
        }
        return items[1];
    }

    SumNode extractMax() {
        if (size == 0) {
            SumNode empty = {0, -1, -1};
            return empty;
        }
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

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << items[i].sum << " ";
        }
        cout << endl;
    }
};

void sortDescending(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] < key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

bool wasVisited(IndexPair visited[], int visitedCount, int i, int j) {
    for (int idx = 0; idx < visitedCount; idx++) {
        if (visited[idx].i == i && visited[idx].j == j) {
            return true;
        }
    }
    return false;
}

int kMaxSumCombinations(int a[], int n, int b[], int m, int k, int result[]) {
    if (n <= 0 || m <= 0 || k <= 0) return 0;

    sortDescending(a, n);
    sortDescending(b, m);

    MaxHeap heap;
    IndexPair visited[MAX];
    int visitedCount = 0;

    SumNode first = {a[0] + b[0], 0, 0};
    heap.insert(first);
    visited[visitedCount++] = {0, 0};

    int resCount = 0;
    while (!heap.isEmpty() && resCount < k && resCount < MAX) {
        SumNode top = heap.extractMax();
        result[resCount++] = top.sum;

        int i = top.i;
        int j = top.j;

        if (i + 1 < n && !wasVisited(visited, visitedCount, i + 1, j)) {
            SumNode nextDown = {a[i + 1] + b[j], i + 1, j};
            heap.insert(nextDown);
            if (visitedCount < MAX) {
                visited[visitedCount++] = {i + 1, j};
            }
        }
        if (j + 1 < m && !wasVisited(visited, visitedCount, i, j + 1)) {
            SumNode nextRight = {a[i] + b[j + 1], i, j + 1};
            heap.insert(nextRight);
            if (visitedCount < MAX) {
                visited[visitedCount++] = {i, j + 1};
            }
        }
    }

    return resCount;
}

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    int a1[MAX] = {4, 2, 5, 1};
    int b1[MAX] = {8, 0, 3, 5};
    int k1 = 3;
    int res1[MAX];
    int resLen1 = kMaxSumCombinations(a1, 4, b1, 4, k1, res1);
    cout << "Test 1 (K = " << k1 << "): ";
    printArray(res1, resLen1);
    cout << "\n";

    int a2[MAX] = {1, 4, 2, 3};
    int b2[MAX] = {2, 5, 1, 6};
    int k2 = 4;
    int res2[MAX];
    int resLen2 = kMaxSumCombinations(a2, 4, b2, 4, k2, res2);
    cout << "Test 2 (K = " << k2 << "): ";
    printArray(res2, resLen2);
    cout << "\n";

    return 0;
}
```

---

## LeetCode Pattern Takeaways

This **"Sorted Arrays + Max/Min-Heap Frontier with Visited Set"** (multi-way merge on an implicit grid) shows up directly in:
- **LeetCode 373** — *Find K Pairs with Smallest Sums* (mirror image: sort ascending, use a **Min-Heap**!)
- **LeetCode 378** — *Kth Smallest Element in a Sorted Matrix*
- **LeetCode 1439** — *Find the Kth Smallest Sum of a Matrix With Sorted Rows*
- **LeetCode 786** — *K-th Smallest Prime Fraction*
