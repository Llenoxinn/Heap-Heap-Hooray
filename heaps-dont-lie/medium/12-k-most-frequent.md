# 12. K Most Frequent Elements

- **Difficulty:** Medium
- **Reference:** [GeeksforGeeks — Find k numbers with most occurrences in the given array](https://www.geeksforgeeks.org/dsa/find-k-numbers-occurrences-given-array/)

---

## Problem Statement

Given an integer array `nums` of size `N` and an integer `k`, find the `k` numbers with the **highest frequencies** in `nums`. If two numbers have the same frequency, the **larger number** takes precedence. Return the `k` numbers sorted in descending order of priority.

---

## Pattern Recognition & Intuition

### Why think "Heap" here?
First, frequency counting requires a hash map (`unordered_map<int, int>`), which takes $O(N)$ time and leaves us with `U` unique `(val, freq)` pairs (`U <= N`).
- **Naive approach:** Sort all `U` pairs by `(freq, val)` -> $O(N + U \log U)$ time.
- **Heap approach:** We only need the **top `k`**, not a full sort of all `U` unique elements! By maintaining a bounded heap of size `k`, we reduce the selection cost to $O(U \log k)$.

### Min-Heap vs Max-Heap Decision Rule
- We want the **K MOST frequent** elements.
- Therefore, maintain a **Min-Heap of size at most `k`**, ordered by **lowest priority at the root** (`hasLowerPriority(a, b)` is true when `a.freq < b.freq`, or `a.freq == b.freq && a.val < b.val`).
- Why? The root always holds the *weakest* member currently inside our top-`k` club. Whenever a stronger `(val, freq)` candidate comes along, we evict the root (`extractMin()`) and insert the newcomer.
- When finished, popping the remaining `k` items gives them from weakest to strongest, so a single `reverse()` yields descending order!

### Key Invariants & Edge Cases
- **Tie-Breaker Consistency:** The exact same comparator `hasLowerPriority` must be used in `heapifyUp`, `minHeapify`, AND the gatekeeper check `hasLowerPriority(minHeap.getMin(), node)`.
- **Edge Cases:** `k <= 0`, empty array, `k >= U` (all unique elements are returned, ordered by `(freq, val)`).

---

## Step-by-Step Trace

Using **Test 1** from `main()`:
- `arr = [3, 1, 4, 4, 5, 2, 6, 1]`, `k = 2`

1. **Frequency Map:**
   - `1 -> 2`
   - `4 -> 2`
   - `2 -> 1`
   - `3 -> 1`
   - `5 -> 1`
   - `6 -> 1`

2. **Bounded Min-Heap (`size <= 2`) Processing:**
   Even though `unordered_map` visits keys in hash-bucket order, the heap invariant guarantees the same top-`k` survivors regardless of visit order. For example, suppose keys are visited in order `3(1), 1(2), 4(2), 5(1), 2(1), 6(1)`:
   - Visit `(3, 1)`: `size < 2` -> insert. Heap: `[(3, 1)]`
   - Visit `(1, 2)`: `size < 2` -> insert. Heap: `[(3, 1), (1, 2)]`
   - Visit `(4, 2)`: `(3, 1)` < `(4, 2)` -> pop `(3, 1)`, push `(4, 2)`. Heap: `[(1, 2), (4, 2)]`
   - Visit `(5, 1)`: `(1, 2)` is NOT lower priority than `(5, 1)` -> skip.
   - Visit `(2, 1)`: skip.
   - Visit `(6, 1)`: skip.

3. **Drain & Reverse:**
   - Extract min #1: `(1, 2)` (since `freq == 2` ties, `1 < 4` so `1` has lower priority!)
   - Extract min #2: `(4, 2)`
   - Collected: `[1, 4]` -> Reverse: `[4, 1]`

**Final Output:** `[4, 1]`

---

## Time & Space Complexity

- **Time Complexity:** $O(N + U \log K)$, where `U` is the number of distinct elements (`U <= N`).
  - Counting frequencies takes $O(N)$ average time.
  - Processing `U` entries against a size-`K` Min-Heap takes $O(U \log K)$.
  - Draining and reversing `K` items takes $O(K \log K)$.
- **Space Complexity:** $O(U + K)$ for the frequency hash map (`U` entries) and the Min-Heap (`K` entries). *(Note: Bucket sort can achieve $O(N)$ time when ties aren't ordered by value, whereas the Min-Heap approach handles arbitrary tie-breaking cleanly in $O(U \log K)$.)*

---

## C++ Implementation

Source file: [12-k-most-frequent.cpp](12-k-most-frequent.cpp)

```cpp
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
```

---

## Alternative Implementation: Pure Raw-Array Style (No std::vector / STL Containers)

This version adapts the solution to pure fixed-size raw C++ arrays (`#define MAX 100`, `items[MAX]`, `int size`, and `int nums[], int n` parameters), matching `Heap-Implementation-OPTIMIZED.cpp` without `<vector>`, `<unordered_map>`, or `<algorithm>`:
- `MinHeap` stores `FreqNode` entries in a 1-indexed `FreqNode items[MAX]` array (`1..size`) with `insert`, `getMin`, `extractMin`, `isEmpty`, `getSize`, and `display`.
- `unordered_map<int, int>` is replaced by a fixed `FreqNode counts[MAX]` array that accumulates distinct values and their frequencies via linear lookup.
- `std::reverse` is replaced by a manual two-pointer `reverseIntArray(int arr[], int n)` helper before returning `resCount`.

Source file: [12-k-most-frequent-array.cpp](12-k-most-frequent-array.cpp)

```cpp
#include <iostream>
#define MAX 100
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
    FreqNode items[MAX];
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
        size = 0;
    }

    bool insert(FreqNode node) {
        if (size < MAX - 1) {
            items[++size] = node;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    FreqNode getMin() {
        if (size == 0) {
            FreqNode empty = {0, 0};
            return empty;
        }
        return items[1];
    }

    FreqNode extractMin() {
        if (size == 0) {
            FreqNode empty = {0, 0};
            return empty;
        }
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

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << "(" << items[i].val << ":" << items[i].freq << ") ";
        }
        cout << endl;
    }
};

void reverseIntArray(int arr[], int n) {
    int left = 0;
    int right = n - 1;
    while (left < right) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

int kMostFrequent(int nums[], int n, int k, int result[]) {
    if (k <= 0 || n <= 0) return 0;

    FreqNode counts[MAX];
    int uniqueCount = 0;

    for (int i = 0; i < n; i++) {
        int foundIdx = -1;
        for (int j = 0; j < uniqueCount; j++) {
            if (counts[j].val == nums[i]) {
                foundIdx = j;
                break;
            }
        }
        if (foundIdx != -1) {
            counts[foundIdx].freq++;
        } else if (uniqueCount < MAX) {
            counts[uniqueCount].val = nums[i];
            counts[uniqueCount].freq = 1;
            uniqueCount++;
        }
    }

    MinHeap minHeap;
    for (int i = 0; i < uniqueCount; i++) {
        FreqNode node = counts[i];
        if (minHeap.getSize() < k) {
            minHeap.insert(node);
        } else if (hasLowerPriority(minHeap.getMin(), node)) {
            minHeap.extractMin();
            minHeap.insert(node);
        }
    }

    int resCount = 0;
    while (!minHeap.isEmpty() && resCount < MAX) {
        result[resCount++] = minHeap.extractMin().val;
    }
    reverseIntArray(result, resCount);
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
    int arr1[MAX] = {3, 1, 4, 4, 5, 2, 6, 1};
    int n1 = 8;
    int k1 = 2;
    int res1[MAX];
    int resLen1 = kMostFrequent(arr1, n1, k1, res1);
    cout << "Test 1 (k = " << k1 << "): ";
    printArray(res1, resLen1);
    cout << "\n";

    int arr2[MAX] = {7, 10, 11, 5, 2, 5, 5, 7, 11, 8, 9};
    int n2 = 11;
    int k2 = 4;
    int res2[MAX];
    int resLen2 = kMostFrequent(arr2, n2, k2, res2);
    cout << "Test 2 (k = " << k2 << "): ";
    printArray(res2, resLen2);
    cout << "\n";

    return 0;
}
```

---

## LeetCode Pattern Takeaways

This **"Hash Map + Bounded Min-Heap with Custom Comparator"** pattern powers:
- **LeetCode 347** — *Top K Frequent Elements*
- **LeetCode 692** — *Top K Frequent Words* (alphabetical tie-breaker!)
- **LeetCode 451** — *Sort Characters By Frequency*
- **LeetCode 1636** — *Sort Array by Increasing Frequency*
