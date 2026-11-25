# 10. Largest Derangement of a Sequence

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — Largest Derangement of a Sequence](https://www.geeksforgeeks.org/dsa/largest-derangement-sequence/)

---

## 1. Problem Statement

Given a sequence `seq` of `N` distinct integers, find the **lexicographically largest derangement** `res` of `seq` — that is, a permutation of `seq` such that `res[i] != seq[i]` for every index `0 <= i < N`, and `res` is as large as possible from left to right.

---

## 2. Pattern Recognition & Intuition

### Why Think "Heap" Here?
To maximize `res` lexicographically, we want to place the **largest available unused element** at index `0`, then the largest remaining at index `1`, and so on — with one constraint: at index `i`, we cannot place `seq[i]`.

- **Naive approach:** Generate permutations in descending lexicographical order and return the first derangement (`O(N! * N)`), or sort descending and manually resolve collisions with backtracking.
- **Heap approach:** Store all elements in a **Max-Heap**. For each position `i`:
  - Pop the largest remaining element `d`.
  - If `d != seq[i]`, greedily place `res[i] = d`.
  - If `d == seq[i]` (and `i < N - 1`), pop the second largest `nextMax`, place `res[i] = nextMax`, and **re-insert `d` back into the Max-Heap** so `d` can be used immediately at `i + 1`!

### Min-Heap vs. Max-Heap Decision Rule
- We are maximizing lexicographically from left to right, so we always need the **maximum** unused value (and occasionally the second maximum). That directly calls for a **Max-Heap**.

### Key Invariants & Why the Last-Two-Element Swap Works
What happens at the very last index `i = N - 1`? Only one element remains in the Max-Heap. If that lone remaining element equals `seq[N - 1]`, we cannot fetch a second-largest element from the heap.

**Why swapping `res[N - 1]` and `res[N - 2]` always fixes it (for `N >= 2` distinct elements):**
1. The greedy step at `N - 2` guaranteed `res[N - 2] != seq[N - 2]`.
2. Since `res[N - 1] == seq[N - 1]` and all elements are distinct:
   - Moving `res[N - 1]` (`== seq[N - 1]`) to index `N - 2` gives `res[N - 2] != seq[N - 2]` (because `seq[N - 1] != seq[N - 2]`).
   - Moving the old `res[N - 2]` to index `N - 1` gives `res[N - 1] != seq[N - 1]` (because `old res[N - 2] != seq[N - 1]`, as `seq[N - 1]` was the one saved until index `N - 1`).
3. Furthermore, no earlier position `0..N - 3` can be improved without violating a derangement or reducing an earlier digit, so the swap preserves lexicographical optimality!

### Edge Cases
- `N <= 1`: No derangement is possible (a single element must stay in place).
- `N == 2`: Always swaps `[a, b]` -> `[b, a]`.
- Reverse-sorted input (like `[5, 4, 3, 2, 1]`): Hits both repeated collisions and the final swap!

---

## 3. Step-by-Step Trace (Test Case 1 from `main()`)

**Input:** `seq = [5, 4, 3, 2, 1]` (`N = 5`)

**Initial Max-Heap:** `{5, 4, 3, 2, 1}`

| Index `i` | `seq[i]` | `d = extractMax()` | Collision (`d == seq[i]`)? | Action | `res` So Far | Heap After Step |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `0` | `5` | `5` | Yes (`5 == 5`) | `nextMax = extractMax() = 4`; `res[0] = 4`; re-insert `5` | `[4, _, _, _, _]` | `{5, 3, 2, 1}` |
| `1` | `4` | `5` | No (`5 != 4`) | `res[1] = 5` | `[4, 5, _, _, _]` | `{3, 2, 1}` |
| `2` | `3` | `3` | Yes (`3 == 3`) | `nextMax = extractMax() = 2`; `res[2] = 2`; re-insert `3` | `[4, 5, 2, _, _]` | `{3, 1}` |
| `3` | `2` | `3` | No (`3 != 2`) | `res[3] = 3` | `[4, 5, 2, 3, _]` | `{1}` |
| `4` (`N-1`) | `1` | `1` | Yes (`1 == 1`) | Place `res[4] = 1`, then trigger end fix | `[4, 5, 2, 3, 1]` | `{}` |

**Final Swap Check:** `res[4] == seq[4]` (`1 == 1`), so swap `res[3]` and `res[4]`:
`res` becomes **`[4, 5, 2, 1, 3]`**.
Check against `[5, 4, 3, 2, 1]`: `4!=5, 5!=4, 2!=3, 1!=2, 3!=1` — valid and lexicographically largest!

---

## 4. Time & Space Complexity

- **Time Complexity:** `O(N log N)`. Inserting `N` elements takes `O(N log N)`. Each index performs at most 2 `extractMax` calls and 1 `insert`, each `O(log N)`.
- **Space Complexity:** `O(N)` for the Max-Heap and the result vector.

---

## 5. C++ Implementation

Source file: [10-largest-derangement-of-sequence.cpp](10-largest-derangement-of-sequence.cpp)

```cpp
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

class MaxHeap {
private:
    vector<int> items;
    int size;

    void heapifyUp(int curr);
    void maxHeapify(int curr);

public:
    MaxHeap();
    void insert(int x);
    int getMax();
    int extractMax();
    bool isEmpty();
    int getSize();
};

MaxHeap::MaxHeap() {
    items.push_back(0);
    size = 0;
}

void MaxHeap::heapifyUp(int curr) {
    int x = items[curr];
    while (curr / 2 > 0 && x > items[curr / 2]) {
        items[curr] = items[curr / 2];
        curr = curr / 2;
    }
    items[curr] = x;
}

void MaxHeap::maxHeapify(int curr) {
    int x = items[curr];
    while (curr * 2 <= size) {
        int s = curr * 2;
        if (s + 1 <= size && items[s] < items[s + 1]) {
            s++;
        }
        if (items[s] > x) {
            items[curr] = items[s];
            curr = s;
        } else {
            break;
        }
    }
    items[curr] = x;
}

void MaxHeap::insert(int x) {
    items.push_back(x);
    size++;
    heapifyUp(size);
}

int MaxHeap::getMax() {
    if (size == 0) {
        return INT_MIN;
    }
    return items[1];
}

int MaxHeap::extractMax() {
    if (size == 0) {
        return INT_MIN;
    }
    int maxVal = items[1];
    items[1] = items[size];
    items.pop_back();
    size--;
    if (size > 0) {
        maxHeapify(1);
    }
    return maxVal;
}

bool MaxHeap::isEmpty() {
    return size == 0;
}

int MaxHeap::getSize() {
    return size;
}

vector<int> largestDerangement(vector<int>& seq) {
    int n = (int)seq.size();
    if (n <= 1) {
        return {};
    }

    MaxHeap pq;
    for (int i = 0; i < n; i++) {
        pq.insert(seq[i]);
    }

    vector<int> res(n, 0);
    for (int i = 0; i < n; i++) {
        int d = pq.extractMax();
        if (d != seq[i] || i == n - 1) {
            res[i] = d;
        } else {
            int nextMax = pq.extractMax();
            res[i] = nextMax;
            pq.insert(d);
        }
    }

    // If the final position collided with seq[n - 1], swap the last two elements
    if (res[n - 1] == seq[n - 1]) {
        int temp = res[n - 1];
        res[n - 1] = res[n - 2];
        res[n - 2] = temp;
    }

    return res;
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < (int)arr.size()) {
            cout << ", ";
        }
    }
    cout << "]";
}

void runTest(string label, vector<int> seq) {
    cout << "=== " << label << " ===\n";
    cout << "Input:  ";
    printVector(seq);
    cout << "\n";

    vector<int> ans = largestDerangement(seq);
    if (ans.empty()) {
        cout << "Output: No derangement possible\n\n";
        return;
    }

    cout << "Output: ";
    printVector(ans);
    bool valid = true;
    for (int i = 0; i < (int)seq.size(); i++) {
        if (ans[i] == seq[i]) {
            valid = false;
        }
    }
    cout << " (valid derangement: " << (valid ? "YES" : "NO") << ")\n\n";
}

int main() {
    runTest("Test Case 1: Reverse-sorted sequence (triggers last-two swap)", {5, 4, 3, 2, 1});
    runTest("Test Case 2: Unsorted distinct sequence", {56, 21, 42, 67, 23, 74});
    runTest("Test Case 3: Smallest valid sequence", {1, 2});
    runTest("Test Case 4: Single element (impossible)", {9});
    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

This pattern is **"Greedy Placement with Temporary Holdback (Pop 1 or 2 + Re-insert)"**:

- **LC 767 — Reorganize String:** Greedily place the most frequent character; if it matches the previous character, pop the second most frequent and push the first back.
- **LC 1054 — Distant Barcodes:** Identical Max-Heap "pop top, fallback to second, re-insert top" pattern.
- **LC 1405 — Longest Happy String:** Greedily pick the highest-count character unless it would make 3 in a row, in which case pick the second-highest and re-insert.
- **LC 870 — Advantage Shuffle:** Greedy matching with sorted order / heap fallback when the best candidate cannot beat the target.
